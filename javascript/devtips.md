# Development Tips

This page contains some code snippets to understand how to work with the SENSR Javascript SDK.
For a minimal working example, please refer to [example/basic_example.js](./examples/basic_example.js).


## Communicate with SENSR
Communication with SENSR is wrapped inside of the `MessageReceiver` class. To receive messages from SENSR, you can use the following code snippet:

```javascript
const sensr = require('sensr_js_sdk')

async function subscribeToSensrMessages() {

  const messageReceiver = new sensr.receiver.MessageReceiver();
  const TIMEOUT = 1000;

  try {
    messageReceiver.connect();
    while (true) {
      const msg = await messageReceiver.receive();
      messageReceiver.setTimeout(TIMEOUT);

      processMessage(msg);
    }
    messageReceiver.disconnect();
  } catch (err) {
    console.log(err);
    console.log(`No message received for ${TIMEOUT} ms, ` +
        'stopping collection of messages.');
  }
  
}

function processMessage(msg) {
  // You can process the message here any way you want
}
```
You can then implement whatever functionality you need inside the `processMessage` function. Note that we set the timeout to 1000 ms after receiving the first message in this code snippet so that the receiver shuts down if a new message is not received inside of that timeframe.

### Convert an output message to a protobuf object
To get the message from SENSR to a usable format, we need to convert it to a protobuf object. This can simply be done with the following code snippet:

```javascript
const sensr = require('sensr_js_sdk');
const outputMsg = sensr.parsing.outputMsg;

function convertToProtobuf(msg) {
  return  outputMsg.OutputMessage.deserializeBinary(msg);
}
```
For more info about how to use the protobuf objects, please refer to the [proto-files](./../proto) in this repository and the [official documentation](https://developers.google.com/protocol-buffers/docs/reference/javascript-generated).

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
