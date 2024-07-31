[![Build](https://github.com/k-morozov/go-runtime/actions/workflows/meson.yaml/badge.svg?branch=master)](https://github.com/k-morozov/go-runtime/actions/workflows/meson.yaml)

## Concurrency runtime in  C++

## What I did?

- [library for switch context](#switch-context)
- [stackfull coroutine](#stackfull-coroutine)
- [executor](#executor)
- [fibers](#fibers)
- [common components library](#common-components)

----------------------------------------
### Intro

I really wanted to understand how Go's runtime works. I suppose that the best way to research smth - write it by yourself. So this project is only my effort to understand it. This lib doesn't ready for production. For example I wrote specific context switching and I don't know how it works on Window/MacOS or another CPU arch. I try to write good unit tests with diffrent sanitizers and cases. However, I don't think it's enough. For example for simular project fault injection is a good way to found a conccurancy problem. 

***I must admit that every decision is made in the design of the system imperfectly. Every design is trade off.***

When I work with this lib I understood that concurrency is really too hard.

### Requirements

- x86-64
- Linux
- Clang++18

### How to build

You can find more information:
- [configure file](tools/configure.py)
- [CI file](.github/workflows/target_build_types.yaml)

```shell
sudo apt-get install python3 python3-pip python3-setuptools python3-wheel ninja-build
python3 -m venv env
pip install -r requirements.txt
sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
# /usr/bin/clang++ is clang++-18
conan install . -of=build --build=missing -pr:a=./config/debug_profile
meson setup --native-file build/conan_meson_native.ini build . --buildtype debug
meson compile -C build
meson test -C build
```

### Bench

[see here](bench/readme.md)

### Perf results

[see here](doc/workload.svg)

-----------------------------------------------

## What have I implemented?

### Switch context

Main part for implementation stackfull coroutine.

- [start see](src/context/context.h)
- [asm core](src/context/buffer/prepare_context.s)

### Stackfull coroutine

My implementation of stackfull coroutine.

- [see here](src/coro/stackfull_coro.h)

### Executor

Scheduler, executor.

- [manual executor, step by step](src/executor/manual/intrusive_manual_executor.h)
- [simple intrusive thread pool](src/executor/pool/intrusive_pool.h)
- [strand executor](src/executor/strand/intrusive_strand.h)
- [my last work - thread pool with sharded workers](src/executor/pool/distributed_pool.h)

### Fibers

Stackfull coroutine + Executor = Fibers. Im proud of async mutex, wait group, event.

- [Fiber](src/fiber/awaiter_fiber.h)
- [async mutex](src/fiber/sync/async_mutex.h)
- [async wait_group](src/fiber/sync/wait_group.h)

### Common components

- [function](src/components/function/basic_function.h)
- [spinlock](src/components/sync/spinLock.h)
- [simple intrusive list](src/components/intrusive/list.h)
- [lock free queue](src/components/lock_free/simple_ms_queue.h)
- [queue spinlock](src/components/sync/queue_spinlock.h)

P.S. Thanks for R.Lipovsky for his wonderfully program :)