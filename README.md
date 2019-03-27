![SDK Logo](https://gitlab.com/stefano6/sdk_public_test/blob/master/images/Logo.png)
# Argos SDK

## Installation

### Dependencies

Argos SDK depends on [ZeroMQ](http://zeromq.org/intro:get-the-software) and [Protobuf](https://developers.google.com/protocol-buffers/).

Samples also depend on [GLFW](https://www.glfw.org/).

### From Source

Update submodule:

```bash
git submodule update --init
```

Make sure you have the listed dependencies installed:

```bash
sudo apt-get install autoconf automake libtool curl make g++ unzip
```

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
git clone https://github.com/protocolbuffers/protobuf.git
cd protobuf
git checkout v3.6.0
git submodule update --init --recursive
./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig
cd .. && rm -rf protobuf
```

Glfw (optional - only needed for samples).

```bash
sudo apt-get install libglfw3-dev
```

And finally:

```bash
mkdir build
cd build
cmake ..
make
make install
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

### ImGui

```bash
mkdir build_imgui
cd build_imgui
cmake ../samples/imgui_display
make
```

Or you can simply run build_imgui_sample.sh from the root source folder.
