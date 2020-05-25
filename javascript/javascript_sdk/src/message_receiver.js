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
    this._socket.subscribe();
  }

  disconnect() {
    console.log(`Disconnecting from ${this._currentConnectedAddress}`);
    this._socket.unsubscribe();
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

  async receive() {
    const [msg] = await this._socket.receive();
    return msg;
  }

  async dumpAllReceived(outputDir, timeout=1000) {
    this.connect();
    this.resetTimeout();

    let numExported = 0;
    while (true) {
      try {
        const msg = await this.receive();

        const outFilename = parsing.formatFilename(outputDir, numExported);
        parsing.exportToBinary(msg, outFilename);

        this.setTimeout(timeout);
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
