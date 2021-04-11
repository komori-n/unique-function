# unique-function

[![Test](https://github.com/komori-n/unique-function/actions/workflows/test.yml/badge.svg)](https://github.com/komori-n/unique-function/actions/workflows/test.yml)
![examples](https://github.com/komori-n/unique-function/workflows/examples/badge.svg)

A simple move-only std::function replacement

## Getting Started

```sh
git clone https://github.com/komori-n/unique-function
```

## How to Use

### `komori::unique_function`

`komori::unique_function` is move-only version of `std::function`.
It can store function pointers, function objects, and lambdas.

```cpp
#include <iostream>
#include <future>
#include "komori/unique_function.hpp"

int main(void) {
  std::promise<int> promise;
  auto future = promise.get_future();

  // unique_function can store move-captured functors.
  komori::unique_function<void()> fn = [promise=std::move(promise)](void) mutable {
    promise.set_value(334);
  };

  fn();
  std::cout << future.get() << std::endl;

  return 0;
}
```

### `komori::onetime_function`

`komori::onetime_function` is a variation of `komori::unique_function` that can only be called once.

```cpp
#include <iostream>
#include <future>
#include "komori/onetime_function.hpp"

int main(void) {
  std::promise<int> promise;
  auto future = promise.get_future();

  // onetime_function can store move-captured functors.
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
- [ ] qualifiers(const, noexcept)
- [x] comparison operator with `std::nullptr_t`
