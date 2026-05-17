# FluCs

FluCs is a C-like language that compiles to C with built-in concurrency primitives. It provides three parallelism mechanisms: threads, processes, and parallel for loops, all of which can be used at any nesting level.

## Compilation Pipeline

```
source.flucs → Lexer → Parser (AST) → Semantic Analyzer → Codegen (C output) → gcc → run
```

The compiler is a single-pass transpiler. It parses FluCs source into an AST, performs scope and shared-variable analysis, then emits equivalent C code that uses `pthread` for threads and `fork()`/`mmap()` for processes.

## Concurrency Primitives

### 1. Thread Calls

Prefix any function call with `thread` to run it in a separate POSIX thread.

**Fire-and-forget (void return):**

```
func void log(int msg) {
    print(msg);
}

thread log(42);
```

Generates a `pthread_create` + `pthread_join` pair. The wrapper function is a C `void*` function that unpacks arguments from an `intptr_t` array:

```c
void* thread_call_1(void* arg) {
    intptr_t* args = (intptr_t*)arg;
    log((int)args[0]);
    return NULL;
}
```

**With result capture:**

```
func int add(int a, int b) {
    return a + b;
}

int result = thread add(4, 7);
await { result };
print(result);
```

The result variable's address is passed as `args[0]`, and the wrapper writes through it:

```c
void* thread_call_1(void* arg) {
    intptr_t* args = (intptr_t*)arg;
    *(int*)args[0] = add((int)args[1], (int)args[2]);
    return NULL;
}
```

At the call site:

```c
int result;
intptr_t _args_tw_1[3] = { (intptr_t)&result, 4, 7 };
pthread_t _thread_result;
pthread_create(&_thread_result, NULL, thread_call_1, _args_tw_1);
pthread_join(_thread_result, NULL);
```

### 2. Process Calls

Prefix a function call with `process` to run it in a forked child process. The result is communicated through shared memory created with `mmap(MAP_SHARED)`.

```
func int compute(int x) {
    return x * 2;
}

int val = process compute(21);
await { val };
print(val);
```

The codegen emits a process wrapper function that unpacks arguments from an `intptr_t` array, writes the result, and exits:

```c
void process_call_1(int* result, intptr_t* args) {
    *result = compute((int)args[0]);
    exit(0);
}
```

At the call site, arguments are packed into an array on the stack. Shared memory is allocated for the result, the process is forked, and the child runs the wrapper:

```c
intptr_t _args_p1[1] = { 21 };
int val = 0;
int* val_ptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE,
                    MAP_SHARED|MAP_ANONYMOUS, -1, 0);
*val_ptr = 0;
pid_t _process_val = fork();
if (_process_val == 0) { process_call_1(val_ptr, _args_p1); }
```

The parent later collects the result with `waitpid`:

```c
waitpid(_process_val, NULL, 0);
val = *val_ptr;
```

Processes are heavier than threads but provide full memory isolation. Arguments are evaluated at the call site before `fork()` and passed to the wrapper through an `intptr_t` array, the same pattern used by thread wrappers. Local variables, constants, and expressions all work as arguments.

### 3. Thread Blocks

A `thread { }` block runs all enclosed statements in a separate thread:

```
thread {
    print("hello from a thread");
    print("\n");
}
```

The block body becomes a `void*` wrapper function and is executed via `pthread_create`:

```c
void* thread_call_1(void* arg) {
    printf("%s","hello from a thread");
    printf("%s","\n");
    return NULL;
}
```

When variables are shared between the thread block and other thread contexts, the semantic analyzer detects this and automatically inserts mutex locks:

```
int counter = 0;

thread {
    counter += 10;
    counter += 20;
}

print(counter);
```

Here `counter` is accessed from both the thread block and the main thread, so it gets marked as shared. Every access is wrapped in lock/unlock:

```c
void* thread_call_1(void* arg) {
    pthread_mutex_lock(&lock_counter);
    counter += 10;
    pthread_mutex_unlock(&lock_counter);
    pthread_mutex_lock(&lock_counter);
    counter += 20;
    pthread_mutex_unlock(&lock_counter);
    return NULL;
}
```

### 4. Parallel For Loops

Prefix a `for` loop with `thread=N` to distribute iterations across N threads:

```
int counter = 0;
thread=3 for (int i = 0; i < 100; i++) {
    counter += i;
}
```

Each thread gets a strided slice of iterations. Thread 0 runs i=0,3,6,...; thread 1 runs i=1,4,7,...; thread 2 runs i=2,5,8,...

The codegen produces a worker function per loop:

```c
void *for_loop_worker_1(void *arg) {
    intptr_t* _args = (intptr_t*)arg;
    int start_index = (int)_args[0] + 0;
    for (int i = start_index; i < 100; i = i + 3) {
        pthread_mutex_lock(&lock_counter);
        counter += i;
        pthread_mutex_unlock(&lock_counter);
    }
    return NULL;
}
```

And the dispatch at the call site creates, starts, and joins all threads:

```c
pthread_t threads[3];
int starts[3];
for (int i = 0; i < 3; i++) {
    starts[i] = i;
    pthread_create(&threads[i], NULL, for_loop_worker_1, &starts[i]);
}
for (int i = 0; i < 3; i++) {
    pthread_join(threads[i], NULL);
}
```

### 5. Awaiting Results

`await { var1, var2 }` blocks the current thread until all listed variables are ready. For thread-sourced variables it calls `pthread_join`; for process-sourced variables it calls `waitpid` and reads from shared memory.

```
int a = thread add(1, 2);
int b = process add(3, 4);
await { a, b };
```

## Shared Variable Detection

The semantic analyzer tracks which thread context each variable is accessed from. When a variable is read or written by two or more different thread contexts, it is automatically marked `is_shared`. Shared variables get a `pthread_mutex_t` guard, where every read/write is wrapped in lock/unlock.

This detection is scope-aware. The analyzer maintains a scope stack with `uthash` hash tables. Each variable entry records which `thread_id`s have accessed it. The `thread_id` changes when entering a thread call, thread block, or parallel for loop body.

For parallel for loops, the analyzer also detects **captured variables**, which are variables from an outer scope that the loop body references. Captured variables are passed to worker functions by pointer, along with their associated mutex, via the `intptr_t` argument array.

## Nesting

All concurrency primitives work at any nesting depth, including inside `if`/`else` branches, `for` loops, function bodies, or other thread/process contexts:

```
func int add(int a, int b) { return a + b; }

if (1) {
    int r = thread add(5, 15);
    await { r };
    print(r);
}
```

The codegen assigns unique IDs to each thread/process call during a pre-pass (`assign_parallel_ids`) that recursively walks the entire AST. Wrapper functions are generated before `main()`, and inline code (`pthread_create`, `fork`, etc.) is emitted at the exact point of use. Forward declarations resolve circular references between user functions and wrapper functions.

## Build

```bash
gcc -o codegen codegen.c lexer.c parser.c semantic_analyzer.c -Wall -Wextra
./codegen source.flucs
```

The compiler writes generated C to `temp.c`, compiles it with `gcc`, and runs the result.

## Test Suite

```bash
./test.sh
```

Runs all 19 test cases covering top-level and nested thread, process, thread block, and parallel for loop usage.
