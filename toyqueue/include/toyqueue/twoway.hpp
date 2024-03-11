#include <memory>
#include <toyqueue/drainfeed.hpp>

namespace toyqueue {

class BiChannel : public FeedDrainer {
 public:
  std::pair<Records, Error> Feed() override;
  Error Drain(Records records) override;

 private:
  friend class TwoWayChannel;

  std::shared_ptr<Drainer> drainer_;
  std::shared_ptr<Feeder> feeder_;
};

class TwoWayChannel {
 public:
  std::shared_ptr<FeedDrainer> Inner();
  std::shared_ptr<FeedDrainer> Outer();

 private:
  std::shared_ptr<FeedDrainer> outbound_;
  std::shared_ptr<FeedDrainer> inbound_;
};

}  // namespace toyqueue
