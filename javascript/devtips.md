# Development Tips

Here are some code snippets to understand how to work with the SENSR Javascript SDK.

## Communicate with SENSR
TODO
<!-- ```javascript
'use strict';

// Include SENSR SDK
const sensr = require('sensr_js_sdk')


``` -->

### Receive output messages from SENSR

### Convert an output message to a protobuf object

# Specific examples

## Social distancing example
The social distancing example is located in [examples/social_distancing.js](./examples/social_distancing.js)

Make sure all dependencies are installed first:

```bash
cd javascript/example
npm install
```

Then we can run the example. The input directory should only have SENSR binary output files.
This is a very basic example that only prints out the ID of each object that breaches the social distancing radius (2 meters).
```bash
node social_distancing.js --input_dir my_folder
```
