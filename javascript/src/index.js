'use strict';

// Filesystem
const fs = require('fs');

// Proto
const label_msg = require('./../js_proto/labels_pb.js')
const output_msg = require('./../js_proto/output_pb.js')

// Formatting
const print_utils = require('./print_utils')

// Arg parsing
const yargs = require('yargs')


main()



function main() {
  const filename = parseCmdArgs();
  parseOutputFile(filename);
}


function parseCmdArgs() {
  const argv = yargs
    .option('filename', {
      description: "File to be parsed",
      alias: 'f',
      type: 'string',
    })
    .help()
    .alias('help', 'h')
    .argv;
  
  if (argv.filename == undefined) {
    throw "No filename specified. See --help."
  }

  return argv.filename;
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

