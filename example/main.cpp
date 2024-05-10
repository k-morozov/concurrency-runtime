#include <iostream>

#include "core/executor/executor.h"

int main() {
  std::cout << "Hello, world!" << std::endl;

  auto ex = NExe::MakeThreadPool();

  return 0;
}
