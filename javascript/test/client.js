

var output_msg = require('./js_proto/sensr_proto/output_pb.js');
var socket;


  socket = new WebSocket("ws://localhost:5050", ["deflate-frame"]);
  socket.binaryType = "arraybuffer";
  socket.addEventListener('message', function (event) {
    //callbackFunc(event.data);
    var response = output_msg.RegularObjectResponse.deserializeBinary(event.data);
    var server_t = response.getTimeStamp();
    var server_time = (server_t.getSeconds() * 1000) + (server_t.getNanos() / 1000000)
    var client_t = new Date();
    var client_time = client_t.getTime();
    //console.log("Server:" , server_time);
    //console.log("Client:" , client_time);
    var diff = client_time - server_time;
    console.log("Diff3:" , diff);
    // var obj_list = response.getObjectsList();
    // obj_list.forEach(element => {
    //     var vel = element.getVelocity();
    //     console.log(element.getId(), element.getConfidence(), element.getLabel())
    //     console.log("Velocity:", vel.getX(), vel.getY(), vel.getZ())
    // });
  });  

