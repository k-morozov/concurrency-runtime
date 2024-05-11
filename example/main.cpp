#include <iostream>

#include "pool/executor/executor.h"

int main() {
  std::cout << "Hello, world!" << std::endl;

    auto ex = pool::MakeThreadPool();

    return 0;
}
