### Goroutine in C++

I try to make this code is true in C++.

[example_go.cpp](example%2Fgo/%2Fexample_go.cpp)

```cpp
fibers::Go(*ex, [](){
    std::cout << "1" << std::endl;

    fibers::Go([] {
        std::cout << "3" << std::endl;
        fibers::Yield();
        std::cout << "5" << std::endl;

        fibers::Go([] {
            std::cout << "7" << std::endl;
            fibers::Yield();
            std::cout << "9" << std::endl;
        });

        fibers::Yield();
        std::cout << "8" << std::endl;
    });

    std::cout << "2" << std::endl;
    fibers::Yield();
    std::cout << "4" << std::endl;
    fibers::Yield();
    std::cout << "6" << std::endl;
});

Output:
1
2
3
4
5
6
7
8
9
```