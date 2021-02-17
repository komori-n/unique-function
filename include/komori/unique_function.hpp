#pragma once

#include <utility>
#include <functional>

namespace komori {
  namespace detail {
    template <typename F, typename Res, typename... ArgTypes>
    struct invoke_helper {
      static Res invoke(void* storage, ArgTypes&&... args) {
        return
          std::invoke(*static_cast<F*>(storage), std::forward<ArgTypes>(args)...);
      }

      static void deleter(void* storage) {
        delete static_cast<F*>(storage);
      }
    };
  }


  template <typename T>
  class unique_function;

  template <typename Res, typename... ArgTypes>
  class unique_function<Res(ArgTypes...)> {
    using invoker_t = Res(*)(void*, ArgTypes...);
    using deleter_t = void(*)(void*);

    template <typename F>
    using helper = detail::invoke_helper<F, Res, ArgTypes...>;

  public:
    unique_function(void) : storage_(nullptr), invoker_(nullptr), deleter_(nullptr) {}

    unique_function(nullptr_t) : storage_(nullptr), invoker_(nullptr), deleter_(nullptr) {}
    template <typename F, typename DF=std::decay_t<F>>
    unique_function(F&& f)
    : storage_(new DF(std::forward<F>(f))),
      invoker_(&helper<DF>::invoke),
      deleter_(&helper<DF>::deleter) {}

    unique_function(unique_function&& f) noexcept
        : storage_(f.storage_), invoker_(f.invoker_), deleter_(f.deleter_) {
      f.storage_ = nullptr;
      f.invoker_ = nullptr;
      f.deleter_ = nullptr;
    }
    template <typename F>
    unique_function& operator=(F&& f) {
      unique_function(std::forward<F>(f)).swap(*this);
      return *this;
    }
    unique_function& operator=(unique_function&& f) {
      f.swap(*this);
      return *this;
    }

    unique_function(const unique_function&) = delete;
    unique_function& operator=(const unique_function&) = delete;
    ~unique_function(void) {
      if (storage_) {
        deleter_(storage_);
        storage_ = nullptr;
      }
    }

    explicit operator bool(void) const {
      return storage_;
    }

    void swap(unique_function& f) {
      std::swap(storage_, f.storage_);
      std::swap(invoker_, f.invoker_);
      std::swap(deleter_, f.deleter_);
    }

    Res operator()(ArgTypes... args) {
      return invoker_(storage_, std::forward<ArgTypes>(args)...);
    }

  private:
    void* storage_;
    invoker_t invoker_;
    deleter_t deleter_;
  };

  template <typename Res, typename... ArgTypes>
  inline bool operator==(const unique_function<Res(ArgTypes...)>& func, std::nullptr_t) {
    return !func;
  }

  template <typename Res, typename... ArgTypes>
  inline bool operator==(std::nullptr_t, const unique_function<Res(ArgTypes...)>& func) {
    return !func;
  }

  template <typename Res, typename... ArgTypes>
  inline bool operator!=(const unique_function<Res(ArgTypes...)>& func, std::nullptr_t) {
    return func;
  }

  template <typename Res, typename... ArgTypes>
  inline bool operator!=(std::nullptr_t, const unique_function<Res(ArgTypes...)>& func) {
    return func;
  }
};
