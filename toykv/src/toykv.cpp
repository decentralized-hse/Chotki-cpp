#include <rocksdb/merge_operator.h>

#include <iostream>
#include <toykv/toykv.hpp>

#include "concatenate_merger.hpp"

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

KeyValueIterator::KeyValueIterator(std::string from_key, std::string till_key)
    : from_key_{std::move(from_key)},
      till_key_{std::move(till_key)},
      from_slice_{from_key_},
      till_slice_{till_key_} {
}

KeyValueIterator::~KeyValueIterator() {
  assert(iter_ == nullptr);
}

void KeyValueIterator::Close() {
  if (iter_ != nullptr) {
    delete iter_;
    iter_ = nullptr;
  }
}

bool KeyValueIterator::Valid() const {
  return iter_ != nullptr && iter_->Valid();
}

char KeyValueIterator::Liter() const {
  return Valid() ? iter_->key()[0] : 0;
}

std::string KeyValueIterator::Key() const {
  if (!Valid()) {
    return "";
  }

  const auto key = iter_->key().ToString();
  return key.substr(1);
}

std::string KeyValueIterator::Value() const {
  if (!Valid()) {
    return "";
  }

  return iter_->value().ToString();
}

bool KeyValueIterator::Next() {
  if (!Valid()) {
    return false;
  }

  iter_->Next();

  if (!iter_->Valid()) {
    Close();
    return false;
  }

  return true;
}

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
  o.merge_operator = std::make_shared<ConcatenateMerger>();
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

KeyValueIterator KeyValueStorage::Range(char lit, const std::string& from,
                                        const std::string& till) const {
  auto from_key = detail::ComposeKey(lit, from);
  auto till_key = detail::ComposeKey(lit, till);

  if (from_key.compare(till_key) > 0) {
    from_key.swap(till_key);
  }

  KeyValueIterator kvi{std::move(from_key), std::move(till_key)};

  rocksdb::ReadOptions io;
  io.iterate_lower_bound = &kvi.from_slice_;
  io.iterate_upper_bound = &kvi.till_slice_;

  kvi.iter_ = db_->NewIterator(io);
  kvi.iter_->SeekToFirst();

  return kvi;
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
  o.sync = sync_;

  auto status = db_->Write(o, &batch_);
  if (!detail::Success(status)) {
    return ErrInternalError;
  }
  batch_.Clear();

  return NoError;
}

void KeyValueStorage::Close() {
  if (db_ != nullptr) {
    if (auto status = db_->Close(); !status.ok()) {
      std::cerr << "Failed to close database: " << status.ToString();
    }
    delete db_;
    db_ = nullptr;
  }
}

}  // namespace toykv
