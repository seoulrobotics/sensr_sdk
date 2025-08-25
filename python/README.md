# Seoul Robotics SENSR SDK - Python

This Python SDK shows how to communicate with SENSR and parse the output in Python. There are some sample scripts, which are standalone programs that can be used as a reference.

## Quick start

1. Prerequisites

    - Network connection to SENSR master (`>=2.4.0`) (Assuming already running at the same computer in runtime mode)
    - Linux `x86_64`/`aarch_64` or macOS `x86_64`/`arm64`
    - Python`>=3.6`
    - python packages in [requirements.txt](./requirements.txt)

2. Clone repository with submodules, build protobuf for python

    ```bash
    git clone https://github.com/seoulrobotics/sensr_sdk --recursive
    cd sensr_sdk/python
    ```

3. Build protobuf for python using `configure.sh`

    > **NOTE**: If protoc is not available, this script will automatically download and run protobuf-compiler from [github.com](https://github.com/protocolbuffers/protobuf/releases)

    ```bash
    ./configure.sh
    ```

4. Install python3 pip packages

    ```bash
    python3 -m pip install --upgrade pip # Optional, recommended
    python3 -m pip install -r requirements.txt
    ```

5. Run example script

    > **NOTE**:
    >
    > 1. If SENSR is running in the remote machine, replace `localhost` to other IP address or hostname accordingly.
    >
    > 2. Replace `object` to other value to print other types, one of: `zone`, `point`, `object`, `health`, `time`

    ```bash
    python3 ./console_output.py --address localhost --example_type object
    ```

## Running examples

### Print output to console with SSL

> **NOTE**: This example requires pair of SSL certificates used in the SENSR. Currently, this example does not support SSL verification with Root Certifacte from OS.

1. Launch SENSR with SSL enabled
2. Copy `sensr-ca.crt` to `$HOME/seoulrobotics/keys/sensr-ca.crt`
3. Open another terminal and type the following

  ```bash
  python3 console_output_secure.py --address localhost --example_type zone
  ```

### Recording and Playing SDK outputs

- Saving output to file

    ```bash
    python3 save_load_output.py --mode save --address localhost --dir {output_path}
    ```

- Loading output from file

  ```bash
  python3 save_load_output.py --mode load --dir {output_path}
  ```
