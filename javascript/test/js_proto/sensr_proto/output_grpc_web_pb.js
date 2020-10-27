/**
 * @fileoverview gRPC-Web generated client stub for sensr_proto
 * @enhanceable
 * @public
 */

// GENERATED CODE -- DO NOT EDIT!


/* eslint-disable */
// @ts-nocheck



const grpc = {};
grpc.web = require('grpc-web');


var google_protobuf_timestamp_pb = require('google-protobuf/google/protobuf/timestamp_pb.js')

var google_protobuf_empty_pb = require('google-protobuf/google/protobuf/empty_pb.js')

var google_api_annotations_pb = require('../google/api/annotations_pb.js')

var argos_proto_type_pb = require('../argos_proto/type_pb.js')

var sensr_proto_type_pb = require('../sensr_proto/type_pb.js')
const proto = {};
proto.sensr_proto = require('./output_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.sensr_proto.LidarPerceptionServiceClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options['format'] = 'text';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.sensr_proto.LidarPerceptionServicePromiseClient =
    function(hostname, credentials, options) {
  if (!options) options = {};
  options['format'] = 'text';

  /**
   * @private @const {!grpc.web.GrpcWebClientBase} The client
   */
  this.client_ = new grpc.web.GrpcWebClientBase(options);

  /**
   * @private @const {string} The hostname
   */
  this.hostname_ = hostname;

};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.google.protobuf.Empty,
 *   !proto.sensr_proto.RegularObjectResponse>}
 */
const methodDescriptor_LidarPerceptionService_RegularObjectUpdate = new grpc.web.MethodDescriptor(
  '/sensr_proto.LidarPerceptionService/RegularObjectUpdate',
  grpc.web.MethodType.SERVER_STREAMING,
  google_protobuf_empty_pb.Empty,
  proto.sensr_proto.RegularObjectResponse,
  /**
   * @param {!proto.google.protobuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.sensr_proto.RegularObjectResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.google.protobuf.Empty,
 *   !proto.sensr_proto.RegularObjectResponse>}
 */
const methodInfo_LidarPerceptionService_RegularObjectUpdate = new grpc.web.AbstractClientBase.MethodInfo(
  proto.sensr_proto.RegularObjectResponse,
  /**
   * @param {!proto.google.protobuf.Empty} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.sensr_proto.RegularObjectResponse.deserializeBinary
);


/**
 * @param {!proto.google.protobuf.Empty} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.sensr_proto.RegularObjectResponse>}
 *     The XHR Node Readable Stream
 */
proto.sensr_proto.LidarPerceptionServiceClient.prototype.regularObjectUpdate =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/sensr_proto.LidarPerceptionService/RegularObjectUpdate',
      request,
      metadata || {},
      methodDescriptor_LidarPerceptionService_RegularObjectUpdate);
};


/**
 * @param {!proto.google.protobuf.Empty} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.sensr_proto.RegularObjectResponse>}
 *     The XHR Node Readable Stream
 */
proto.sensr_proto.LidarPerceptionServicePromiseClient.prototype.regularObjectUpdate =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/sensr_proto.LidarPerceptionService/RegularObjectUpdate',
      request,
      metadata || {},
      methodDescriptor_LidarPerceptionService_RegularObjectUpdate);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.sensr_proto.CommandRequest,
 *   !proto.sensr_proto.CommandResponse>}
 */
const methodDescriptor_LidarPerceptionService_SendCommand = new grpc.web.MethodDescriptor(
  '/sensr_proto.LidarPerceptionService/SendCommand',
  grpc.web.MethodType.UNARY,
  proto.sensr_proto.CommandRequest,
  proto.sensr_proto.CommandResponse,
  /**
   * @param {!proto.sensr_proto.CommandRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.sensr_proto.CommandResponse.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.sensr_proto.CommandRequest,
 *   !proto.sensr_proto.CommandResponse>}
 */
const methodInfo_LidarPerceptionService_SendCommand = new grpc.web.AbstractClientBase.MethodInfo(
  proto.sensr_proto.CommandResponse,
  /**
   * @param {!proto.sensr_proto.CommandRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.sensr_proto.CommandResponse.deserializeBinary
);


/**
 * @param {!proto.sensr_proto.CommandRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.sensr_proto.CommandResponse)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.sensr_proto.CommandResponse>|undefined}
 *     The XHR Node Readable Stream
 */
proto.sensr_proto.LidarPerceptionServiceClient.prototype.sendCommand =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/sensr_proto.LidarPerceptionService/SendCommand',
      request,
      metadata || {},
      methodDescriptor_LidarPerceptionService_SendCommand,
      callback);
};


/**
 * @param {!proto.sensr_proto.CommandRequest} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.sensr_proto.CommandResponse>}
 *     Promise that resolves to the response
 */
proto.sensr_proto.LidarPerceptionServicePromiseClient.prototype.sendCommand =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/sensr_proto.LidarPerceptionService/SendCommand',
      request,
      metadata || {},
      methodDescriptor_LidarPerceptionService_SendCommand);
};


module.exports = proto.sensr_proto;

