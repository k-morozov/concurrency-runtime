#include <iostream>

#include "pool/executor/executor.h"

int main() {
  std::cout << "Hello, world!" << std::endl;

  auto ex = NExe::MakeThreadPool();

  return 0;
}
