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
  deserializeBinary,
  exportToBinary,
};

function parseOutputFile(filename) {
  const bytes = fs.readFileSync(filename);
  const output = outputMsg.OutputMessage.deserializeBinary(bytes);

  const objects = output.getObjectsList();

  objects.forEach(function(obj) {
    console.log('------------------------------');
    printUtils.printObject(obj);
  });
}


function deserializeBinary(bytes) {
  const output = outputMsg.OutputMessage.deserializeBinary(bytes);
  return output;
}

function padLeft(string, paddingValue) {
  return String(paddingValue + string).slice(-paddingValue.length);
}

function exportToBinary(outputs, outputDir) {
  try {
    if (!fs.existsSync(outputDir)) {
      fs.mkdirSync(outputDir);
    }

    outputs.forEach(function(output, key) {
      const bytes = output.serializeBinary();
      let fname = padLeft(String(key), '000000');
      fname = `${outputDir}/${fname}.bin`;
      fs.writeFileSync(fname, bytes);
    });
  } catch (err) {
    console.log(err);
  }
}
