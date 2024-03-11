#include <toyqueue/queue.hpp>

#include "blocking_queue.hpp"

namespace toyqueue {

RecordQueue::RecordQueue(int limit)
    : limit_{limit} {
}

int RecordQueue::Limit() const {
  std::lock_guard guard{lock_};

  return limit_;
}

void RecordQueue::SetLimit(int limit) {
  std::lock_guard guard{lock_};

  limit_ = limit;
}

std::shared_ptr<FeedDrainCloser> RecordQueue::Blocking() {
  return std::make_shared<detail::BlockingRecordQueue>(shared_from_this());
}

Error RecordQueue::Close() {
  SetLimit(0);

  return NoError;
}

std::pair<Records, Error> RecordQueue::Feed() {
  std::unique_lock lock{lock_};

  if (records_.empty()) {
    return {{}, limit_ == 0 ? ErrClosed : ErrWouldBlock};
  }

  const bool was_full = records_.size() >= limit_;
  Records ret;
  records_.swap(ret);
  if (was_full) {
    cv_.notify_all();
  }

  return {std::move(ret), NoError};
}

Error RecordQueue::Drain(Records records) {
  std::unique_lock lock{lock_};

  const bool was_empty = records_.empty();
  const size_t result_size = records_.size() + records.size();
  if (result_size > limit_) {
    return limit_ == 0 ? ErrClosed : ErrWouldBlock;
  }

  records_.reserve(result_size);
  std::move(std::make_move_iterator(records.begin()),
            std::make_move_iterator(records.end()),
            std::back_inserter(records_));
  if (was_empty) {
    cv_.notify_all();
  }

  return NoError;
}

}  // namespace toyqueue
