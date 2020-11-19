# Seoul Robotics SENSR SDK

## Installation

### Dependencies

SENSR SDK depends on [ZeroMQ](http://zeromq.org/intro:get-the-software) (tested with 4.3.1) and [Protobuf](https://developers.google.com/protocol-buffers/) (tested with 3.6.0).

ImGui sample code also depends on [GLFW](https://www.glfw.org/) (optional).

You can install prebuilt binaries or build from source as following.

#### Build Dependencies from Source

ZeroMQ (4.3.1) from source:

```bash
git clone https://github.com/zeromq/libzmq
cd libzmq
git checkout v4.3.1
mkdir build && cd build && cmake .. && make -j16 && sudo make install
sudo ldconfig
cd ../../ && rm -rf libzmq
```

Protobuf (3.6.0) from source:

```bash
curl -OL https://github.com/protocolbuffers/protobuf/releases/download/v3.6.0/protobuf-cpp-3.6.0.zip
unzip protobuf-cpp-3.6.0.zip
rm protobuf-cpp-3.6.0.zip
cd protobuf-3.6.0
./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig
```

### Build SENSR SDK

Update submodule:

```bash
git submodule update --init
```

Make sure you have the listed dependencies installed:

```bash
sudo apt-get install autoconf automake libtool curl make g++ unzip
```

Build SENSR SDK:

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

Or you can simply run build_sdk.sh from the root source folder.

## Samples

You can build the two samples separately with their own cmake config:

### Console

```bash
mkdir build_console
cd build_console
cmake ../samples/console_output
make
```

Or you can simply run build_console_output.sh from the root source folder.

### Run the code

```bash
cd build_console
sudo su
./console_output_sample
```

This code controls GPO1
When an object is recognized by the lidar, voltage is applied to GPO1.

### Run the DIO control code (code c)
```bash
cd samples/rco_dio_control/code_c
sudo su
make
./RCO_DIO_control -O 1 -t
  #This example : using DO1 and voltage true(high)
  #-O : output -I : input -h : help 
```
