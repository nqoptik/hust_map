# Build stage
FROM ubuntu:focal AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    freeglut3-dev \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /root/hust_map

COPY build/textures build/textures
COPY src src
COPY CMakeLists.txt .

RUN cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make

# Production stage
FROM ubuntu:focal AS production

RUN apt-get update && apt-get install -y --no-install-recommends \
    freeglut3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /root/hust_map

COPY --from=build /root/hust_map/build build

CMD ["build/hust_map"]
