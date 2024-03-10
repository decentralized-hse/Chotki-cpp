#include <iostream>
#include <toykv/toykv.hpp>

int main() {
  toykv::KeyValueStorage kv{true};

  if (auto err = kv.Open("test"); err != toykv::NoError) {
    return 1;
  }

  if (auto err = kv.Set('O', "123", "456"); err != toykv::NoError) {
    return 1;
  }

  const auto [value, err] = kv.Get('O', "123");
  if (err != toykv::NoError) {
    return 1;
  }

  std::cout << "value: " << value << std::endl;
}
