# Python Guide

This Python SDK shows how to communicate with SENSR and parse the output in Python. There are some sample scripts, which are standalone programs that can be used as a reference. 

## Installation

In order to run the sample code properly, the system needs to be configured first by running the command below. 

```bash
./configure.sh
```

## Execution

After installation, you can run the example.

### Print output to console

- Launch SENSR 
- Open another terminal and type the following
  ```bash
  python3 console_output.py --address localhost --example_type zone
  # You need to put the correct IP address instead of localhost if SENSR is running on a different machine.
  # You can replace "zone" with "point", "object", "health", or "time" to run the other included examples.
  ```

### Save output to files

- Launch SENSR 
- Open another terminal and type the following to save output
  ```bash
  python3 save_load_output.py --mode save --address localhost --dir <folder_path_to_save>
  # You need to put the correct IP address instead of localhost if SENSR is running on a different machine.
  ```

### Load output from files

- Open terminal and type the following to load output
  ```bash
  python3 save_load_output.py --mode load --dir <folder_path_to_load>
  ```