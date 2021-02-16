# unique-function

![examples](https://github.com/komori-n/unique-function/workflows/examples/badge.svg)

a simple move-only std::function replacement

## Getting Started

```sh
git clone https://github.com/komori-n/unique-function
```

## How to Use

### `komori::unique_function`

`komori::unique_function` is move-only version of `std::function`.
It can be used to store function pointers, function objects and lambdas.

```cpp
#include <iostream>
#include "komori/unique_function.hpp"

int main(void) {
  std::promise<int> promise;
  auto future = promise.get_future();

  // unique_function can store move-captured lambda.
  komori::unique_function<void()> fn = [promise=std::move(promise)](void) mutable {
    promise.set_value(334);
  };

  fn();
  std::cout << future.get() << std::endl;

  return 0;
}
```

### `komori::onetime_function`

`komori::onetime_function` is a version of `komori::unique_function` that can only be called once.

```cpp
#include <iostream>
#include "komori/onetime_function.hpp"

int main(void) {
  std::promise<int> promise;
  auto future = promise.get_future();

  // onetime_function can store move-captured lambda.
  komori::onetime_function<void()> fn = [promise=std::move(promise)](void) mutable {
    promise.set_value(334);
  };

  fn();
  std::cout << future.get() << std::endl;

  // after called once, the stored function is destructed.
  if (!fn) {
    std::cout << "fn is empty" << std::endl;
  }

  return 0;
}
```

## License

Licensed under GNU General Public License, version 3.0

## To do

- [ ] small object optimization(SOO)
- [ ] avoid using std::invoke(to work with c++-11)
- [ ] qualifiers(const, noexcept)
- [ ] comparison operator with `std::nullptr_t`
