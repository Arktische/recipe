#include "defer.hpp"

#include <gtest/gtest.h>
#include <time.h>

#include <fstream>
#include <iostream>

#include "log.hpp"

TEST(misc_test, test_defer) {
  int a{1};
  
  defer({ LOG << a << std::endl; });
  
  defer({ a++; });
  
  defer({ LOG << a << std::endl; });
  
  LOG << a << std::endl;
}
