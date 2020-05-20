'use strict';

// Proto
const labelUtils = require('./label_utils.js');

// Formatter settings
const NUM_DECIMALS = 3;

module.exports = {
  printObject,
};

function format(float) {
  return float.toFixed(NUM_DECIMALS);
}

function prettyPrintFloat(float, name) {
  console.log(`${name}:\n\t${format(float)}`);
}

function prettyPrintVec2(vec, name) {
  const x = format(vec.getX());
  const y = format(vec.getY());
  console.log(`${name}:\n\t${x}\n\t${y}`);
}

function prettyPrintVec3(vec, name) {
  const x = format(vec.getX());
  const y = format(vec.getY());
  const z = format(vec.getZ());
  console.log(`${name}:\n\t${x}\n\t${y}\n\t${z}`);
}

function printBbox(bbox) {
  prettyPrintVec2(bbox.getPosition(), 'position');
  prettyPrintVec3(bbox.getSize(), 'size');
  prettyPrintFloat(bbox.getYaw(), 'yaw');
}

function printObject(obj) {
  const id = obj.getId();
  const label = labelUtils.labelToString(obj.getLabel());
  console.log(`Object #${id}: ${label}`);
  printBbox(obj.getBbox());
}
