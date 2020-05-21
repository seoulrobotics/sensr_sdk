## Javascript Guide

Interfacing with SENSR output using javascript.


## Installation

```bash
cd javascript
npm install
./gen_proto.sh
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
TODO
