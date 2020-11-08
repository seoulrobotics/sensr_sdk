import os
import sys
import tqdm
import zmq

import output_pb2

class MessageReceiver(object):
    def __init__(self, output_dir, address='localhost'):
        self._output_dir = output_dir
        self._context = zmq.Context()

        socket = self._context.socket(zmq.SUB)
        self._current_address = f"tcp://{address}:5050"
        print('Connecting to {}'.format(self._current_address))
        socket.connect(self._current_address)
        socket.setsockopt(zmq.SUBSCRIBE, b'')

        self._socket = socket

    def reset_timeout(self):
        self._socket.setsockopt(zmq.LINGER, -1)
        self._socket.setsockopt(zmq.RCVTIMEO, -1)

    def set_timeout(self, timeout):
        # assert timeout > 0
        self._socket.setsockopt(zmq.LINGER, 0)
        self._socket.setsockopt(zmq.RCVTIMEO, timeout)

    def subscribe(self, *, timeout=1000):
        self.reset_timeout()

        num_received = 0
        recv_once = False
        # outputs = []
        while True:
            # output = output_pb2.OutputMessage()
            try:
                msg = self._socket.recv()

                # Remove points
                output = output_pb2.OutputMessage()
                output.ParseFromString(msg)
                output.ground_points = b''
                output.background_points = b''
                for obj in output.objects:
                    obj.points = b''
                del output.region_of_interest[:]
    
                output_fn = os.path.join(self._output_dir, f'{num_received:09}.bin')
                with open(output_fn, 'wb') as fp:
                    fp.write(output.SerializeToString())

                if not recv_once:
                    recv_once = True
                    self.set_timeout(timeout)
                    sys.stdout.flush()

                num_received += 1
            except zmq.Again:
                break

        print('No message received for {} ms, stopping collection of messages'.format(timeout))

        return num_received

def main():
    output_dir = 'tmp_output'
    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)

    print('Dumping SENSR output to {}...'.format(output_dir))
    msg_rcv = MessageReceiver(output_dir)
    num_received = msg_rcv.subscribe(timeout=-1)
    print('Finished dumping {} messages from SENSR.'.format(num_received))

if __name__ == '__main__':
    main()