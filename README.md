### Goroutine in C++

I like Go for this:

```go
go some_func
```

I try to make this code is true in C++.

```cpp
pool::Executor pool{3};
pool.Start();

bool done = false;

fibers::Go(pool, [&]() {
    done = true;
});

pool.WaitIdle();
std::cout << done << std::endl;
```