'use strict';

const datatypeMsg = require('./../js_proto/data_type_pb.js');

module.exports = {
  labelToString,
};

function labelToString(label) {
  switch (label) {
    case datatypeMsg.LabelType.LABEL_CAR:
      return 'Car';
    case datatypeMsg.LabelType.LABEL_PEDESTRIAN:
      return 'Ped';
    case datatypeMsg.LabelType.LABEL_CYCLIST:
      return 'Cyclist';
    case datatypeMsg.LabelType.LABEL_MISC:
      return 'Misc';
    case datatypeMsg.LabelType.LABEL_GROUND:
      return 'Ground';
    default:
      return 'Misc';
  }
}
