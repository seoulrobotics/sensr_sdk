#!/usr/bin/env bash

# NOTE: Script template from https://betterdev.blog/minimal-safe-bash-script-template/

set -Eeuo pipefail
trap cleanup SIGINT SIGTERM ERR EXIT

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)

usage() {
  cat <<EOF
Usage: $(basename "${BASH_SOURCE[0]}") [-h] [-v]

Simple script to generate protobuf definitions in python for SENSR SDK
  - Downloads protoc from github.com if not available
  - Builds protobuf defintion for python

Available options:

-h, --help      Print this help and exit
-v, --verbose   Print script debug info
EOF
  exit
}

cleanup() {
  trap - SIGINT SIGTERM ERR EXIT
  # cleanup
}

msg() {
  echo >&2 -e "${1-}"
}

die() {
  local msg=$1
  local code=${2-1} # default exit status 1
  msg "$msg"
  exit "$code"
}

parse_params() {
  while :; do
    case "${1-}" in
    -h | --help) usage ;;
    -v | --verbose) set -x ;;
    -?*) die "Unknown option: $1" ;;
    *) break ;;
    esac
    shift
  done

  args=("$@")
  [[ ! ${#args[@]} -eq 0 ]] && die "Unexptected script argument: ${args[*]}\n\n$(usage)"

  return 0
}

parse_params "$@"


set_protoc_os_arch() {
  echo "Determining platform ..."
  case $(uname -s) in
    Linux) PB_OS="linux" ;;
    Darwin) PB_OS="osx" ;;
    *) die "  Unsupported os: $(uname -s)" ;;
  esac

  case $(uname -m) in
    x86_64) PB_ARCH="x86_64" ;;
    aarch64 | arm64) PB_ARCH="aarch_64" ;;
    *) die "  Unsupported architecture: $(uname -m)" ;;
  esac
}

print_protoc_status() {
  echo "  protoc found: $PROTOC"
  echo "  protoc version: $($PROTOC --version)"
}

download_protoc_locally() {
  PB_ZIP="protoc-$PB_VER-$PB_OS-$PB_ARCH.zip"

  if [ "$(unzip -t "$PB_PATH/$PB_ZIP" 2> /dev/null)" ]; then
    echo "  protoc release found, skipping download ..."
    echo "    $PB_PATH/$PB_ZIP"
  else
    echo "Downloading protoc release to local directory..."
    mkdir -p "$PB_PATH"
    curl -fSsL "https://github.com/protocolbuffers/protobuf/releases/download/v$PB_VER/$PB_ZIP" -o "$PB_PATH/$PB_ZIP"    
  fi
  unzip -oq "$PB_PATH/$PB_ZIP" -d "$PB_PATH"
}

build_proto() {
  echo "Generating protobuf definition for python ..."
  PROTO_OUT=$script_dir # NOTE: python files will be generated. e.g.) ./sensr_proto/custom_pb2.py
  PROTO_SRC="$script_dir/../proto"
  mkdir -p "$PROTO_OUT"
  $PROTOC -I="$PROTO_SRC" --python_out="$PROTO_OUT" "$PROTO_SRC"/*/*.proto
  echo "  protobuf files has generated to $PROTO_OUT"
}


PB_VER="3.12.0"
PB_PATH="$script_dir/protoc"
PROTOC="$PB_PATH/bin/protoc"

echo "Finding protoc ..."
if [ ! -x "$PROTOC" ]; then
  echo "  protoc found in local directory:"
  echo "  remove this path to use protoc from system PATH"
  echo "  $PROTOC"
elif [ ! -x "$(command -v protoc)" ]; then
  PROTOC=$(command -v protoc)
else
  set_protoc_os_arch
  download_protoc_locally
fi

print_protoc_status
build_proto
