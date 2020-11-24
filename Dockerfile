FROM ubuntu:focal

RUN apt-get update && apt-get install -y --no-install-recommends \
    cmake \
    freeglut3-dev \
    g++ \
    libglew-dev \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /root/hust_map

COPY build/textures build/textures
COPY src src
COPY CMakeLists.txt .

RUN cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make

RUN rm -rf src
RUN rm CMakeLists.txt

CMD ["./build/hust_map"]
