#pragma once

#include <rocksdb/db.h>

#include <string>
#include <toykv/errors.hpp>

namespace toykv {

/// @note KeyValueIterator must be
/// either fully exhausted with `Next()` calls
/// or be explicityly closed with `Closed`
class KeyValueIterator {
 public:
  ~KeyValueIterator();

  bool Valid() const;
  char Liter() const;
  std::string Key() const;
  std::string Value() const;

  bool Next();
  void Close();

 private:
  friend class KeyValueStorage;

  KeyValueIterator(std::string from_key, std::string till_key);

  rocksdb::Iterator* iter_{nullptr};
  const std::string from_key_;
  const std::string till_key_;
  rocksdb::Slice from_slice_;
  rocksdb::Slice till_slice_;
};

class KeyValueStorage {
 public:
  rocksdb::DB* DB();

  KeyValueStorage();

  /// Opens database with default merger,
  /// which just concatenates values
  Error Open(const std::string& name);

  std::pair<std::string, Error> Get(char lit, const std::string& key) const;
  KeyValueIterator Range(char lit, const std::string& from,
                         const std::string& till) const;

  Error Set(char lit, const std::string& key, const std::string& value);
  Error Merge(char lit, const std::string& key, const std::string& value);
  Error Commit();
  void Close();

 private:
  rocksdb::DB* db_{nullptr};
  rocksdb::WriteBatch batch_;
  bool sync_{};
};

}  // namespace toykv
