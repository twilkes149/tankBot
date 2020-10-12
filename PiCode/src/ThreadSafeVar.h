#ifndef THREAD_SAFE_VAR_H
#define THREAD_SAFE_VAR_H
#include <mutex>

template <class type>
class ThreadSafeVar {
  private:
    type var_;
    std::mutex m_;

  public:
    ThreadSafeVar() {}
    ThreadSafeVar(type var) {
      m_.lock();
      var_ = val;
      m_.lock();
    }

    type read() {
      type returnVal;
      m_.lock();
      returnVal = var_;
      m_.unlock();
      return returnVal;
    }

    void write(type val) {
      m_.lock();
      var_ = val;
      m_.unlock();
    }

    type& operator=(const type rhs) = delete;
    type operator==(const type rhs) = delete;
    type operator!=(const type rhs) = delete;
};

#endif