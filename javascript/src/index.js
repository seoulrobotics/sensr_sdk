'use strict';

// Filesystem
var fs = require('fs');

// Proto
var label_msg = require('./../js_proto/labels_pb.js')
var output_msg = require('./../js_proto/output_pb.js')

// Formatting
const num_decimal_places = 3;


main()


function main() {
  
  //TODO take filename as input
  let filename = "/home/seoulrobotics-thor/dev/sensr_sdk/javascript/sample_output/0000.bin";
  parseOutputFile(filename);

}


function labelToString(label) {
  switch(label) {
    case label_msg.LabelType.CAR:
      return "Car";
    case label_msg.LabelType.PEDESTRIAN:
      return "Ped";
    case label_msg.LabelType.CYCLIST:
      return "Cyclist";
    case label_msg.LabelType.MISC:
      return "Misc";
    default:
      return "Misc";
  }
}

function Format(float) {
  return float.toFixed(num_decimal_places);
}

function PrettyPrintFloat(float, name) {
  console.log(`${name}:\n\t${Format(float)}`);
}

function PrettyPrintVec2(vec, name) {
  const x = Format(vec.getX());
  const y = Format(vec.getY());
  console.log(`${name}:\n\t${x}\n\t${y}`);
}

function PrettyPrintVec3(vec, name) {
  const x = Format(vec.getX());
  const y = Format(vec.getY());
  const z = Format(vec.getZ());
  console.log(`${name}:\n\t${x}\n\t${y}\n\t${z}`);
}

function PrintBbox(bbox) {
  PrettyPrintVec2(bbox.getPosition(), "position");
  PrettyPrintVec3(bbox.getSize(), "size");
  PrettyPrintFloat(bbox.getYaw(), "yaw")
}

function PrintObject(obj) {
  const id = obj.getId();
  const label = labelToString(obj.getLabel());
  console.log(`Object #${id}: ${label}`);
  PrintBbox(obj.getBbox());
}

function parseOutputFile(filename) {

  const bytes = fs.readFileSync(filename);
  const output = output_msg.OutputMessage.deserializeBinary(bytes);

  const objects = output.getObjectsList();
  
  objects.forEach(function(obj, key) {
    console.log("------------------------------");
    PrintObject(obj);
  });

  
  
}

