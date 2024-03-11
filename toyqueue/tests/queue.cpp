#include <gtest/gtest.h>

#include <array>
#include <boost/endian/conversion.hpp>
#include <thread>
#include <toyqueue/queue.hpp>

namespace {

toyqueue::Record fromUint64(uint64_t x) {
  toyqueue::Record record(8);
  boost::endian::endian_store<uint64_t, 8, boost::endian::order::little>(
      record.data(), x);
  return record;
}

uint64_t fromRecord(const toyqueue::Record& record) {
  assert(record.size() == 8);
  return boost::endian::endian_load<uint64_t, 8, boost::endian::order::little>(
      record.data());
}

}  // namespace

TEST(TestToyQueue, SimpleNonBlocking) {
  constexpr size_t N = 1 << 10;
  constexpr size_t K = 1 << 4;

  toyqueue::RecordQueue queue{N};

  for (size_t i = 0; i < N; i += K) {
    toyqueue::Records records;
    records.reserve(K);
    for (uint64_t record = i; record < i + K; ++record) {
      records.push_back(fromUint64(record));
    }
    ASSERT_EQ(queue.Drain(std::move(records)), toyqueue::NoError);
  }
  ASSERT_EQ(queue.Drain({fromUint64(1u)}), toyqueue::ErrWouldBlock);

  const auto [all, err] = queue.Feed();
  ASSERT_EQ(err, toyqueue::NoError);
  ASSERT_EQ(all.size(), N);
  for (uint64_t i = 0; i < N; ++i) {
    ASSERT_EQ(fromRecord(all.at(i)), i);
  }

  ASSERT_EQ(queue.Feed().second, toyqueue::ErrWouldBlock);
  ASSERT_EQ(queue.Close(), toyqueue::NoError);
  ASSERT_EQ(queue.Drain({fromUint64(1u)}), toyqueue::ErrClosed);
  ASSERT_EQ(queue.Feed().second, toyqueue::ErrClosed);
}

TEST(TestToyQueue, SimpleBlocking) {
  constexpr size_t N = 1 << 10;
  constexpr size_t K = 1 << 4;

  auto orig = std::make_shared<toyqueue::RecordQueue>(N);
  auto queue = orig->Blocking();

  for (size_t i = 0; i < N; i += K) {
    toyqueue::Records records;
    records.reserve(K);
    for (uint64_t record = i; record < i + K; ++record) {
      records.push_back(fromUint64(record));
    }
    ASSERT_EQ(queue->Drain(std::move(records)), toyqueue::NoError);
  }

  const auto [all, err] = queue->Feed();
  ASSERT_EQ(err, toyqueue::NoError);
  ASSERT_EQ(all.size(), N);
  for (uint64_t i = 0; i < N; ++i) {
    ASSERT_EQ(fromRecord(all.at(i)), i);
  }
}

TEST(TestToyQueue, Drain) {
  constexpr size_t N = 1 << 10;
  constexpr size_t K = 1 << 4;

  auto orig = std::make_shared<toyqueue::RecordQueue>(N);
  auto queue = orig->Blocking();

  std::vector<std::thread> threads;
  for (size_t k = 0; k < K; ++k) {
    threads.emplace_back([queue, k, N] {
      const uint64_t i = (k << 32);
      for (uint64_t n = 0; n < N; ++n) {
        uint64_t cur = i | n;
        auto bytes = fromUint64(cur);
        ASSERT_EQ(queue->Drain({std::move(bytes)}), toyqueue::NoError);
      }
    });
  }

  std::array<uint32_t, K> check{};
  for (size_t i = 0; i < N * K;) {
    const auto [nums, err] = queue->Feed();
    ASSERT_EQ(err, toyqueue::NoError);
    for (const auto& num : nums) {
      ASSERT_EQ(num.size(), 8u);

      const uint64_t j = fromRecord(num);
      const uint32_t k = (j >> 32);
      const uint32_t n = (j & 0xffffffff);
      ASSERT_EQ(check.at(k), n);
      check.at(k) = n + 1;
      ++i;
    }
  }

  for (auto& thread : threads) {
    thread.join();
  }
}
