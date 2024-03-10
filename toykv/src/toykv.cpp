#include <toykv/toykv.hpp>

namespace toykv {

KeyValueStorage::KeyValueStorage(bool sync)
    : sync_{sync} {
}

Error KeyValueStorage::Open(const std::string& name) {
  (void)name;

  return Error::NoError;
}

std::pair<std::string, Error> KeyValueStorage::Get(
    std::byte lit, const std::string& key) const {
  (void)lit;
  (void)key;
  return {"", Error::NoError};
}

Error KeyValueStorage::Set(char lit, std::string key, std::string value) {
  (void)lit;
  (void)key;
  (void)value;
  return Error::NoError;
}

Error KeyValueStorage::Merge(char lit, std::string key, std::string value) {
  (void)lit;
  (void)key;
  (void)value;
  return Error::NoError;
}

Error KeyValueStorage::Commit() {
  return Error::NoError;
}

}  // namespace toykv
