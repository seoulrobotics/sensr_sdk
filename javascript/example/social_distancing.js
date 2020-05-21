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
// sensr.main();

function main() {
  const argv = fetchArgs();
  runSocialDistancing(argv.input_dir)
}


function fetchArgs() {
  return yargs
      .command('run', 'Run social distancing example.', {
        input_dir: {
          description: 'Directory of binary files to be read',
          alias: 'i',
          type: 'string',
        },
      })
      .check(function(argv) {
        if (argv._.includes('run') && argv.input_dir != undefined) {
          return true;
        } else {
          return false;
        }
      })
      .help()
      .alias('help', 'h')
      .argv;
}



function runSocialDistancing(inputDir) {
  console.log('Running social distancing example...');

  const fileList = sensr.parsing.getFileList(inputDir);
  fileList.forEach(function(file) {
    const absPath = path.join(inputDir, file);
    const output = sensr.parsing.getOutput(absPath);
    console.log(`Processing ${absPath}`);

    const [breachedObjects, nonBreachedObjects] = processOutput(output);

    displayResult(breachedObjects, nonBreachedObjects);
  });

  console.log('Done');
}


function processOutput(output) {
  const breachedObjects = new Set();
  const nonBreachedObjects = new Set();
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
        breachedObjects.add(object1.getId());
        breachedObjects.add(object2.getId());
      }
    }
  }
  objects.forEach(function(obj) {
    if (obj.getLabel() != sensr.parsing.labelMsg.LabelType.PEDESTRIAN) {
      return;
    }
    if (!breachedObjects.has(obj.getId())) {
      nonBreachedObjects.add(obj.getId());
    }
  });

  return [breachedObjects, nonBreachedObjects];
}

function compareObjects(obj1, obj2) {
  const pos1 = obj1.getBbox().getPosition();
  const pos2 = obj2.getBbox().getPosition();
  const dist = math.distance(
      [pos1.getX(), pos1.getY()],
      [pos2.getX(), pos2.getY()]);
  return dist < BREACH_DISTANCE;
}

function displayResult(breachedObjects, nonBreachedObjects) {
  // eslint-disable-next-line max-len
  console.log('The following objects have breached the social distancing protocols.');
  breachedObjects.forEach(function(obj) {
    console.log(obj);
  });
}
