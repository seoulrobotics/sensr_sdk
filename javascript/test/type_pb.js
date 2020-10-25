// source: argos_proto/type.proto
/**
 * @fileoverview
 * @enhanceable
 * @suppress {messageConventions} JS Compiler reports an error if a variable or
 *     field starts with 'MSG_' and isn't a translatable message.
 * @public
 */
// GENERATED CODE -- DO NOT EDIT!

var jspb = require('google-protobuf');
var goog = jspb;
var global = Function('return this')();

var sensr_proto_global_type_pb = require('../sensr_proto/global_type_pb.js');
goog.object.extend(proto, sensr_proto_global_type_pb);
var sensr_proto_type_pb = require('../sensr_proto/type_pb.js');
goog.object.extend(proto, sensr_proto_type_pb);
goog.exportSymbol('proto.argos_proto.Object', null, global);
goog.exportSymbol('proto.argos_proto.PointCloud', null, global);
goog.exportSymbol('proto.argos_proto.PointType', null, global);
goog.exportSymbol('proto.argos_proto.TrackingInfo', null, global);
goog.exportSymbol('proto.argos_proto.Zone', null, global);
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.argos_proto.TrackingInfo = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.argos_proto.TrackingInfo.repeatedFields_, null);
};
goog.inherits(proto.argos_proto.TrackingInfo, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.argos_proto.TrackingInfo.displayName = 'proto.argos_proto.TrackingInfo';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.argos_proto.PointCloud = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.argos_proto.PointCloud, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.argos_proto.PointCloud.displayName = 'proto.argos_proto.PointCloud';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.argos_proto.Object = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, null, null);
};
goog.inherits(proto.argos_proto.Object, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.argos_proto.Object.displayName = 'proto.argos_proto.Object';
}
/**
 * Generated by JsPbCodeGenerator.
 * @param {Array=} opt_data Optional initial data array, typically from a
 * server response, or constructed directly in Javascript. The array is used
 * in place and becomes part of the constructed object. It is not cloned.
 * If no data is provided, the constructed object will be empty, but still
 * valid.
 * @extends {jspb.Message}
 * @constructor
 */
proto.argos_proto.Zone = function(opt_data) {
  jspb.Message.initialize(this, opt_data, 0, -1, proto.argos_proto.Zone.repeatedFields_, null);
};
goog.inherits(proto.argos_proto.Zone, jspb.Message);
if (goog.DEBUG && !COMPILED) {
  /**
   * @public
   * @override
   */
  proto.argos_proto.Zone.displayName = 'proto.argos_proto.Zone';
}

/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.argos_proto.TrackingInfo.repeatedFields_ = [7,8];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.argos_proto.TrackingInfo.prototype.toObject = function(opt_includeInstance) {
  return proto.argos_proto.TrackingInfo.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.argos_proto.TrackingInfo} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.argos_proto.TrackingInfo.toObject = function(includeInstance, msg) {
  var f, obj = {
    probability: jspb.Message.getFloatingPointFieldWithDefault(msg, 4, 0.0),
    trackingReliable: jspb.Message.getBooleanFieldWithDefault(msg, 5, false),
    velocity: (f = msg.getVelocity()) && sensr_proto_global_type_pb.Vector3.toObject(includeInstance, f),
    historyList: jspb.Message.toObjectList(msg.getHistoryList(),
    sensr_proto_global_type_pb.Vector3.toObject, includeInstance),
    predictionList: jspb.Message.toObjectList(msg.getPredictionList(),
    sensr_proto_global_type_pb.Vector3.toObject, includeInstance)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.argos_proto.TrackingInfo}
 */
proto.argos_proto.TrackingInfo.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.argos_proto.TrackingInfo;
  return proto.argos_proto.TrackingInfo.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.argos_proto.TrackingInfo} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.argos_proto.TrackingInfo}
 */
