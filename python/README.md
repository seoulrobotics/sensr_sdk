# Python Guide

This Python SDK shows how to communicate with SENSR and parse the output in Python. There are some sample scripts, which are standalone programs as well as used for developers to refer to the code usages. 

## Installation

In order to run the sample code properly, the system needs to be configured first by running the command below. 

```bash
./configure.sh
```

## Execution

Please follow the installation section beforehand. 

- Dump the output messages received from SENSR
  ```bash
  python3 message_receiver.py
  # Run SENSR in another terminal
  ```
- Parse a binary output file
  ```bash
  python3 parse_output.py sample_output/frame_00006.bin
  ```

For details about how to use the SDK in more complex projects, please refer to [devtips.md](./devtips.md)

