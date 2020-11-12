

var output_msg = require('./js_proto/sensr_proto/output_pb.js');
var point_msg = require('./js_proto/sensr_proto/point_cloud_pb.js');
var socket_result;
var socket_point;


  socket_result = new WebSocket("ws://localhost:5050", ["deflate-frame"]);
  socket_result.binaryType = "arraybuffer";
  socket_result.addEventListener('message', function (event) {
    //callbackFunc(event.data);
    var response = output_msg.OutputMessage.deserializeBinary(event.data);
    var server_t = response.getTimeStamp();
    var server_time = (server_t.getSeconds() * 1000) + (server_t.getNanos() / 1000000)
    var client_t = new Date();
    var client_time = client_t.getTime();
    //console.log("Server:" , server_time);
    //console.log("Client:" , client_time);
    var diff = client_time - server_time;
    console.log("Diff3:" , diff);
    var obj_list = response.getStream().getObjectsList();
     obj_list.forEach(element => {
         var point_list = element.getPoints_asU8();
         console.log(element.getId(), element.getConfidence(), element.getLabel(), point_list.length)
     });
  });  

  socket_point = new WebSocket("ws://localhost:5051", ["deflate-frame"]);
  socket_point.binaryType = "arraybuffer";
  socket_point.addEventListener('message', function (event) {
    //callbackFunc(event.data);
    var response = point_msg.PointResult.deserializeBinary(event.data);
    var obj_list = response.getPointsList();
     obj_list.forEach(element => {
         var point_list = element.getPoints_asU8();
         console.log(element.getId(), point_list.length)
     });
  });

