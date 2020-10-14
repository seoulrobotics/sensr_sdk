const {OuputRequest, OutputMessage, TestCommand, TestReply} = require('./output_pb.js');
const {OutputServiceClient} = require('./output_grpc_web_pb.js');

var client = new OutputServiceClient('http://localhost:8080');

var request = new OuputRequest();
request.setMessage('World');

var stream = client.updateOutput(request, {'custom-header-1': 'value1'});
stream.on('data', function(response) {
    var obj_list = response.getObjectsList();
    obj_list.forEach(element => {
        console.log(element.getId());
    });
  });
  stream.on('status', function(status) {
    console.log(status.code);
    console.log(status.details);
    console.log(status.metadata);
  });
  stream.on('end', function(end) {
    console.log("endddddd");
  });

  var request = new TestCommand();
    request.setMessage('Hello World!aaa');

    client.sendCommand(request, {}, function(err, response) {
        console.log(response.getMessage());
    });
