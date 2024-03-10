#include <iostream>

#include "toykv/toykv.hpp"

int main() {
  toykv::KeyValueStorage kv{true};

  auto err = kv.Open("test");
  if (err != toykv::NoError) {
    return 1;
  }

  auto err = kv.Set('O', "123", "456");
  if (err != toykv::NoError) {
    return 1;
  }

  const auto [value, err] = kv.Get('O', "123");
  if (err != toykv::NoError) {
    return 1;
  }

  std::cout << "value: " << value << std::endl;
}
