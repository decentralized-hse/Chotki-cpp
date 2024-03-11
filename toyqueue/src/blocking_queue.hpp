#include <toyqueue/queue.hpp>

namespace toyqueue::detail {

class BlockingRecordQueue : public FeedDrainCloser {
 public:
  BlockingRecordQueue(std::shared_ptr<RecordQueue> queue);

  std::pair<Records, Error> Feed() override;
  Error Drain(Records records) override;
  Error Close() override;

 private:
  std::shared_ptr<RecordQueue> queue_;
};

}  // namespace toyqueue::detail
