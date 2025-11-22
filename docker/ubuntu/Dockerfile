# Commands:
# [build] docker build -t ghr-chess-engine-ubuntu . -f docker/ubuntu/Dockerfile
# [run] docker run -it --rm ghr-chess-engine-ubuntu
# [configure cmake] cmake --preset=clang_release
# [build cmake] cmake --build --preset=clang_release

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV VCPKG_ROOT=/opt/vcpkg
ENV VCPKG_FEATURE_FLAGS=manifest,binarycaching
ENV VCPKG_BINARY_SOURCES="clear;x-gha,readwrite"
ENV PATH="${VCPKG_ROOT}:${PATH}"

RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    lldb \
    lld \
    gcc \
    g++ \
    git \
    curl \
    zip \
    unzip \
    pkg-config \
    ninja-build \
    cmake \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

RUN clang++ --version && g++ --version

RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT}
RUN ${VCPKG_ROOT}/bootstrap-vcpkg.sh

WORKDIR /workspace
