#!/bin/bash

SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
OUT_DIR="$SRC_DIR"
PROTO_DIR="$SRC_DIR/../proto"

pip3 install -r $SRC_DIR/requirements.txt

PROTOC="$(command -v protoc)"
if [ -z "$PROTOC" ]
then
  echo protobuf-compiler is not found! It is being downloaded...

  PB_REL="https://github.com/protocolbuffers/protobuf/releases"
  PB_VER="3.11.4"
  PB_ZIP="protoc-$PB_VER-linux-x86_64.zip"
  PROTOC_DIR="$SRC_DIR/protoc-$PB_VER"
  PROTOC="$PROTOC_DIR/bin/protoc"

  curl -LO $PB_REL/download/v$PB_VER/$PB_ZIP
  unzip $PB_ZIP -d $PROTOC_DIR
  rm $PB_ZIP
fi

if [ ! -d $OUT_DIR ]; then
  mkdir $OUT_DIR
fi

echo Found protobuf-compiler at $PROTOC
$PROTOC -I=$PROTO_DIR --python_out=$OUT_DIR $PROTO_DIR/*.proto