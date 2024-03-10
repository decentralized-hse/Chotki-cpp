#pragma once

#include <rocksdb/db.h>

#include <string>
#include <toykv/errors.hpp>

namespace toykv {

class KeyValueStorage {
 public:
  rocksdb::DB* DB();

  KeyValueStorage();

  Error Open(const std::string& name);

  std::pair<std::string, Error> Get(char lit, const std::string& key) const;

  Error Set(char lit, const std::string& key, const std::string& value);
  Error Merge(char lit, const std::string& key, const std::string& value);
  Error Commit();

 private:
  rocksdb::DB* db_{nullptr};
  rocksdb::WriteBatch batch_;
  bool sync_{};
};

}  // namespace toykv