proto.argos_proto.TrackingInfo.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 4:
      var value = /** @type {number} */ (reader.readFloat());
      msg.setProbability(value);
      break;
    case 5:
      var value = /** @type {boolean} */ (reader.readBool());
      msg.setTrackingReliable(value);
      break;
    case 6:
      var value = new sensr_proto_global_type_pb.Vector3;
      reader.readMessage(value,sensr_proto_global_type_pb.Vector3.deserializeBinaryFromReader);
      msg.setVelocity(value);
      break;
    case 7:
      var value = new sensr_proto_global_type_pb.Vector3;
      reader.readMessage(value,sensr_proto_global_type_pb.Vector3.deserializeBinaryFromReader);
      msg.addHistory(value);
      break;
    case 8:
      var value = new sensr_proto_global_type_pb.Vector3;
      reader.readMessage(value,sensr_proto_global_type_pb.Vector3.deserializeBinaryFromReader);
      msg.addPrediction(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.argos_proto.TrackingInfo.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.argos_proto.TrackingInfo.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.argos_proto.TrackingInfo} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.argos_proto.TrackingInfo.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getProbability();
  if (f !== 0.0) {
    writer.writeFloat(
      4,
      f
    );
  }
  f = message.getTrackingReliable();
  if (f) {
    writer.writeBool(
      5,
      f
    );
  }
  f = message.getVelocity();
  if (f != null) {
    writer.writeMessage(
      6,
      f,
      sensr_proto_global_type_pb.Vector3.serializeBinaryToWriter
    );
  }
  f = message.getHistoryList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      7,
      f,
      sensr_proto_global_type_pb.Vector3.serializeBinaryToWriter
    );
  }
  f = message.getPredictionList();
  if (f.length > 0) {
    writer.writeRepeatedMessage(
      8,
      f,
      sensr_proto_global_type_pb.Vector3.serializeBinaryToWriter
    );
  }
};


/**
 * optional float probability = 4;
 * @return {number}
 */
proto.argos_proto.TrackingInfo.prototype.getProbability = function() {
  return /** @type {number} */ (jspb.Message.getFloatingPointFieldWithDefault(this, 4, 0.0));
};


/**
 * @param {number} value
 * @return {!proto.argos_proto.TrackingInfo} returns this
 */
proto.argos_proto.TrackingInfo.prototype.setProbability = function(value) {
  return jspb.Message.setProto3FloatField(this, 4, value);
};


/**
 * optional bool tracking_reliable = 5;
 * @return {boolean}
 */
proto.argos_proto.TrackingInfo.prototype.getTrackingReliable = function() {
  return /** @type {boolean} */ (jspb.Message.getBooleanFieldWithDefault(this, 5, false));
};


/**
 * @param {boolean} value
 * @return {!proto.argos_proto.TrackingInfo} returns this
 */
proto.argos_proto.TrackingInfo.prototype.setTrackingReliable = function(value) {
  return jspb.Message.setProto3BooleanField(this, 5, value);
};


/**
 * optional Vector3 velocity = 6;
 * @return {?proto.Vector3}
 */
proto.argos_proto.TrackingInfo.prototype.getVelocity = function() {
  return /** @type{?proto.Vector3} */ (
    jspb.Message.getWrapperField(this, sensr_proto_global_type_pb.Vector3, 6));
};


