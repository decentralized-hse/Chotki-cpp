#include "concatenate_merger.hpp"

#include <rocksdb/env.h>

#include <iostream>

namespace toykv {

ConcatenateMerger::ConcatenateMerger() = default;

bool ConcatenateMerger::FullMerge(
    [[maybe_unused]] const rocksdb::Slice& key,
    const rocksdb::Slice* existing_value,
    [[maybe_unused]] const std::deque<std::string>& operand_list,
    std::string* new_value, [[maybe_unused]] rocksdb::Logger* logger) const {
  std::cerr << "Full merging with operand_list: ";
  for (const auto& op : operand_list) {
    std::cerr << "{" << op << "} ";
  }
  std::cerr << "\n";

  if (existing_value != nullptr) {
    std::cerr << "Existing value: " << existing_value->ToString() << std::endl;
    *new_value = existing_value->ToString();
  } else {
    std::cerr << "No existing value" << std::endl;
  }
  for (const auto& op : operand_list) {
    new_value->append(op);
  }

  return true;
}

bool ConcatenateMerger::PartialMerge(
    const rocksdb::Slice& key, const rocksdb::Slice& left_operand,
    const rocksdb::Slice& right_operand, std::string* new_value,
    [[maybe_unused]] rocksdb::Logger* logger) const {
  const auto left = left_operand.ToString();
  const auto right = right_operand.ToString();
  std::cerr << "Partially merging {" << right << "} and {" << right
            << "} for key {" << key.ToStringView() << "}";
  *new_value = left + right;

  return true;
}
}  // namespace toykv
