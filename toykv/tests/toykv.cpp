#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <toykv/toykv.hpp>

namespace detail {

std::string Now() {
  const auto now = std::chrono::system_clock::now();
  const std::time_t now_c = std::chrono::system_clock::to_time_t(now);

  return std::ctime(&now_c);
}

class DbFileRemover {
 public:
  DbFileRemover(std::string db_name)
      : db_name_{std::move(db_name)} {
    std::filesystem::remove_all(db_name_);
  }

  ~DbFileRemover() {
    std::filesystem::remove_all(db_name_);
  }

 private:
  const std::string db_name_;
};

struct Int {
  int i;

  std::string String() const {
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(8) << i;
    return ss.str();
  }

  void Inc() {
    ++i;
  }
};

}  // namespace detail

TEST(TestToyKV, Simple) {
  detail::DbFileRemover _{"store.db"};

  toykv::KeyValueStorage kv;
  ASSERT_EQ(kv.Open("store.db"), toykv::NoError);
  ASSERT_TRUE(kv.DB());

  const auto now = detail::Now();

  ASSERT_EQ(kv.Merge('T', now, "now"), toykv::NoError);
  {
    const auto [_, err] = kv.Get('T', now);
    ASSERT_EQ(err, toykv::ErrNotFound);
  }
  ASSERT_EQ(kv.Commit(), toykv::NoError);
  {
    const auto [value, err] = kv.Get('T', now);
    ASSERT_EQ(err, toykv::NoError);
    ASSERT_EQ(value, "now");
  }

  ASSERT_EQ(kv.Merge('T', now, " is "), toykv::NoError);
  ASSERT_EQ(kv.Merge('T', now, now), toykv::NoError);
  ASSERT_EQ(kv.Commit(), toykv::NoError);
  {
    const auto [value, err] = kv.Get('T', now);
    ASSERT_EQ(err, toykv::NoError);
    ASSERT_EQ(value, "now is " + now);
  }

  kv.Close();
  ASSERT_FALSE(kv.DB());
}

TEST(TestToyKV, Range) {
  for (const auto db_name : {"non-sync", "sync.db"}) {
    detail::DbFileRemover _{db_name};

    toykv::KeyValueStorage kv;
    ASSERT_EQ(kv.Open(db_name), toykv::NoError);
    ASSERT_TRUE(kv.DB());

    auto set_begin = std::chrono::high_resolution_clock::now();
    detail::Int i{0};
    for (; i.i < (1 << 20); i.Inc()) {
      ASSERT_EQ(kv.Set('N', i.String(), "set"), toykv::NoError);
    }
    ASSERT_EQ(kv.Commit(), toykv::NoError);
    auto set_end = std::chrono::high_resolution_clock::now();

    auto set_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                            set_end - set_begin)
                            .count();
    std::cerr << db_name << " set phase duration: " << set_duration << "ms"
              << std::endl;

    const detail::Int fro{1100};
    const detail::Int to{2233};

    i = fro;
    auto rng = kv.Range('N', fro.String(), to.String());
    for (; rng.Valid();) {
      ASSERT_EQ(rng.Liter(), 'N');
      ASSERT_EQ(rng.Key(), i.String());
      ASSERT_EQ(rng.Value(), "set");

      i.Inc();
      rng.Next();
    }
    ASSERT_EQ(i.i, to.i);
    ASSERT_EQ(rng.Liter(), 0);
    ASSERT_EQ(rng.Key(), "");
    ASSERT_EQ(rng.Value(), "");
    ASSERT_FALSE(rng.Next());

    kv.Close();
    ASSERT_FALSE(kv.DB());
  }
}
