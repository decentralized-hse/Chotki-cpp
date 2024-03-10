#include <seastar/core/app-template.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/sleep.hh>
#include <toykv/toykv.hpp>

int main(int argc, char** argv) {
  seastar::app_template app;

  toykv::KeyValueStorage kv;
  assert(kv.Commit() == toykv::Error::NoError);

  app.run(argc, argv, [] {
    return seastar::make_ready_future();
  });
}
