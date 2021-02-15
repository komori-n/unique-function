#include <iostream>
#include <cstdlib>
#include <future>

#include "komori/onetime_function.hpp"

int main(void) {
  komori::onetime_function<void()> fn = [](void) {
    std::cout << "onetime example" << std::endl;
  };

  if (fn) fn();
  if (fn) fn();  // never called


  std::promise<int> promise;
  auto future = promise.get_future();
  komori::onetime_function<void()> gn = [promise=std::move(promise)](void) mutable {
    std::cout << "wait.." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    promise.set_value(334);
  };

  gn();
  std::cout << future.get() << std::endl;

  if (!gn) {
    std::cout << "gn no longer exist" << std::endl;
  }

  return EXIT_SUCCESS;
}
