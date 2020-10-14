'use strict';

const {OutputServiceClient} = require('./../js_proto/output_grpc_web_pb.js');
const parsing = require('./parse_output');


class MessageReceiver {
  constructor() {
    //this._socket = new zmq.Subscriber;
    this._currentConnectedAddress = null;
  }

  connect(address = 'localhost') {
    this._currentConnectedAddress = `http://${address}:8080`;
    console.log(`Connecting to ${this._currentConnectedAddress}`);
    this._outputService = new OutputServiceClient(_currentConnectedAddress);
    var request = new OutputRequest();
    request.setMessage('Hello World!');
    this._outputService.UpdateOutput(request, {}, function(err, response) {
      console.log(`heellllo`);
    });
    //this._socket.connect(this._currentConnectedAddress);
    //this._socket.subscribe();
  }

  disconnect() {
    console.log(`Disconnecting from ${this._currentConnectedAddress}`);
    //this._socket.unsubscribe();
    //this._socket.disconnect(this._currentConnectedAddress);
  }

  resetTimeout() {
    //this._socket.receiveTimeout = -1;
    //this._socket.linger = -1;
  }

  setTimeout(timeout) {
    //this._socket.receiveTimeout = timeout;
   // this._socket.linger = 0;
  }

  async receive(err, response) {
    //const [msg] = await this._socket.receive();
    //return msg;
  }

  async dumpAllReceived(outputDir, timeout=1000) {
    this.connect();
    //this.resetTimeout();

    let numExported = 0;
    while (true) {
      try {
        //this.setTimeout(timeout);
        ++numExported;
      } catch (err) {
        console.log(`No message received for ${timeout} ms, ` +
                            'stopping collection of messages.');
        break;
      }
    }

    this.disconnect();
    return numExported;
  }
}


module.exports = {
  MessageReceiver,
};
