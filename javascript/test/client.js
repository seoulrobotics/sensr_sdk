

var output_msg = require('./js_proto/sensr_proto/output_pb.js');
var point_msg = require('./js_proto/sensr_proto/point_cloud_pb.js');

var socket_result;
var socket_point;
var stream_callback_func;
var event_callback_func;

export function init() {
  socket_result = new WebSocket("ws://localhost:5050", ["deflate-frame"]);
  socket_result.binaryType = "arraybuffer";
  socket_point = new WebSocket("ws://localhost:5051", ["deflate-frame"]);
  socket_point.binaryType = "arraybuffer";

  socket_result.addEventListener('message', function (event) {
    var response = output_msg.OutputMessage.deserializeBinary(event.data);
    var server_t = response.getTimeStamp();
    var server_time = (server_t.getSeconds() * 1000) + (server_t.getNanos() / 1000000)
    var client_t = new Date();
    var client_time = client_t.getTime();
    var diff = client_time - server_time;
    console.log("Diff:" , diff);
    if (response.hasStream()) {
      stream_callback_func(response.getStream());
    } else if (response.hasEvent()) {
      event_callback_func(response.getEvent());
    }
  }); 
}

export function getStreamMessage(callbackFunc) {
  stream_callback_func = callbackFunc;
}

export function getEventMessage(callbackFunc) {
  event_callback_func = callbackFunc;
}

export function streamMessageEnd(callbackFunc) {
  socket_result.addEventListener('close', function (event) {
    callbackFunc();
  }); 
}

export function streamMessageError(callbackFunc) {
  socket_result.addEventListener('error', function (event) {
    callbackFunc();
  }); 
}

// export function getOutputMessage(callbackFunc) {
//   socket_result.addEventListener('message', function (event) {
//     var response = output_msg.OutputMessage.deserializeBinary(event.data);
//     callbackFunc(response);
//   }); 
// }

export function getPointResults(callbackFunc) {
  socket_point.addEventListener('message', function (event) {
    var response = point_msg.PointResult.deserializeBinary(event.data);
    callbackFunc(response);
  }); 
}

export function pointResultEnd(callbackFunc) {
  socket_point.addEventListener('close', function (event) {
    callbackFunc();
  }); 
}

export function pointResultError(callbackFunc) {
  socket_point.addEventListener('error', function (event) {
    callbackFunc();
  }); 
}
