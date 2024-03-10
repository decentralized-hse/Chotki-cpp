#pragma once

#include <string>

#include "errors.hpp"

namespace toykv {

class KeyValueStorage {
 public:
  KeyValueStorage(bool sync = false);

  Error Open(const std::string& name);

  std::pair<std::string, Error> Get(std::byte lit,
                                    const std::string& key) const;

  Error Set(char lit, std::string key, std::string value);
  Error Merge(char lit, std::string key, std::string value);
  Error Commit();

 private:
  bool sync_;
};

}  // namespace toykv
