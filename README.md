# Seoul Robotics SENSR SDK


## Dependencies

- [Websocket](https://tools.ietf.org/html/rfc6455)
- [Protobuf](https://developers.google.com/protocol-buffers/) (tested with `3.11.4`)

You can install prebuilt binaries or build from source as following.

### Protobuf

- Option 1) Download pre-built binary from release, install to system (**recommended**)

    ```bash
    sudo apt-get install -y curl
    # NOTE: Replace x86_64 with aarch_64 accordingly
    curl -fSsL https://github.com/protocolbuffers/protobuf/releases/download/v3.11.4/protoc-3.11.4-linux-x86_64.zip -o protoc-3.11.4-linux-x86_64.zip
    unzip protoc-3.11.4-linux-x86_64.zip -d /usr/local
    ```

- Option 2) Build protobuf from source on Ubuntu 18/20: (**advanced**)

    ```bash
    sudo apt-get install -y autoconf automake libtool curl make g++ unzip
    curl -fSsLO https://github.com/protocolbuffers/protobuf/releases/download/v3.11.4/protobuf-cpp-3.11.4.zip && unzip protobuf-cpp-3.11.4.zip && rm protobuf-cpp-3.11.4.zip
    cd protobuf-3.11.4
    ./autogen.sh && ./configure CXXFLAGS=-fPIC
    make -j`nproc` && make check
    sudo make install
    sudo ldconfig
    ```


### Build SENSR SDK

Update submodule:

```bash
git submodule update --init --recursive
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

#### How to run

console_output_sample takes below arguments.
arg[1]     : ip address of SENSR program running machine
arg[2]-[5] : sample types. (zone object point time)

```bash
./build_console/console_output_sample localhost zone
```

#### How to enable WSS

WSS can be used if you need secure output communication.
You can enable WSS by passing the certificate key path when you create a `Client` instance.

```cpp
sensr::Client client(address, "$HOME/seoulrobotics/keys/sensr-ca.crt");
```
