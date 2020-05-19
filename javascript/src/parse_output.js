'use strict';

// Filesystem
const fs = require('fs');

// Proto
const label_msg = require('./../js_proto/labels_pb.js')
const output_msg = require('./../js_proto/output_pb.js')

// Formatting
const print_utils = require('./print_utils')

module.exports = {
  parseOutputFile
}

function parseOutputFile(filename) {

  const bytes = fs.readFileSync(filename);
  const output = output_msg.OutputMessage.deserializeBinary(bytes);

  const objects = output.getObjectsList();

  objects.forEach(function (obj, key) {
    console.log("------------------------------");
    print_utils.printObject(obj);
  });
}
