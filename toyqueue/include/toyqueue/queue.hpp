#pragma once

#include <condition_variable>
#include <mutex>
#include <toyqueue/drainfeed.hpp>
#include <toyqueue/errors.hpp>

namespace toyqueue {

namespace detail {

class BlockingRecordQueue;

}  // namespace detail

class RecordQueue : public FeedDrainCloser,
                    public std::enable_shared_from_this<RecordQueue> {
 public:
  RecordQueue(int limit);

  int Limit() const;
  void SetLimit(int limit);

  std::pair<Records, Error> Feed() override;
  Error Drain(Records records) override;
  Error Close() override;

  std::shared_ptr<FeedDrainCloser> Blocking();

 private:
  friend class detail::BlockingRecordQueue;

  Records records_;
  mutable std::mutex lock_;
  std::condition_variable cv_;
  int limit_;
};

}  // namespace toyqueue
