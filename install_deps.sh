#!/bin/bash

if [ -f "/etc/os-release" ]; then
    . /etc/os-release
elif [ -f "/etc/arch-release" ]; then
    export ID=arch
else
    echo "/etc/os-release missing."
    exit 1
fi

debian_packages=(
    ninja-build
    ragel
    libhwloc-dev
    libnuma-dev
    libpciaccess-dev
    libcrypto++-dev
    libboost-all-dev
    libxml2-dev
    xfslibs-dev
    libgnutls28-dev
    liblz4-dev
    libsctp-dev
    liburing-dev
    gcc
    make
    meson
    python3
    python3-pyelftools
    systemtap-sdt-dev
    libtool
    cmake
    libyaml-cpp-dev
    libc-ares-dev
    stow
    g++
    libfmt-dev
    diffutils
    valgrind
    doxygen
    openssl
    pkg-config
    libprotobuf-dev
    protobuf-compiler
    libgflags-dev
)

fedora_packages=(
    "${redhat_packages[@]}"
    gcc-c++
    ninja-build
    ragel
    boost-devel
    fmt-devel
    libubsan
    libasan
    libatomic
    valgrind-devel
    libgflags-dev
)

case "$ID" in
    ubuntu|debian|pop)
        apt install -y "${debian_packages[@]}"
    ;;
    fedora)
        dnf install -y "${fedora_packages[@]}"
    ;;
    *)
        echo "Your system ($ID) is not supported by this script. Please install dependencies manually."
        exit 1
    ;;
esac
