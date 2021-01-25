'use strict';

// Include SENSR SDK
const sensr = require('sensr_js_sdk');
const parsing = sensr.parsing;
const receiver = sensr.receiver;
const labelMsg = parsing.labelMsg;
const outputMsg = parsing.outputMsg;
const printObject = parsing.printUtils.printObject;

// Specify timeout, the receiver will shut down after this many
// milliseconds if no message is received from SENSR. The timeout
// is only set after the first message is received
const TIMEOUT = 1000;


run();

async function run() {
  // Receive messages from SENSR
  const messageReceiver = new receiver.MessageReceiver();
  
  messageReceiver.connect();
  try {
    while (true) {
      const msg = await messageReceiver.receive();
      // messageReceiver.setTimeout(TIMEOUT);
     
      processMessage(msg);
    }
  } catch (err) {
    console.log(`No message received for ${TIMEOUT} ms, ` +
        'stopping collection of messages.');
  }
  messageReceiver.disconnect();
}

async function processMessage(msg) {
  // Convert to protobuf object
  try {
    const output = outputMsg.OutputMessage.deserializeBinary(msg);
    
    // You can process the SENSR output inside the doStuff function.
    // For example, here we print information about the objects.
    // doStuff(output);
  } catch (err) {
    console.log(err);
    return;
  }
}

async function doStuff(output) {
  const objects = output.getObjectsList();
  objects.forEach((obj) => {
    console.log('------------------------------');
    printObject(obj);
  });
}