/**
 * @param {?proto.Vector3|undefined} value
 * @return {!proto.argos_proto.TrackingInfo} returns this
*/
proto.argos_proto.TrackingInfo.prototype.setVelocity = function(value) {
  return jspb.Message.setWrapperField(this, 6, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.argos_proto.TrackingInfo} returns this
 */
proto.argos_proto.TrackingInfo.prototype.clearVelocity = function() {
  return this.setVelocity(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.argos_proto.TrackingInfo.prototype.hasVelocity = function() {
  return jspb.Message.getField(this, 6) != null;
};


/**
 * repeated Vector3 history = 7;
 * @return {!Array<!proto.Vector3>}
 */
proto.argos_proto.TrackingInfo.prototype.getHistoryList = function() {
  return /** @type{!Array<!proto.Vector3>} */ (
    jspb.Message.getRepeatedWrapperField(this, sensr_proto_global_type_pb.Vector3, 7));
};


/**
 * @param {!Array<!proto.Vector3>} value
 * @return {!proto.argos_proto.TrackingInfo} returns this
*/
proto.argos_proto.TrackingInfo.prototype.setHistoryList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 7, value);
};


/**
 * @param {!proto.Vector3=} opt_value
 * @param {number=} opt_index
 * @return {!proto.Vector3}
 */
proto.argos_proto.TrackingInfo.prototype.addHistory = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 7, opt_value, proto.Vector3, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.argos_proto.TrackingInfo} returns this
 */
proto.argos_proto.TrackingInfo.prototype.clearHistoryList = function() {
  return this.setHistoryList([]);
};


/**
 * repeated Vector3 prediction = 8;
 * @return {!Array<!proto.Vector3>}
 */
proto.argos_proto.TrackingInfo.prototype.getPredictionList = function() {
  return /** @type{!Array<!proto.Vector3>} */ (
    jspb.Message.getRepeatedWrapperField(this, sensr_proto_global_type_pb.Vector3, 8));
};


/**
 * @param {!Array<!proto.Vector3>} value
 * @return {!proto.argos_proto.TrackingInfo} returns this
*/
proto.argos_proto.TrackingInfo.prototype.setPredictionList = function(value) {
  return jspb.Message.setRepeatedWrapperField(this, 8, value);
};


/**
 * @param {!proto.Vector3=} opt_value
 * @param {number=} opt_index
 * @return {!proto.Vector3}
 */
proto.argos_proto.TrackingInfo.prototype.addPrediction = function(opt_value, opt_index) {
  return jspb.Message.addToRepeatedWrapperField(this, 8, opt_value, proto.Vector3, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.argos_proto.TrackingInfo} returns this
 */
proto.argos_proto.TrackingInfo.prototype.clearPredictionList = function() {
  return this.setPredictionList([]);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.argos_proto.PointCloud.prototype.toObject = function(opt_includeInstance) {
  return proto.argos_proto.PointCloud.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.argos_proto.PointCloud} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.argos_proto.PointCloud.toObject = function(includeInstance, msg) {
  var f, obj = {
    type: jspb.Message.getFieldWithDefault(msg, 1, 0),
    id: jspb.Message.getFieldWithDefault(msg, 2, ""),
    points: msg.getPoints_asB64()
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.argos_proto.PointCloud}
 */
proto.argos_proto.PointCloud.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.argos_proto.PointCloud;
  return proto.argos_proto.PointCloud.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.argos_proto.PointCloud} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.argos_proto.PointCloud}
 */
proto.argos_proto.PointCloud.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {!proto.argos_proto.PointType} */ (reader.readEnum());
      msg.setType(value);
      break;
    case 2:
      var value = /** @type {string} */ (reader.readString());
      msg.setId(value);
      break;
    case 3:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setPoints(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.argos_proto.PointCloud.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.argos_proto.PointCloud.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.argos_proto.PointCloud} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.argos_proto.PointCloud.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getType();
  if (f !== 0.0) {
    writer.writeEnum(
      1,
      f
    );
  }
  f = message.getId();
  if (f.length > 0) {
    writer.writeString(
      2,
      f
    );
  }
  f = message.getPoints_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      3,
      f
    );
  }
};


/**
 * optional PointType type = 1;
 * @return {!proto.argos_proto.PointType}
 */
