'use strict';

// Filesystem
const fs = require('fs');

// Proto
const label_msg = require('./../js_proto/labels_pb.js')
const output_msg = require('./../js_proto/output_pb.js')

// Formatting
const print_utils = require('./print_utils')



main()


function main() {
  const filename = parseCmdArgs();
  parseOutputFile(filename);
}


function parseCmdArgs () {
  let args = process.argv.slice(2);
  if (args === undefined ||  args.length == 0) {
    throw "No binary file specified.";
  } else if (args.length > 1) {
    throw "Wrong number of command line arguements.";
  }

  const filename = args[0];
  return filename;
}

function parseOutputFile(filename) {

  const bytes = fs.readFileSync(filename);
  const output = output_msg.OutputMessage.deserializeBinary(bytes);

  const objects = output.getObjectsList();
  
  objects.forEach(function(obj, key) {
    console.log("------------------------------");
    print_utils.printObject(obj);
  });

  
  
}

