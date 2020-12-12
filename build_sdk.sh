#!/bin/bash

# function definitions
function make_cpp_sdk() {
    rm -rf build
    rm -rf _install
    mkdir build
    cd build
    #cmake ..
    cmake -DCMAKE_INSTALL_PREFIX=../_install ..
    make -j6
    make install
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
