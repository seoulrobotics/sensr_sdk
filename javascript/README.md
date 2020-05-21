# Javascript Guide

This guide will show you how to use the SENSR output with Javascript using our SDK. 
We use NodeJS for these examples so make sure you have the dev environment set up.

```bash
sudo apt-get update
sudo apt-get install nodejs
sudo apt-get install npm
```
If you have ROS installed you might face some problems with installing npm, please refer to [troubleshooting.md](./troubleshooting.md).

## Setting up your own project
To use the SENSR SDK, you first need to create your own project. 

Create an empty directory for your project and navigate to it. This can be located wherever you want. To initialize your project run:
```bash 
mkdir my_project
cd my_project
npm init # Follow the instructions, the defaults should be fine.

# Add sensr_js_sdk as dependency. This is added as a local module,
# hence you need to specify the relative (or absolute) path to 
# sensr_js_sdk
npm install ../relative/path/to/sensr_sdk/javascript/sensr_js_sdk 
```
You should now have a main file (called `index.js` by default) that you can run using `npm start` or `node index.js`. 

## Basic usage
The  `javascript/examples` folder includes some example projects. A good starting point is `javascript/examples/run_sdk.js`.

First you need to install the dependencies for the examples. This includes sensr_js_sdk.
```bash
cd sensr_sdk/javascript/examples
npm install 
```

The minimal example in `javascript/examples/run_sdk.js` is as follows:
```javascript
'use strict';

// Include SENSR SDK
const sensr = require('sensr_js_sdk')

sensr.run()
```

The are some command line options that you can pass to access different functionality.
- Dump the output messages received from SENSR
  ```bash
  node run_sdk.js dump --output_dir sample_output
  # Run SENSR in another terminal
  ```

- Parse a binary output file, for example
  ```bash
  node run_sdk.js parse -f sample_output/1234.bin
  ```

For more information, see
```bash
node run_sdk.js --help
# or
node run_sdk.js parse --help
# or
node run_sdk.js dump --help
```





## Running the Social Distancing Example 
Make sure all dependencies are installed first:

```bash
cd javascript/example
npm install
```

Then we can run the example. The input directory should have SENSR binary output files.
This is a very basic example that only prints out the ID of each object that breaches the social distancing radius (2m).
```bash
node social_distancing.js --input_dir test_folder
```