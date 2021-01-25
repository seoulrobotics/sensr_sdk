'use strict';
const {OutputProto} = require('./../js_proto/sensr_proto/output_pb.js');
const {PointCloudProto} = require('./../js_proto/sensr_proto/point_cloud_pb.js');
const parsing = require('./parse_output');
const WebSocket = require('websocket').w3cwebsocket;

class MessageReceiver {
  constructor() {
    this._currentConnectedAddressObject = null;
    this._currentConnectedAddressPoints = null;
    this._objectUpdateListener = null;
    this._pointCloudUpdateListener = null;
    
  }
 
  connect(address = 'localhost') {
    this._currentConnectedAddressObject = new WebSocket(`ws://${address}:5050`);
    this._currentConnectedAddressPoints = new WebSocket(`ws://${address}:5051`);
    this._currentConnectedAddressObject.binaryType = 'arraybuffer';
    this._currentConnectedAddressPoints.binaryType = 'arraybuffer';
    console.log(`Connecting to ${this._currentConnectedAddressObject}, 
                 and ${this._currentConnectedAddressPoints}`);
  }

  listenToObjectUpdate(streamCallback, eventCallback) {
    if (this._objectUpdateListener) {
      this._currentConnectedAddressObject.removeEventListener('message', this._objectUpdateListener);
      this._objectUpdateListener = undefined;
    }
    if (!streamCallback && !eventCallback) return;

    this._objectUpdateListener = event => {
      const response = OutputProto.OutputMessage.deserializeBinary(event.data);
      if (response.hasStream() && streamCallback) {
        streamCallback(response.getStream());
      } else if (response.hasEvent() && eventCallback) {
        eventCallback(response.getEvent());
      }
    };
    this._currentConnectedAddressObject.addEventListener('message', this._objectUpdateListener);
  }
  
  listenToPointCloudUpdate(callback) {
    if (this._pointCloudUpdateListener) {
      this._currentConnectedAddressPoints.removeEventListener('message', this._pointCloudUpdateListener);
      this._pointCloudUpdateListener = undefined;
    }
    if (!callback) return;

    this._pointCloudUpdateListener = event => {
      callback(PointCloudProto.PointResult.deserializeBinary(event.data));
    };

    this._currentConnectedAddressPoints.addEventListener('message', this._pointCloudUpdateListener);
  }

  disconnect() {
    console.log(`Disconnecting from ${this._currentConnectedAddressObject},
                 and ${this._currentConnectedAddressPoints}`);
    this._currentConnectedAddressObject.close()
    this._currentConnectedAddressPoints.close()
  }
  
  async parseOutput(outputDir, timeout) {
    let start = new Date();

    let numExported = 0;
    while(true){
      let istimeout = new Date() - start;
      if(istimeout > timeout) break;
      try{
        const msg = await this.receive();
      
        const outFilename = parsing.formatFilename(outputDir, numExported);
        parsing.exportToBinary(msg, outFilename);
        ++numExported;
      } catch (err) {
        console.log(err);
        console.log(`No message received for ${timeout} ms, ` +
                            'stopping collection of messages.');
        break;
      }
    }
    this.disconnect();
    return numExported;

  }


  async receive() {
    
    this.listenToPointCloudUpdate((response)=>{
      const [msg] = response;
      console.log(msg);
      if(response == undefined) console.log("none");
      console.log("Hello")
      this.getpoint(response);
      return response;

      this.getpoint(response);
    });
   
  }


  async getpoint(message){
    console.log(message.points);

  }

  async dumpAllReceived(outputDir, timeout=1000) {
    this.connect();
    let msg = await this.receive();
    if(msg == undefined) console.log("none");
    
    let numExported = 1;
    // const numExported = this.parseOutput(outputDir, timeout);
    return numExported;
  }
}



module.exports = {
  MessageReceiver,
};