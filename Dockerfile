FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    pkg-config \
    libsdl2-dev \
    libsdl2-image-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

CMD ["bash", "-c", "mkdir -p release && cd release && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j$(nproc)"]
