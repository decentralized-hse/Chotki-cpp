#pragma once

namespace toykv {

enum class Error {
  NoError = 0,
  ErrAlreadyOpen = 1,
  ErrNotFound = 2,
  ErrInternalError = 3,
};
using enum Error;

}  // namespace toykv
