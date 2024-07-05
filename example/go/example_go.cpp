#include <iostream>

#include <executor/pool/intrusive_pool.h>
#include <go/go.h>

int main() {
    auto a1 = new int(1);

    auto b1 = new int(2);
    std::cout << "b1=" << b1 << std::endl;

    auto c1 = new int(4);
    delete c1;
    delete b1;

    auto b2 = new int(3);
    std::cout << "b2=" << b2 << std::endl;
    delete b2;

    delete a1;
    return 0;
}
