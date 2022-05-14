#pragma
#include <functional>
#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)
#define defer(fn) CppDefer CONCAT(__defer__, __LINE__) = [&]() { fn; }
class CppDefer {
 public:
  template <class Func>
  CppDefer(Func&& fn) : fn_(std::forward<Func>(fn)) {}
  CppDefer(CppDefer&& other) noexcept : fn_(std::move(other.fn_)) {
    other.fn_ = nullptr;
  }
  ~CppDefer() {
    if (fn_) fn_();
  }

  CppDefer(const CppDefer&) = delete;
  void operator=(const CppDefer&) = delete;

 private:
  std::function<void()> fn_;
};
