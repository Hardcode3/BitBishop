# Commands:
# [build] docker build -t bitbishop-ubuntu . -f docker/ubuntu/Dockerfile
# [run] docker run -it --rm bitbishop-ubuntu

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV VCPKG_ROOT=/opt/vcpkg
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
