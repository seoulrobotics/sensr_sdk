FROM ubuntu:18.04

RUN apt-get update && apt-get install autoconf automake libtool curl make g++ unzip git cmake -y

RUN git clone https://github.com/zeromq/libzmq && \
    cd libzmq && \
    git checkout v4.3.1 && \
    mkdir build && cd build && cmake .. && make -j16 && make install && \
    ldconfig && \
    cd ../../ && rm -rf libzmq


RUN git clone https://github.com/protocolbuffers/protobuf.git && \
    cd protobuf && \
    git checkout v3.6.0 && \
    git submodule update --init --recursive && \
    ./autogen.sh && \
    ./configure && \
    make -j16 && \
    make check && \
    make install && \
    ldconfig && \
    cd .. && rm -rf protobuf

RUN apt-get install libglfw3-dev -y

RUN cd ..

RUN git clone https://gitlab.com/stefano6/sdk_public_test.git && \
    cd sdk_public_test && \
    git submodule init && \
    git submodule update --recursive && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install
