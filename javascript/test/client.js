// import { Empty } from 'google-protobuf/google/protobuf/empty_pb';
const { Empty } =require('google-protobuf/google/protobuf/empty_pb')
const { RegularObjectResponse, CommandRequest, CommandResponse, Command } = require('./js_proto/sensr_proto/output_pb.js');
const { LidarPerceptionServiceClient } = require('./js_proto/sensr_proto/output_grpc_web_pb.js');


var client = new LidarPerceptionServiceClient(
  'http://112.216.77.162:8080'
)
var request = new Empty();
var call = client.regularObjectUpdate(request, ()=>{});

window.onunload = window.onbeforeunload = function (evt) {
  var message = 'Are you sure?';
  if (typeof evt == 'undefined') {
      evt = window.event;
  }
  if (evt) {
      if (evt.type == "unload" && evt.returnValue) {
          let request1 = new CommandRequest();
          request1.setCommand(Command.CMD_CUSTOM);
          //request1.getParamMap()["stop"] = "empty";
          request1.getParamMap().set("stop", "empty");
          client.sendCommand(request1, ()=>{});
      }
      evt.returnValue = message;
  }
  return message;
};

export function getData(callbackFunc) {
  call.on('data', function(response) {
    callbackFunc(response); 
  });
}

export function getStatus(callbackFunc) {
  call.on("status", (status) => {
    callbackFunc(status)
  });
}

export function getError(callbackFunc) {
  call.on("error", (error) =>{
    callbackFunc(error)
  });
}

export function getEnd(callbackFunc) {
  call.on("end", (end)=>{
    callbackFunc(end)
  });
}

