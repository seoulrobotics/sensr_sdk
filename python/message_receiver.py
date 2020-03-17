import os
import zmq

import labels_pb2
import output_pb2

class MessageReceiver(object):
    def __init__(self, address='localhost'):
        self._context = zmq.Context()

        socket = self._context.socket(zmq.SUB)
        socket.connect(f"tcp://{address}:5050")
        socket.setsockopt(zmq.SUBSCRIBE, b'')

        self._socket = socket

    def reset_timeout(self):
        self._socket.setsockopt(zmq.LINGER, -1)
        self._socket.setsockopt(zmq.RCVTIMEO, -1)

    def set_timeout(self, timeout):
        assert timeout > 0
        self._socket.setsockopt(zmq.LINGER, 0)
        self._socket.setsockopt(zmq.RCVTIMEO, timeout)

    def subscribe(self, *, timeout=1000):
        self.reset_timeout()

        outputs = []
        while True:
            output = output_pb2.OutputMessage()
            try:
                msg = self._socket.recv()
                output.ParseFromString(msg)
                outputs.append(output)
                self.set_timeout(timeout)
            except zmq.Again:
                break

        return outputs

def main():
    msg_rcv = MessageReceiver()
    print(labels_pb2.LabelType.keys())

    output_dir = 'sample_output'
    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)

    outputs = msg_rcv.subscribe()

    for i, output in enumerate(outputs):
        output_fn = os.path.join(output_dir, f'{i:04}.bin')
        with open(output_fn, 'wb') as fp:
            fp.write(output.SerializeToString())

if __name__ == '__main__':
    main()