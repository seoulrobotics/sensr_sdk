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

### Run the Xavier DIO control code (code c)
```bash
cd samples/xavier_dio_control
sudo su
make
./xavier_dio -set 0 1
  #This example : using DO0 and voltage true(high)
  #-set : output -get : input 
```

### Run the DIO with zone & velocity
```bash
mkdir rco_dio_zone_velocity
cd rco_dio_zone_velocity
cmake ../samples/rco_dio_zone_velocity
make
```
Or you can simply run rco_dio_zone_velocity.sh from the root source folder.

modify the zone_info(zone,pin,velocity).txt file in the root source folder.
write the zone information

zone_id is start number 0
pin_number is start number 1 and finish number 4

example
1 1 2 (zone_id, pin_number, velocity(m/s))

if you run the program
1. run the SENSR program
2. sudo su
3. enter the rco_dio_zone_velocity in the root source folder
4. ./rco_dio_zone_velocity

