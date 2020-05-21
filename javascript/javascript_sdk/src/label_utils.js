'use strict';

const labelMsg = require('./../js_proto/labels_pb.js');

module.exports = {
  labelToString,
};

function labelToString(label) {
  switch (label) {
    case labelMsg.LabelType.CAR:
      return 'Car';
    case labelMsg.LabelType.PEDESTRIAN:
      return 'Ped';
    case labelMsg.LabelType.CYCLIST:
      return 'Cyclist';
    case labelMsg.LabelType.MISC:
      return 'Misc';
    default:
      return 'Misc';
  }
}
