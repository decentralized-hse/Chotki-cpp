include(FetchContent)

Log("Fetching dependencies")

# Seastar
Log("Fetching Seastar")
FetchContent_Declare(
        seastar
        GIT_REPOSITORY  https://github.com/scylladb/seastar.git
        GIT_TAG         seastar-22.11.0
)
FetchContent_MakeAvailable(seastar)

# Rocksdb
Log("Fetching Rocksdb")
FetchContent_Declare(
        rocksdb
        GIT_REPOSITORY  https://github.com/facebook/rocksdb.git
        GIT_TAG         v8.11.3
)
FetchContent_MakeAvailable(rocksdb)
