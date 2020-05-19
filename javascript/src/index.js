'use strict';

// Filesystem
var fs = require('fs');

// Proto
var label_msg = require('./../js_proto/labels_pb.js')
var output_msg = require('./../js_proto/output_pb.js')

// Formatting
var print_utils = require('./print_utils')



main()


function main() {
  
  let args = process.argv.slice(2);
  if (args === undefined ||  args.length == 0) {
    console.log("No binary file specified.");
  } else if (args.length > 1) {
    console.log("Wrong number of command line arguements.");
  } else {

    const filename = args[0];
    parseOutputFile(filename);

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

