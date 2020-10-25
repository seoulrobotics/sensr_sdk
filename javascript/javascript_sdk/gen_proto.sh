#!/bin/bash

SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
OUT_DIR="${SRC_DIR}/js_proto"
PROTO_DIR="${SRC_DIR}/../../proto/"
PROTO_DIR_GOOGLE="${SRC_DIR}/../../proto/google/api"
PROTO_DIR2="${SRC_DIR}/../../proto/sensr_proto"
PROTO_DIRqqq="${SRC_DIR}/../../proto/argos_proto"
if [ ! -d $OUT_DIR ]; then
  mkdir $OUT_DIR
fi

protoc \
  --proto_path=$PROTO_DIR \
  --js_out=import_style=commonjs,binary:$OUT_DIR \
  --grpc-web_out=import_style=commonjs,mode=grpcwebtext:$OUT_DIR \
  $PROTO_DIRqqq/*.proto \
