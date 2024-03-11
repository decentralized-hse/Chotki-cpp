#include "blocking_queue.hpp"

namespace toyqueue::detail {

BlockingRecordQueue::BlockingRecordQueue(std::shared_ptr<RecordQueue> queue)
    : queue_{queue} {
}

Error BlockingRecordQueue::Close() {
  return queue_->Close();
}

std::pair<Records, Error> BlockingRecordQueue::Feed() {
  std::unique_lock lock{queue_->lock_};

  const bool was_full = queue_->records_.size() >= queue_->limit_;
  while (queue_->records_.empty()) {
    if (queue_->limit_ == 0) {
      return {{}, ErrClosed};
    }
    queue_->cv_.wait(lock);
  }

  Records ret;
  queue_->records_.swap(ret);
  if (was_full) {
    queue_->cv_.notify_all();
  }

  return {std::move(ret), NoError};
}

Error BlockingRecordQueue::Drain(Records records) {
  std::unique_lock lock{queue_->lock_};

  auto records_begin = records.begin();
  while (records_begin != records.end()) {
    const bool was_empty = queue_->records_.empty();
    while (queue_->limit_ <= queue_->records_.size()) {
      if (queue_->limit_ == 0) {
        return ErrClosed;
      }
      queue_->cv_.wait(lock);
    }

    const size_t cap =
        std::min(static_cast<size_t>(records.end() - records_begin),
                 queue_->limit_ - queue_->records_.size());
    std::move(std::make_move_iterator(records_begin),
              std::make_move_iterator(records_begin + cap),
              std::back_inserter(queue_->records_));
    records_begin += cap;

    if (was_empty) {
      queue_->cv_.notify_all();
    }
  }

  return NoError;
}

}  // namespace toyqueue::detail
