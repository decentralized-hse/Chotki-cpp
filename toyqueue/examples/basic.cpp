#include <boost/endian.hpp>
#include <cassert>
#include <iostream>
#include <toyqueue/queue.hpp>

namespace {

toyqueue::Record fromUint64(uint64_t x) {
  toyqueue::Record record(8);
  boost::endian::endian_store<uint64_t, 8, boost::endian::order::little>(
      record.data(), x);
  return record;
}

uint64_t fromRecord(const toyqueue::Record& record) {
  return boost::endian::endian_load<uint64_t, 8, boost::endian::order::little>(
      record.data());
}

}  // namespace

int main() {
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
    assert(queue->Drain(std::move(records)) == toyqueue::NoError);
  }
  std::cout << "Drained " << N << " elements" << std::endl;

  const auto [all, err] = queue->Feed();
  assert(err == toyqueue::NoError);
  assert(all.size() == N);
  std::cout << "Feeded " << all.size() << " elements" << std::endl;
  for (uint64_t i = 0; i < N; ++i) {
    assert(fromRecord(all.at(i)) == i);
  }
}
