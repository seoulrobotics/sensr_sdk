'use strict';

// Proto
const label_utils = require('./label_utils.js')

// Formatter settings
const NUM_DECIMALS = 3;

module.exports = {
  printObject
}

function Format(float) {
  return float.toFixed(NUM_DECIMALS);
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
  
function printObject(obj) {
  const id = obj.getId();
  const label = label_utils.labelToString(obj.getLabel());
  console.log(`Object #${id}: ${label}`);
  PrintBbox(obj.getBbox());
}