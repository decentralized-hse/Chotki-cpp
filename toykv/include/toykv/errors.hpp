#pragma once

namespace toykv {

enum class Error {
  NoError = 0,
  ErrAlreadyOpen = 1,
};
using enum Error;

}  // namespace toykv
