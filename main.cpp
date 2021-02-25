#include <iostream>
#include <fstream>
#include <time.h>
#include "util/defer.hpp"
using namespace cpputil;

int test_defer() {
    std::ofstream f1("test.txt");
    int a = 8;
    time_t start_ts, finish_ts;
    time(&start_ts);
    DEFER({
              std::cout << "defer called, " << "value of variable a(int) is : " << a << std::endl;
              f1.close();
          });
    time(&finish_ts);
    f1 << "hello defer" << std::endl;
    std::cout << "defer consume time:" << (finish_ts - start_ts)/1000 << "ms" << std::endl;
    a++;
    return 0;
}

int main() {
    test_defer();
}
