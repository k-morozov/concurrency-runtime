#include <iostream>

#include <executor/pool/intrusive_pool.h>
#include <go/go.h>

int main() {
    std::cout << "Hello, world!" << std::endl;

    NExecutors::IntrusiveThreadPool pool(1);
    pool.Start();

    fibers::Go(pool, [](){
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

    pool.WaitIdle();
    return 0;
}
