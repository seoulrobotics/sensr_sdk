## Javascript Guide

TODO


## Installation

```bash
cd javascript
npm install
./gen_proto.sh
```

## Execution

- Dump the output messages received from SENSR (TODO make sure this is correct)
  ```bash
  npm start receive
  # Run SENSR in another terminal
  ```

- Parse a binary output file, for example
  ```bash
  npm start parse -f sample_output/1234.bin
  ```

For more information, see
```bash
npm start --help
```

