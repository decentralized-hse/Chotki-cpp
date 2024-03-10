#include "toykv/toykv.hpp"

namespace toykv {

KeyValueStorage(bool sync)
    : sync_{sync} {
}

Error Open(const std::string& name) {
  (void)name;

  return Error::NoError;
}

std::pair<std::string, Error> Get(std::byte lit, const std::string& key) const {
  (void)lit;
  (void)key;
  return {"", Error::NoError};
}

Error Set(char lit, std::string key, std::string value) {
  (void)lit;
  (void)key;
  (void)value;
  return Error::NoError;
}

Error Merge(char lit, std::string key, std::string value) {
  (void)lit;
  (void)key;
  (void)value;
  return Error::NoError;
}

Error Commit() {
  return Error::NoError;
}

}  // namespace toykv
