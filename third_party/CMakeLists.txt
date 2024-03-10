include(FetchContent)

Log("Fetching dependencies")

Set(FETCHCONTENT_QUIET FALSE)

# Seastar
Log("Fetching Seastar")
FetchContent_Declare(
        seastar
        GIT_REPOSITORY  https://github.com/scylladb/seastar.git
        GIT_TAG         seastar-22.11.0
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(seastar)

# Rocksdb
Log("Fetching Rocksdb")
FetchContent_Declare(
        rocksdb
        GIT_REPOSITORY  https://github.com/facebook/rocksdb.git
        GIT_TAG         v8.11.3
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(rocksdb)
