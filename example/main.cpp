#include <iostream>

#include "pool/executor/executor.h"

int main() {
  std::cout << "Hello, world!" << std::endl;

    auto ex = pool::MakeThreadPool(4);

    return 0;
}