proto.argos_proto.PointCloud.prototype.getType = function() {
  return /** @type {!proto.argos_proto.PointType} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {!proto.argos_proto.PointType} value
 * @return {!proto.argos_proto.PointCloud} returns this
 */
proto.argos_proto.PointCloud.prototype.setType = function(value) {
  return jspb.Message.setProto3EnumField(this, 1, value);
};


/**
 * optional string id = 2;
 * @return {string}
 */
proto.argos_proto.PointCloud.prototype.getId = function() {
  return /** @type {string} */ (jspb.Message.getFieldWithDefault(this, 2, ""));
};


/**
 * @param {string} value
 * @return {!proto.argos_proto.PointCloud} returns this
 */
proto.argos_proto.PointCloud.prototype.setId = function(value) {
  return jspb.Message.setProto3StringField(this, 2, value);
};


/**
 * optional bytes points = 3;
 * @return {!(string|Uint8Array)}
 */
proto.argos_proto.PointCloud.prototype.getPoints = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 3, ""));
};


/**
 * optional bytes points = 3;
 * This is a type-conversion wrapper around `getPoints()`
 * @return {string}
 */
proto.argos_proto.PointCloud.prototype.getPoints_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getPoints()));
};


/**
 * optional bytes points = 3;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getPoints()`
 * @return {!Uint8Array}
 */
proto.argos_proto.PointCloud.prototype.getPoints_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getPoints()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.argos_proto.PointCloud} returns this
 */
proto.argos_proto.PointCloud.prototype.setPoints = function(value) {
  return jspb.Message.setProto3BytesField(this, 3, value);
};





if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.argos_proto.Object.prototype.toObject = function(opt_includeInstance) {
  return proto.argos_proto.Object.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.argos_proto.Object} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.argos_proto.Object.toObject = function(includeInstance, msg) {
  var f, obj = {
    id: jspb.Message.getFieldWithDefault(msg, 1, 0),
    bbox: (f = msg.getBbox()) && sensr_proto_type_pb.BoundingBox.toObject(includeInstance, f),
    label: jspb.Message.getFieldWithDefault(msg, 3, 0),
    track: (f = msg.getTrack()) && proto.argos_proto.TrackingInfo.toObject(includeInstance, f),
    points: msg.getPoints_asB64()
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.argos_proto.Object}
 */
proto.argos_proto.Object.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.argos_proto.Object;
  return proto.argos_proto.Object.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.argos_proto.Object} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.argos_proto.Object}
 */
proto.argos_proto.Object.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt32());
      msg.setId(value);
      break;
    case 2:
      var value = new sensr_proto_type_pb.BoundingBox;
      reader.readMessage(value,sensr_proto_type_pb.BoundingBox.deserializeBinaryFromReader);
      msg.setBbox(value);
      break;
    case 3:
      var value = /** @type {!proto.sensr_proto.LabelType} */ (reader.readEnum());
      msg.setLabel(value);
      break;
    case 101:
      var value = new proto.argos_proto.TrackingInfo;
      reader.readMessage(value,proto.argos_proto.TrackingInfo.deserializeBinaryFromReader);
      msg.setTrack(value);
      break;
    case 102:
      var value = /** @type {!Uint8Array} */ (reader.readBytes());
      msg.setPoints(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.argos_proto.Object.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.argos_proto.Object.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.argos_proto.Object} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.argos_proto.Object.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getId();
  if (f !== 0) {
    writer.writeInt32(
      1,
      f
    );
  }
  f = message.getBbox();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      sensr_proto_type_pb.BoundingBox.serializeBinaryToWriter
    );
  }
  f = message.getLabel();
  if (f !== 0.0) {
    writer.writeEnum(
      3,
      f
    );
  }
  f = message.getTrack();
  if (f != null) {
    writer.writeMessage(
      101,
      f,
      proto.argos_proto.TrackingInfo.serializeBinaryToWriter
    );
  }
  f = message.getPoints_asU8();
  if (f.length > 0) {
    writer.writeBytes(
      102,
      f
    );
  }
};


/**
 * optional int32 id = 1;
 * @return {number}
 */
proto.argos_proto.Object.prototype.getId = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.argos_proto.Object} returns this
 */
