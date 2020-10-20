#!/bin/bash

# function definitions
function install_protoc_web_gen_plugin() {
    # install protoc-gen-grpc-web if it does not exist.
    PROTOC_WEB_GEN_PLUGIN=/usr/local/bin/protoc-gen-grpc-web
    if [ ! -f "$PROTOC_WEB_GEN_PLUGIN" ]; then
        if ! command -v wget &> /dev/null
        then
            sudo apt-get install wget
        fi
        wget -O protoc-gen-grpc-web -q --show-progress https://github.com/grpc/grpc-web/releases/download/1.2.1/protoc-gen-grpc-web-1.2.1-linux-x86_64
        sudo mv protoc-gen-grpc-web /usr/local/bin/protoc-gen-grpc-web
        chmod +x /usr/local/bin/protoc-gen-grpc-web
    fi
}

function make_cpp_sdk() {
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
}

function make_javascript_sdk() {
    javascript/javascript_sdk/gen_proto.sh
}

# Main function
if [ $# -ne 1 ]; then
    echo "Usage: $0 generating-language"
    echo "Support languages: cpp, javascript, python"
    exit -1
else
    install_protoc_web_gen_plugin
    if [ "$1" == "cpp" ]; then
        make_cpp_sdk
    elif [ "$1" == "javascript" ]; then
        make_javascript_sdk
    elif [ "$1" == "python" ]; then
        echo "Not supported yet."
    else
        echo "$1 is not supported language."
        exit -1
    fi
    echo "Build $1 sdk complete."
fi
exit 0
