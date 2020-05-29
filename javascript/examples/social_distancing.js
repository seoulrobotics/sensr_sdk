'use strict';

// Includes
// const parsing = require('./../javascript_sdk/src/parse_output');
const math = require('mathjs');
const path = require('path');
const yargs = require('yargs');

// Include SENSR SDK
const sensr = require('sensr_js_sdk')

// Constants
const BREACH_DISTANCE = 2.0;


main();

function main() {
  const argv = fetchArgs();
  runSocialDistancing(argv.hostname)
}

function fetchArgs() {
  return yargs
      .command('run', 'Run social distancing example.', {
        address: {
          description: 'Host IP address or hostname which runs SENSR',
          alias: 'a',
          type: 'string',
        },
      })
      .check(function(argv) {
        if (argv._.includes('run') && argv.address != undefined) {
          return true;
        } else {
          return false;
        }
      })
      .help()
      .alias('help', 'h')
      .argv;
}

async function runSocialDistancing(hostname) {

  const messageReceiver = new sensr.receiver.MessageReceiver();
  const TIMEOUT = 1000;

  try {
    messageReceiver.connect(hostname);
    while (true) {
      const msg = await messageReceiver.receive();
      messageReceiver.setTimeout(TIMEOUT);

      const output = sensr.parsing.deserializeBinary(msg)
      const [breachedObjectIds, nonbreachedObjects] = processOutput(output);
      displayResult(breachedObjectIds);
    }
    messageReceiver.disconnect();
  } catch (err) {
    console.log(err);
    console.log(`No message received for ${TIMEOUT} ms, ` +
        'stopping collection of messages.');
  }
}

function processOutput(output) {
  const breachedObjectIds = new Set();
  const objects = output.getObjectsList();

  for (let i = 0; i < objects.length; ++i) {
    const object1 = objects[i];
    if (object1.getLabel() != sensr.parsing.labelMsg.LabelType.PEDESTRIAN) {
      continue;
    }
    for (let j = i+1; j < objects.length; ++j) {
      const object2 = objects[j];
      if (object2.getLabel() != sensr.parsing.labelMsg.LabelType.PEDESTRIAN) {
        continue;
      }
      if (compareObjects(object1, object2)) {
        breachedObjectIds.add(object1.getId());
        breachedObjectIds.add(object2.getId());
      }
    }
  }

  let breachedObjects = []
  let nonbreachedObjects = []
  objects.forEach(function(obj) {
    if (obj.getLabel() != sensr.parsing.labelMsg.LabelType.PEDESTRIAN) {
      return;
    }

    if (breachedObjectIds.has(obj.getId())) {
      breachedObjects.push(obj);
    }
    else {
      nonbreachedObjects.push(obj);
    }
  });

  return [breachedObjects, nonbreachedObjects];
}

function compareObjects(obj1, obj2) {
  const pos1 = obj1.getBbox().getPosition();
  const pos2 = obj2.getBbox().getPosition();
  const dist = math.distance(
      [pos1.getX(), pos1.getY()],
      [pos2.getX(), pos2.getY()]);
  return dist < BREACH_DISTANCE;
}

function displayResult(breachedObjects) {
  // eslint-disable-next-line max-len
  console.log('The following objects have breached the social distancing protocols.');
  breachedObjects.forEach(function(obj) {
    const pos = obj.getBbox().getPosition();
    console.log(`id: ${obj.getId()}, position: (${pos.getX().toFixed(3)}, ${pos.getY().toFixed(3)})`);
  });
}
