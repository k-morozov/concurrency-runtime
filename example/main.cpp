#include <iostream>

#include <executor/thread_pool//thread_pool.h>

int main() {
  std::cout << "Hello, world!" << std::endl;

    auto ex = pool::MakeThreadPool(4);

    return 0;
}
