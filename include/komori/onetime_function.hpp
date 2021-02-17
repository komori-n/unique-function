#pragma once

#include <utility>
#include <functional>

namespace komori {
  namespace detail {
    template <typename F, typename Res, typename... ArgTypes>
    struct invoke_helper {
      static Res invoker(void* storage, ArgTypes&&... args) {
        return
          std::invoke(*static_cast<F*>(storage), std::forward<ArgTypes>(args)...);
      }

      static void deleter(void* storage) {
        delete static_cast<F*>(storage);
      }
    };
  }


  template <typename T>
  class onetime_function;

  template <typename Res, typename... ArgTypes>
  class onetime_function<Res(ArgTypes...)> {
    using invoker_t = Res(*)(void*, ArgTypes...);
    using deleter_t = void(*)(void*);

    template <typename F>
    using helper = detail::invoke_helper<F, Res, ArgTypes...>;

  public:
    onetime_function(void) : storage_(nullptr), invoker_(nullptr), deleter_(nullptr) {}

    onetime_function(nullptr_t) : storage_(nullptr), invoker_(nullptr), deleter_(nullptr) {}
    template <typename F, typename DF=std::decay_t<F>>
    onetime_function(F&& f)
    : storage_(new DF(std::forward<F>(f))),
      invoker_(&helper<DF>::invoker),
      deleter_(&helper<DF>::deleter) {}

    onetime_function(onetime_function&& f) noexcept
        : storage_(f.storage_), invoker_(f.invoker_), deleter_(f.deleter_) {
      f.storage_ = nullptr;
      f.invoker_ = nullptr;
      f.deleter_ = nullptr;
    }
    template <typename F>
    onetime_function& operator=(F&& f) {
      onetime_function(std::forward<F>(f)).swap(*this);
      return *this;
    }

    onetime_function(const onetime_function&) = delete;
    onetime_function& operator=(const onetime_function&) = delete;
    ~onetime_function(void) {
      if (storage_) {
        deleter_(storage_);
        storage_ = nullptr;
        invoker_ = nullptr;
        deleter_ = nullptr;
      }
    }

    explicit operator bool(void) const {
      return storage_;
    }

    void swap(onetime_function& f) {
      std::swap(storage_, f.storage_);
      std::swap(invoker_, f.invoker_);
      std::swap(deleter_, f.deleter_);
    }

    template <std::nullptr_t Dummy = nullptr>
    auto operator()(ArgTypes&&... args)
    -> std::enable_if_t<!std::is_same<Res, void>::value && Dummy == nullptr, Res> {
      if (storage_) {
        auto&& res = invoker_(storage_, std::forward<ArgTypes>(args)...);
        onetime_function().swap(*this);
        return std::forward<Res>(res);
      } else {
        throw std::runtime_error("storage is null");
      }
    }

    template <std::nullptr_t Dummy = nullptr>
    auto operator()(ArgTypes&&... args)
    -> std::enable_if_t<std::is_same<Res, void>::value && Dummy == nullptr, Res> {
      if (storage_) {
        invoker_(storage_, std::forward<ArgTypes>(args)...);
        onetime_function().swap(*this);
      } else {
        throw std::runtime_error("storage is null");
      }
    }

  private:
    void* storage_;
    invoker_t invoker_;
    deleter_t deleter_;
  };

  template <typename Res, typename... ArgTypes>
  inline bool operator==(const onetime_function<Res(ArgTypes...)>& func, std::nullptr_t) {
    return !func;
  }

  template <typename Res, typename... ArgTypes>
  inline bool operator==(std::nullptr_t, const onetime_function<Res(ArgTypes...)>& func) {
    return !func;
  }

  template <typename Res, typename... ArgTypes>
  inline bool operator!=(const onetime_function<Res(ArgTypes...)>& func, std::nullptr_t) {
    return func;
  }

  template <typename Res, typename... ArgTypes>
  inline bool operator!=(std::nullptr_t, const onetime_function<Res(ArgTypes...)>& func) {
    return func;
  }
}
