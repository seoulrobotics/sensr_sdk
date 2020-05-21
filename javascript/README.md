## Javascript Guide

Interfacing with SENSR output using javascript.


## Installation

```bash
sudo apt install nodejs
cd javascript/javascript_sdk
npm install
```

## Execution

- Dump the output messages received from SENSR
  ```bash
  npm start receive --output_dir sample_output
  # Run SENSR in another terminal
  ```

- Parse a binary output file, for example
  ```bash
  npm start parse -f sample_output/1234.bin
  ```

For more information, see
```bash
npm start --help
# or
npm start parse --help
# or
npm start receive --help
```

## Running the example
Make sure to first install the Javascript SDK as detailed above.
To run the example we first need to set up the example project:

```bash
cd javascript/example
npm install
# Note: The example depends on the sensr_js_sdk package which is a local module
# If your project is in another folder than the example you will need to do
# 'npm install ../relative/path/to/sensr_js_sdk'
# to install the dependency
```

Running
```bash
#TODO
```