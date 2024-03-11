#pragma once

namespace toyqueue {

enum class Error {
  NoError = 0,
  ErrWouldBlock = 1,  // the queue is over capacity
  ErrClosed = 2,      // queue is closed
};
using enum Error;

}  // namespace toyqueue
