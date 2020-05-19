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
  parseCmdArgs();
}


function parseCmdArgs() {
  const argv = yargs
    .command('parse', 'Parse from binary file.', {
      filename: {
        description: "File to be parsed",
        alias: 'f',
        type: 'string',
      }
    })
    .command('receive', 'Receive messages from SENSR.', {
      todo: {
        description: "todo",
        alias: 't',
        type: 'boolean',
      }
    })
    .check(function (argv) {
      if (argv._.includes('parse') && argv.filename != undefined) {
        return true;
      } else if (argv._.includes('receive')) {
        return true;
      } else {
        return false;
      }
    })
    .help()
    .alias('help', 'h')
    .argv;

  if (argv._.includes('parse')) {
    parseOutputFile(argv.filename)
  }

  if (argv._.includes('receive')) {
    console.log("RECEIVING... rip this doesn't actually do anything yet.")
  }
  
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

