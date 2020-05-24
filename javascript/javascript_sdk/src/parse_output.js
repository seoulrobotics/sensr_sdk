'use strict';

// Filesystem
const fs = require('fs');
const path = require('path');

// Proto
const labelMsg = require('./../js_proto/labels_pb.js');
const outputMsg = require('./../js_proto/output_pb.js');

// Formatting
const printUtils = require('./print_utils');

module.exports = {
  mkdir,
  getFileList,
  getOutput,
  parseOutputFile,
  deserializeBinary,
  exportToBinary,
  formatFilename,
  labelMsg,
  outputMsg,
  printUtils,
};

function mkdir(dir) {
  if (!fs.existsSync(dir)) {
    fs.mkdirSync(dir);
  }
}

function getFileList(dir) {
  return fs.readdirSync(dir);
}

function getOutput(filename) {
  try {
    const bytes = fs.readFileSync(filename);
    const output = outputMsg.OutputMessage.deserializeBinary(bytes);
    return output;
  } catch (err) {
    console.log(err);
  }
}

function parseOutputFile(filename) {
  const output = getOutput(filename);

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


function exportToBinary(bytes, filename) {
  try {
    fs.writeFileSync(filename, bytes);
  } catch (err) {
    console.log(err);
  }
}

function formatFilename(dir, fileId) {
  let fname = padLeft(String(fileId), '000000') + '.bin';
  fname = path.join(dir, fname);
  return fname;
}

