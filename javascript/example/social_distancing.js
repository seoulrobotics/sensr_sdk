'use strict';

module.exports = {
  runSocialDistancing,
};

// Parsing
const parsing = require('./../src/parse_output');

// Math
const math = require('mathjs');

// Constants
const BREACH_DISTANCE = 2.0;


function runSocialDistancing(inputDir) {
  console.log('Running social distancing example...');

  const fileList = parsing.getFileList(inputDir);
  fileList.forEach(function(file) {
    const absPath = `${inputDir}/${file}`;
    const output = parsing.getOutput(absPath);
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
    // Continue if not ped TODO
    const object1 = objects[i];
    for (let j = i+1; j < objects.length; ++j) {
      // Continue if not ped TODO
      const object2 = objects[j];
      if (compareObjects(object1, object2)) {
        breachedObjects.add(object1.getId());
        breachedObjects.add(object2.getId());
      }
    }
  }
  objects.forEach(function(obj) {
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
