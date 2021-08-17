## cpputil
useful but **NOT** fully tested cpp macros & data structures. **You should only use it in your toy project**

### file description
Noted that all included files have been omitted in following examples. 
### util
#### defer.hpp
macro implemention for golang-flavored `defer` keyword

**Example**:
```cpp
std::ofstream f1("test.txt");
int a = 8;
DEFER({
    std::cout << "defer called, " << "value of variable a(int) is : " << a << std::endl;
    f1.close();
});
f1 << "hello defer" << std::endl;
a++;
return 0;
```

### concurrency
#### ring-buffer.hpp
lock-free mpmc queue with linked list implemention.
#### murmurhash.hpp
murmurhash implemention.