proto.argos_proto.Object.prototype.setId = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional sensr_proto.BoundingBox bbox = 2;
 * @return {?proto.sensr_proto.BoundingBox}
 */
proto.argos_proto.Object.prototype.getBbox = function() {
  return /** @type{?proto.sensr_proto.BoundingBox} */ (
    jspb.Message.getWrapperField(this, sensr_proto_type_pb.BoundingBox, 2));
};


/**
 * @param {?proto.sensr_proto.BoundingBox|undefined} value
 * @return {!proto.argos_proto.Object} returns this
*/
proto.argos_proto.Object.prototype.setBbox = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.argos_proto.Object} returns this
 */
proto.argos_proto.Object.prototype.clearBbox = function() {
  return this.setBbox(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.argos_proto.Object.prototype.hasBbox = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * optional sensr_proto.LabelType label = 3;
 * @return {!proto.sensr_proto.LabelType}
 */
proto.argos_proto.Object.prototype.getLabel = function() {
  return /** @type {!proto.sensr_proto.LabelType} */ (jspb.Message.getFieldWithDefault(this, 3, 0));
};


/**
 * @param {!proto.sensr_proto.LabelType} value
 * @return {!proto.argos_proto.Object} returns this
 */
proto.argos_proto.Object.prototype.setLabel = function(value) {
  return jspb.Message.setProto3EnumField(this, 3, value);
};


/**
 * optional TrackingInfo track = 101;
 * @return {?proto.argos_proto.TrackingInfo}
 */
proto.argos_proto.Object.prototype.getTrack = function() {
  return /** @type{?proto.argos_proto.TrackingInfo} */ (
    jspb.Message.getWrapperField(this, proto.argos_proto.TrackingInfo, 101));
};


/**
 * @param {?proto.argos_proto.TrackingInfo|undefined} value
 * @return {!proto.argos_proto.Object} returns this
*/
proto.argos_proto.Object.prototype.setTrack = function(value) {
  return jspb.Message.setWrapperField(this, 101, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.argos_proto.Object} returns this
 */
proto.argos_proto.Object.prototype.clearTrack = function() {
  return this.setTrack(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.argos_proto.Object.prototype.hasTrack = function() {
  return jspb.Message.getField(this, 101) != null;
};


/**
 * optional bytes points = 102;
 * @return {!(string|Uint8Array)}
 */
proto.argos_proto.Object.prototype.getPoints = function() {
  return /** @type {!(string|Uint8Array)} */ (jspb.Message.getFieldWithDefault(this, 102, ""));
};


/**
 * optional bytes points = 102;
 * This is a type-conversion wrapper around `getPoints()`
 * @return {string}
 */
proto.argos_proto.Object.prototype.getPoints_asB64 = function() {
  return /** @type {string} */ (jspb.Message.bytesAsB64(
      this.getPoints()));
};


/**
 * optional bytes points = 102;
 * Note that Uint8Array is not supported on all browsers.
 * @see http://caniuse.com/Uint8Array
 * This is a type-conversion wrapper around `getPoints()`
 * @return {!Uint8Array}
 */
proto.argos_proto.Object.prototype.getPoints_asU8 = function() {
  return /** @type {!Uint8Array} */ (jspb.Message.bytesAsU8(
      this.getPoints()));
};


/**
 * @param {!(string|Uint8Array)} value
 * @return {!proto.argos_proto.Object} returns this
 */
proto.argos_proto.Object.prototype.setPoints = function(value) {
  return jspb.Message.setProto3BytesField(this, 102, value);
};



/**
 * List of repeated fields within this message type.
 * @private {!Array<number>}
 * @const
 */
proto.argos_proto.Zone.repeatedFields_ = [3];



if (jspb.Message.GENERATE_TO_OBJECT) {
/**
 * Creates an object representation of this proto.
 * Field names that are reserved in JavaScript and will be renamed to pb_name.
 * Optional fields that are not set will be set to undefined.
 * To access a reserved field use, foo.pb_<name>, eg, foo.pb_default.
 * For the list of reserved names please see:
 *     net/proto2/compiler/js/internal/generator.cc#kKeyword.
 * @param {boolean=} opt_includeInstance Deprecated. whether to include the
 *     JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @return {!Object}
 */
proto.argos_proto.Zone.prototype.toObject = function(opt_includeInstance) {
  return proto.argos_proto.Zone.toObject(opt_includeInstance, this);
};


/**
 * Static version of the {@see toObject} method.
 * @param {boolean|undefined} includeInstance Deprecated. Whether to include
 *     the JSPB instance for transitional soy proto support:
 *     http://goto/soy-param-migration
 * @param {!proto.argos_proto.Zone} msg The msg instance to transform.
 * @return {!Object}
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.argos_proto.Zone.toObject = function(includeInstance, msg) {
  var f, obj = {
    id: jspb.Message.getFieldWithDefault(msg, 1, 0),
    polygonBox: (f = msg.getPolygonBox()) && sensr_proto_type_pb.PolygonBox.toObject(includeInstance, f),
    objectIdsList: (f = jspb.Message.getRepeatedField(msg, 3)) == null ? undefined : f,
    zoneType: jspb.Message.getFieldWithDefault(msg, 4, 0)
  };

  if (includeInstance) {
    obj.$jspbMessageInstance = msg;
  }
  return obj;
};
}


/**
 * Deserializes binary data (in protobuf wire format).
 * @param {jspb.ByteSource} bytes The bytes to deserialize.
 * @return {!proto.argos_proto.Zone}
 */
proto.argos_proto.Zone.deserializeBinary = function(bytes) {
  var reader = new jspb.BinaryReader(bytes);
  var msg = new proto.argos_proto.Zone;
  return proto.argos_proto.Zone.deserializeBinaryFromReader(msg, reader);
};


/**
 * Deserializes binary data (in protobuf wire format) from the
 * given reader into the given message object.
 * @param {!proto.argos_proto.Zone} msg The message object to deserialize into.
 * @param {!jspb.BinaryReader} reader The BinaryReader to use.
 * @return {!proto.argos_proto.Zone}
 */
proto.argos_proto.Zone.deserializeBinaryFromReader = function(msg, reader) {
  while (reader.nextField()) {
    if (reader.isEndGroup()) {
      break;
    }
    var field = reader.getFieldNumber();
    switch (field) {
    case 1:
      var value = /** @type {number} */ (reader.readInt32());
      msg.setId(value);
      break;
    case 2:
      var value = new sensr_proto_type_pb.PolygonBox;
      reader.readMessage(value,sensr_proto_type_pb.PolygonBox.deserializeBinaryFromReader);
      msg.setPolygonBox(value);
      break;
    case 3:
      var value = /** @type {!Array<number>} */ (reader.readPackedInt32());
      msg.setObjectIdsList(value);
      break;
    case 4:
      var value = /** @type {number} */ (reader.readInt32());
      msg.setZoneType(value);
      break;
    default:
      reader.skipField();
      break;
    }
  }
  return msg;
};


/**
 * Serializes the message to binary data (in protobuf wire format).
 * @return {!Uint8Array}
 */
proto.argos_proto.Zone.prototype.serializeBinary = function() {
  var writer = new jspb.BinaryWriter();
  proto.argos_proto.Zone.serializeBinaryToWriter(this, writer);
  return writer.getResultBuffer();
};


/**
 * Serializes the given message to binary data (in protobuf wire
 * format), writing to the given BinaryWriter.
 * @param {!proto.argos_proto.Zone} message
 * @param {!jspb.BinaryWriter} writer
 * @suppress {unusedLocalVariables} f is only used for nested messages
 */
proto.argos_proto.Zone.serializeBinaryToWriter = function(message, writer) {
  var f = undefined;
  f = message.getId();
  if (f !== 0) {
    writer.writeInt32(
      1,
      f
    );
  }
  f = message.getPolygonBox();
  if (f != null) {
    writer.writeMessage(
      2,
      f,
      sensr_proto_type_pb.PolygonBox.serializeBinaryToWriter
    );
  }
  f = message.getObjectIdsList();
  if (f.length > 0) {
    writer.writePackedInt32(
      3,
      f
    );
  }
  f = message.getZoneType();
  if (f !== 0) {
    writer.writeInt32(
      4,
      f
    );
  }
};


/**
 * optional int32 id = 1;
 * @return {number}
 */
proto.argos_proto.Zone.prototype.getId = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 1, 0));
};


/**
 * @param {number} value
 * @return {!proto.argos_proto.Zone} returns this
 */
proto.argos_proto.Zone.prototype.setId = function(value) {
  return jspb.Message.setProto3IntField(this, 1, value);
};


/**
 * optional sensr_proto.PolygonBox polygon_box = 2;
 * @return {?proto.sensr_proto.PolygonBox}
 */
proto.argos_proto.Zone.prototype.getPolygonBox = function() {
  return /** @type{?proto.sensr_proto.PolygonBox} */ (
    jspb.Message.getWrapperField(this, sensr_proto_type_pb.PolygonBox, 2));
};


/**
 * @param {?proto.sensr_proto.PolygonBox|undefined} value
 * @return {!proto.argos_proto.Zone} returns this
*/
proto.argos_proto.Zone.prototype.setPolygonBox = function(value) {
  return jspb.Message.setWrapperField(this, 2, value);
};


/**
 * Clears the message field making it undefined.
 * @return {!proto.argos_proto.Zone} returns this
 */
proto.argos_proto.Zone.prototype.clearPolygonBox = function() {
  return this.setPolygonBox(undefined);
};


/**
 * Returns whether this field is set.
 * @return {boolean}
 */
proto.argos_proto.Zone.prototype.hasPolygonBox = function() {
  return jspb.Message.getField(this, 2) != null;
};


/**
 * repeated int32 object_ids = 3;
 * @return {!Array<number>}
 */
proto.argos_proto.Zone.prototype.getObjectIdsList = function() {
  return /** @type {!Array<number>} */ (jspb.Message.getRepeatedField(this, 3));
};


/**
 * @param {!Array<number>} value
 * @return {!proto.argos_proto.Zone} returns this
 */
proto.argos_proto.Zone.prototype.setObjectIdsList = function(value) {
  return jspb.Message.setField(this, 3, value || []);
};


/**
 * @param {number} value
 * @param {number=} opt_index
 * @return {!proto.argos_proto.Zone} returns this
 */
proto.argos_proto.Zone.prototype.addObjectIds = function(value, opt_index) {
  return jspb.Message.addToRepeatedField(this, 3, value, opt_index);
};


/**
 * Clears the list making it empty but non-null.
 * @return {!proto.argos_proto.Zone} returns this
 */
proto.argos_proto.Zone.prototype.clearObjectIdsList = function() {
  return this.setObjectIdsList([]);
};


/**
 * optional int32 zone_type = 4;
 * @return {number}
 */
proto.argos_proto.Zone.prototype.getZoneType = function() {
  return /** @type {number} */ (jspb.Message.getFieldWithDefault(this, 4, 0));
};


/**
 * @param {number} value
 * @return {!proto.argos_proto.Zone} returns this
 */
proto.argos_proto.Zone.prototype.setZoneType = function(value) {
  return jspb.Message.setProto3IntField(this, 4, value);
};


/**
 * @enum {number}
 */
proto.argos_proto.PointType = {
  POINT_TYPE_GROUND: 0,
  POINT_TYPE_ENVIRONMENT: 1,
  POINT_TYPE_RAW: 2
};

goog.object.extend(exports, proto.argos_proto);
