#include <rocksdb/merge_operator.h>
#include <rocksdb/slice.h>

namespace toykv {

class ConcatenateMerger : public rocksdb::MergeOperator {
 public:
  ConcatenateMerger();

  const char* Name() const override {
    return "ConcatenateMerger";
  }

  bool FullMerge(const rocksdb::Slice& key,
                 const rocksdb::Slice* existing_value,
                 const std::deque<std::string>& operand_list,
                 std::string* new_value,
                 rocksdb::Logger* logger) const override;

  bool PartialMerge(const rocksdb::Slice& key,
                    const rocksdb::Slice& left_operand,
                    const rocksdb::Slice& right_operand, std::string* new_value,
                    rocksdb::Logger* logger) const override;
};

}  // namespace toykv
