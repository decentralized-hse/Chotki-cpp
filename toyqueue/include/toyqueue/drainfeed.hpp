#pragma once

#include <cstdint>
#include <toyqueue/errors.hpp>
#include <vector>

namespace toyqueue {

using Record = std::vector<uint8_t>;
using Records = std::vector<Record>;

class Feeder {
 public:
  virtual ~Feeder() = default;
  virtual std::pair<Records, Error> Feed() = 0;
};

class FeedCloser : public Feeder {
 public:
  virtual Error Close() = 0;
};

class FeedSeeker : public FeedCloser {
 public:
  virtual std::pair<int64_t, Error> Seek(int64_t offset, int whence) = 0;
};

class Drainer {
 public:
  virtual ~Drainer() = default;
  virtual Error Drain(Records records) = 0;
};

class DrainCloser : public Drainer {
 public:
  virtual Error Close() = 0;
};

class DrainSeeker : public DrainCloser {
 public:
  virtual std::pair<int64_t, Error> Seek(int64_t offset, int whence) = 0;
};

class FeedDrainer : public Feeder,
                    public Drainer {};

class FeedDrainCloser : public FeedDrainer {
 public:
  virtual Error Close() = 0;
};

}  // namespace toyqueue
