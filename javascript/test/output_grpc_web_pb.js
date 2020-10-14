/**
 * @fileoverview gRPC-Web generated client stub for 
 * @enhanceable
 * @public
 */

// GENERATED CODE -- DO NOT EDIT!


/* eslint-disable */
// @ts-nocheck



const grpc = {};
grpc.web = require('grpc-web');


var google_protobuf_timestamp_pb = require('google-protobuf/google/protobuf/timestamp_pb.js')

var data_type_pb = require('./data_type_pb.js')
const proto = require('./output_pb.js');

/**
 * @param {string} hostname
 * @param {?Object} credentials
 * @param {?Object} options
 * @constructor
 * @struct
 * @final
 */
proto.OutputServiceClient =
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
proto.OutputServicePromiseClient =
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
 *   !proto.OuputRequest,
 *   !proto.OutputMessage>}
 */
const methodDescriptor_OutputService_UpdateOutput = new grpc.web.MethodDescriptor(
  '/OutputService/UpdateOutput',
  grpc.web.MethodType.SERVER_STREAMING,
  proto.OuputRequest,
  proto.OutputMessage,
  /**
   * @param {!proto.OuputRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.OutputMessage.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.OuputRequest,
 *   !proto.OutputMessage>}
 */
const methodInfo_OutputService_UpdateOutput = new grpc.web.AbstractClientBase.MethodInfo(
  proto.OutputMessage,
  /**
   * @param {!proto.OuputRequest} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.OutputMessage.deserializeBinary
);


/**
 * @param {!proto.OuputRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.OutputMessage>}
 *     The XHR Node Readable Stream
 */
proto.OutputServiceClient.prototype.updateOutput =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/OutputService/UpdateOutput',
      request,
      metadata || {},
      methodDescriptor_OutputService_UpdateOutput);
};


/**
 * @param {!proto.OuputRequest} request The request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!grpc.web.ClientReadableStream<!proto.OutputMessage>}
 *     The XHR Node Readable Stream
 */
proto.OutputServicePromiseClient.prototype.updateOutput =
    function(request, metadata) {
  return this.client_.serverStreaming(this.hostname_ +
      '/OutputService/UpdateOutput',
      request,
      metadata || {},
      methodDescriptor_OutputService_UpdateOutput);
};


/**
 * @const
 * @type {!grpc.web.MethodDescriptor<
 *   !proto.TestCommand,
 *   !proto.TestReply>}
 */
const methodDescriptor_OutputService_SendCommand = new grpc.web.MethodDescriptor(
  '/OutputService/SendCommand',
  grpc.web.MethodType.UNARY,
  proto.TestCommand,
  proto.TestReply,
  /**
   * @param {!proto.TestCommand} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.TestReply.deserializeBinary
);


/**
 * @const
 * @type {!grpc.web.AbstractClientBase.MethodInfo<
 *   !proto.TestCommand,
 *   !proto.TestReply>}
 */
const methodInfo_OutputService_SendCommand = new grpc.web.AbstractClientBase.MethodInfo(
  proto.TestReply,
  /**
   * @param {!proto.TestCommand} request
   * @return {!Uint8Array}
   */
  function(request) {
    return request.serializeBinary();
  },
  proto.TestReply.deserializeBinary
);


/**
 * @param {!proto.TestCommand} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @param {function(?grpc.web.Error, ?proto.TestReply)}
 *     callback The callback function(error, response)
 * @return {!grpc.web.ClientReadableStream<!proto.TestReply>|undefined}
 *     The XHR Node Readable Stream
 */
proto.OutputServiceClient.prototype.sendCommand =
    function(request, metadata, callback) {
  return this.client_.rpcCall(this.hostname_ +
      '/OutputService/SendCommand',
      request,
      metadata || {},
      methodDescriptor_OutputService_SendCommand,
      callback);
};


/**
 * @param {!proto.TestCommand} request The
 *     request proto
 * @param {?Object<string, string>} metadata User defined
 *     call metadata
 * @return {!Promise<!proto.TestReply>}
 *     Promise that resolves to the response
 */
proto.OutputServicePromiseClient.prototype.sendCommand =
    function(request, metadata) {
  return this.client_.unaryCall(this.hostname_ +
      '/OutputService/SendCommand',
      request,
      metadata || {},
      methodDescriptor_OutputService_SendCommand);
};


module.exports = proto;

