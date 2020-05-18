'use strict';

// Filesystem
var fs = require('fs');

// Proto
var label_msg = require('./../js_proto/labels_pb.js')
var output_msg = require('./../js_proto/output_pb.js')



main()


function main() {
  console.log('Hello SENSR Javascript!');
  
  //TODO
  let filename = "/home/seoulrobotics-thor/dev/sensr_sdk/javascript/sample_output/0000.bin";
  parseOutputFile(filename);



  console.log('DONE!');    
}


function labelToString(label) {
  // TODO
  return String(label);
}



function parseOutputFile(filename) {

  const bytes = fs.readFileSync(filename);

  let output = output_msg.OutputMessage.deserializeBinary(bytes);
  let a = output.getObjectsList();
  console.log(a);
  
  
}

