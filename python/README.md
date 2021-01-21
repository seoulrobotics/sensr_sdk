# Python Guide

This Python SDK shows how to communicate with SENSR and parse the output in Python. There are some sample scripts, which are standalone programs that can be used as a reference. 

## Installation

In order to run the sample code properly, the system needs to be configured first by running the command below. 

```bash
./configure.sh
```

## Execution

After installation if complete, you can run the example.

- Launch SENSR 
- Open another terminal and type the following
  ```bash
  python3 console_output.py --address localhost --example_type zone
  # You need to put the correct address instead of localhost if SENSR is running on a different machine.
  # You can replace "zone" with "point", "object", "health", or "time" to run the other included examples.
  ```
