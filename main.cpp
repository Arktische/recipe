#include <iostream>
#include <fstream>
#include <time.h>
#include "util/defer.hpp"
using namespace cpputil;

int test_defer() {
    int a = 1;
    DEFER({
        a++;
          });
    return a;
}

int main() {
    int res = test_defer();
    std::cout << res << std::endl;
    return 0;
}
