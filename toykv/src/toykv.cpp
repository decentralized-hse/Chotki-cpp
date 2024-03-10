#include <iostream>
#include <toykv/toykv.hpp>

namespace toykv {

namespace detail {

std::string ComposeKey(char lit, const std::string& key) {
  return std::string{lit} + key;
}

bool Success(rocksdb::Status status) {
  if (status.ok()) {
    return true;
  }

  std::cerr << "Database error: " << status.ToString() << std::endl;
  return false;
}

}  // namespace detail

KeyValueStorage::KeyValueStorage() {
}

rocksdb::DB* KeyValueStorage::DB() {
  return db_;
}

Error KeyValueStorage::Open(const std::string& name) {
  if (db_ != nullptr) {
    return ErrAlreadyOpen;
  }
  sync_ = name.ends_with(".db");

  rocksdb::Options o;
  o.create_if_missing = true;
  auto status = rocksdb::DB::Open(o, name, &db_);
  if (!detail::Success(status)) {
    return ErrInternalError;
  }

  return NoError;
}

std::pair<std::string, Error> KeyValueStorage::Get(
    char lit, const std::string& key) const {
  const auto k = detail::ComposeKey(lit, key);

  std::string value;
  auto status = db_->Get(rocksdb::ReadOptions{}, k, &value);
  if (status.IsNotFound()) {
    return {"", ErrNotFound};
  }
  if (!detail::Success(status)) {
    return {"", ErrInternalError};
  }

  return {value, NoError};
}

Error KeyValueStorage::Set(char lit, const std::string& key,
                           const std::string& value) {
  auto status = batch_.Put(detail::ComposeKey(lit, key), value);
  if (!detail::Success(status)) {
    return ErrInternalError;
  }

  return NoError;
}

Error KeyValueStorage::Merge(char lit, const std::string& key,
                             const std::string& value) {
  auto status = batch_.Merge(detail::ComposeKey(lit, key), value);
  if (!detail::Success(status)) {
    return ErrInternalError;
  }

  return Error::NoError;
}

Error KeyValueStorage::Commit() {
  rocksdb::WriteOptions o;
  o.disableWAL = !sync_;

  auto status = db_->Write(o, &batch_);
  if (!detail::Success(status)) {
    return ErrInternalError;
  }
  batch_.Clear();

  return NoError;
}

}  // namespace toykv
