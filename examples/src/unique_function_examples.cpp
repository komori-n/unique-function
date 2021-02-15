#include <iostream>
#include <cstdlib>
#include <future>

#include "komori/unique_function.hpp"

int main(void) {
  std::promise<int> promise;
  auto future = promise.get_future();

  komori::unique_function<void()> fn = [promise=std::move(promise)](void) mutable {
    std::cout << "wait.." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    promise.set_value(334);
  };

  fn();
  std::cout << future.get() << std::endl;

  return EXIT_SUCCESS;
}
