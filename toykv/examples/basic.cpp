#include <iostream>
#include <toykv/toykv.hpp>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <database-file>" << std::endl;
    return 1;
  }

  toykv::KeyValueStorage kv;
  {
    auto err = kv.Open(argv[1]);
    assert(err == toykv::NoError);
  }

  {
    auto err = kv.Open(argv[1]);
    assert(err == toykv::ErrAlreadyOpen);
  }

  {
    auto err = kv.Set('O', "123", "456");
    assert(err == toykv::NoError);
  }

  {
    const auto [value, err] = kv.Get('O', "123");
    assert(err == toykv::ErrNotFound);  // Because not commited
  }

  {
    auto err = kv.Commit();
    assert(err == toykv::NoError);
  }

  {
    const auto [value, err] = kv.Get('O', "123");
    assert(err == toykv::NoError);
    std::cout << "Value: " << value << std::endl;
  }

  {
    const auto [value, err] = kv.Get('O', "456");
    assert(err == toykv::ErrNotFound);
  }
}
