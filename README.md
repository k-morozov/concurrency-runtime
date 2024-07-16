[![Build](https://github.com/k-morozov/go-runtime/actions/workflows/meson.yaml/badge.svg?branch=master)](https://github.com/k-morozov/go-runtime/actions/workflows/meson.yaml)

## Concurrency runtime in  C++

## What I did?

- [library for switch context](#switch-context)
- [stackfull coroutine](#stackfull-coroutine)
- [executor](#executor)
- [fibers](#fibers)
- [common components library](#common-components)

----------------------------------------

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


### Switch context

in progres

### Stackfull coroutine

in progres

### Executor

in progres

### Fibers

in progres

### Common components

- function
- spinlock
- intrusive list
- lock free queue
