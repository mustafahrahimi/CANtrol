FROM ubuntu:22.04

RUN apt-get update && apt-get install -y build-essential cmake

WORKDIR /app

COPY . .

RUN mkdir -p build && cd build && cmake .. && cmake --build .

CMD ["./build/CANtrol"]
