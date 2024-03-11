#include <toyqueue/twoway.hpp>

namespace toyqueue {

std::pair<Records, Error> BiChannel::Feed() {
  return feeder_->Feed();
}

Error BiChannel::Drain(Records records) {
  return drainer_->Drain(std::move(records));
}

std::shared_ptr<FeedDrainer> TwoWayChannel::Inner() {
  auto bichannel = std::make_shared<BiChannel>();
  bichannel->drainer_ = outbound_;
  bichannel->feeder_ = inbound_;

  return bichannel;
}

std::shared_ptr<FeedDrainer> TwoWayChannel::Outer() {
  auto bichannel = std::make_shared<BiChannel>();
  bichannel->drainer_ = inbound_;
  bichannel->feeder_ = outbound_;

  return bichannel;
}

}  // namespace toyqueue
