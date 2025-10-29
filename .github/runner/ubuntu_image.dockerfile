# Commands:
# [build] docker build -t ghr-chess-engine-ubuntu . -f .github\runner\ubuntu_image.dockerfile
# [run] docker run -it --rm ghr-chess-engine-ubuntu
# [configure cmake] cmake --preset=clang_release
# [build cmake] cmake --build --preset=clang_release

FROM ubuntu:24.04

# [curl, zip]: dependencies for vcpkg
RUN apt-get update && apt-get install -y \
    curl zip \
    pkg-config \
    build-essential \
    git \
    cmake \
    clang \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Configure VCPKG
WORKDIR /
RUN git clone https://github.com/microsoft/vcpkg.git
WORKDIR /vcpkg
RUN ./bootstrap-vcpkg.sh -disableMetrics
ENV VCPKG_ROOT=/vcpkg
ENV PATH="${VCPKG_ROOT}:${PATH}"

WORKDIR /chess_engine
COPY . .

RUN ["/bin/bash"]
