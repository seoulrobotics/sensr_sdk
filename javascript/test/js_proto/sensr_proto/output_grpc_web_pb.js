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

var google_api_annotations_pb = require('../google/api/annotations_pb.js')

var argos_proto_type_pb = require('../argos_proto/type_pb.js')
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
proto.sensr_proto.OutputServiceClient =
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
proto.sensr_proto.OutputServicePromiseClient =
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
 *   !proto.sensr_proto.OuputRequest,
 *   !proto.sensr_proto.OutputMessage>}
 */
const methodDescriptor_OutputService_UpdateOutput = new grpc.web.MethodDescriptor(
  '/sensr_proto.OutputService/UpdateOutput',
  grpc.web.MethodType.SERVER_STREAMING,
  proto.sensr_proto.OuputRequest,
  proto.sensr_proto.OutputMessage,
  /**
   * @param {!proto.sensr_proto.OuputRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.sensr_proto.OutputMessage.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.sensr_proto.OuputRequest,
 *   !proto.sensr_proto.OutputMessage>}
 */
const methodInfo_OutputService_UpdateOutput = new grpc.web.AbstractClientBase.MethodInfo(
  proto.sensr_proto.OutputMessage,
  /**
   * @param {!proto.sensr_proto.OuputRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.sensr_proto.OutputMessage.deserializeBinary
);


/**
 * @param {!proto.sensr_proto.OuputRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.sensr_proto.OutputMessage>}
 *     The XHR Node Readable Stream
 */
proto.sensr_proto.OutputServiceClient.prototype.updateOutput =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/sensr_proto.OutputService/UpdateOutput',
      request,
      metadata || {},
      methodDescriptor_OutputService_UpdateOutput);
};


/**
 * @param {!proto.sensr_proto.OuputRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.sensr_proto.OutputMessage>}
 *     The XHR Node Readable Stream
 */
proto.sensr_proto.OutputServicePromiseClient.prototype.updateOutput =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/sensr_proto.OutputService/UpdateOutput',
      request,
      metadata || {},
      methodDescriptor_OutputService_UpdateOutput);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.sensr_proto.TestCommand,
 *   !proto.sensr_proto.TestReply>}
 */
const methodDescriptor_OutputService_SendCommand = new grpc.web.MethodDescriptor(
  '/sensr_proto.OutputService/SendCommand',
  grpc.web.MethodType.UNARY,
  proto.sensr_proto.TestCommand,
  proto.sensr_proto.TestReply,
  /**
   * @param {!proto.sensr_proto.TestCommand} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.sensr_proto.TestReply.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.sensr_proto.TestCommand,
 *   !proto.sensr_proto.TestReply>}
 */
const methodInfo_OutputService_SendCommand = new grpc.web.AbstractClientBase.MethodInfo(
  proto.sensr_proto.TestReply,
  /**
   * @param {!proto.sensr_proto.TestCommand} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.sensr_proto.TestReply.deserializeBinary
);


/**
 * @param {!proto.sensr_proto.TestCommand} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.sensr_proto.TestReply)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.sensr_proto.TestReply>|undefined}
 *     The XHR Node Readable Stream
 */
proto.sensr_proto.OutputServiceClient.prototype.sendCommand =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/sensr_proto.OutputService/SendCommand',
      request,
      metadata || {},
      methodDescriptor_OutputService_SendCommand,
      callback);
};


/**
 * @param {!proto.sensr_proto.TestCommand} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.sensr_proto.TestReply>}
 *     Promise that resolves to the response
 */
proto.sensr_proto.OutputServicePromiseClient.prototype.sendCommand =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/sensr_proto.OutputService/SendCommand',
      request,
      metadata || {},
      methodDescriptor_OutputService_SendCommand);
};


module.exports = proto.sensr_proto;

