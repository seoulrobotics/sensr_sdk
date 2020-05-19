'use strict';

const label_msg = require('./../js_proto/labels_pb.js')

module.exports = {
  labelToString
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