'use strict';

// Filesystem
const fs = require('fs');

// Proto
const labelMsg = require('./../js_proto/labels_pb.js');
const outputMsg = require('./../js_proto/output_pb.js');

// Formatting
const printUtils = require('./print_utils');

module.exports = {
  parseOutputFile,
};

function parseOutputFile(filename) {
  const bytes = fs.readFileSync(filename);
  const output = outputMsg.OutputMessage.deserializeBinary(bytes);

  const objects = output.getObjectsList();

  objects.forEach(function(obj, key) {
    console.log('------------------------------');
    printUtils.printObject(obj);
  });
}
