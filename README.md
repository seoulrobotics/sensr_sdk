# Seoul Robotics SENSR SDK

## Installation

### Dependencies

SENSR SDK depends on [Websocket](https://tools.ietf.org/html/rfc6455) and [Protobuf](https://developers.google.com/protocol-buffers/) (tested with 3.12.0).

ImGui sample code also depends on [GLFW](https://www.glfw.org/) (optional).

You can install prebuilt binaries or build from source as following.

#### Build Dependencies from Source

Make sure you have the listed dependencies installed:

```bash
sudo apt-get install autoconf automake libtool curl make g++ unzip git cmake
```

Protobuf (3.11.4) from source:

```bash
curl -OL https://github.com/protocolbuffers/protobuf/releases/download/v3.11.4/protobuf-cpp-3.11.4.zip
unzip protobuf-cpp-3.11.4.zip
rm protobuf-cpp-3.11.4.zip
cd protobuf-3.11.4
./autogen.sh
./configure CXXFLAGS=-fPIC
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
#### How to run
console_output_sample takes below arguments.
arg[1]     : ip address of SENSR program running machine
arg[2]-[5] : sample types. (zone object point time)
```
e.g.)
$ ./build_console/console_output_sample localhost zone
```
### ImGui

```bash
mkdir build_imgui
cd build_imgui
cmake ../samples/imgui_display
make
```

Or you can simply run build_imgui_sample.sh from the root source folder.
#### How to run
imgui_display_sample takes below arguments.
arg[1]     : ip address of SENSR program running machine
```
e.g.)
$ ./build_imgui/imgui_display_sample localhost
```
