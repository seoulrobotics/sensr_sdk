#!/bin/bash

SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
OUT_DIR="${SRC_DIR}"
PROTO_DIR="${SRC_DIR}/../proto"

if [ ! -d $OUT_DIR ]; then
  mkdir $OUT_DIR
fi

protoc -I=$PROTO_DIR --python_out=$OUT_DIR $PROTO_DIR/*.proto