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
  mkdir,
  formatFilename,
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

function mkdir(dir) {
  if (!fs.existsSync(dir)) {
    fs.mkdirSync(dir);
  }
}

function exportToBinary(bytes, filename) {
  try {
    fs.writeFileSync(filename, bytes);
  } catch (err) {
    console.log(err);
  }
}

function formatFilename(dir, fileId) {
  let fname = padLeft(String(fileId), '000000');
  fname = `${dir}/${fname}.bin`;
  return fname;
}

