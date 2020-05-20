'use strict';

const zmq = require('zeromq');

const parsing = require('./parse_output');


class MessageReceiver {
  constructor() {
    this._socket = new zmq.Subscriber;
    this._currentConnectedAddress = null;
  }

  connect(address = 'localhost') {
    this._currentConnectedAddress = `tcp://${address}:5050`;
    console.log(`Connecting to ${this._currentConnectedAddress}`);
    this._socket.connect(this._currentConnectedAddress);
  }

  disconnect() {
    this._socket.disconnect(this._currentConnectedAddress);
  }

  resetTimeout() {
    this._socket.receiveTimeout = -1;
    this._socket.linger = -1;
  }

  setTimeout(timeout) {
    this._socket.receiveTimeout = timeout;
    this._socket.linger = 0;
  }

  async subscribe(timeout=1000) {
    this.resetTimeout();

    this._socket.subscribe();

    const outputs = [];
    let receivedOnce = false;
    while (true) {
      try {
        const [msg] = await this._socket.receive();
        const output = parsing.deserializeBinary(msg);
        outputs.push(output);
        receivedOnce = true;
        if (receivedOnce) {
          this.setTimeout(timeout);
        }
      } catch (err) {
        console.log(`No message received for ${timeout} ms.`);
        console.log('Stopping collection of messages.');
        break;
      }
    }

    this._socket.unsubscribe();

    return outputs;
  }
}


module.exports = {
  MessageReceiver,
};
