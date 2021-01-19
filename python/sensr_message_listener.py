import os
import sys
import websockets
import asyncio

from sensr_proto.output_pb2 import OutputMessage
from sensr_proto.point_cloud_pb2 import PointResult


class MessageListener:
    def __init__(self, output_dir, address, max_message=10):
        self._output_dir = output_dir
        self._address = address
        self._max_message = max_message
        self._num_received = 0

    def __del__(self):
        print('Finished dumping {} messages from SENSR.'.format(self._num_received))

    async def _output_stream(self):
        async with websockets.connect(self._address) as websocket:
            while self._num_received < self._max_message:
                output_message = await websocket.recv() # Receive output messages from SENSR
                output_fn = os.path.join(self._output_dir, f'frame_{self._num_received:06}.bin')
                with open(output_fn, 'wb') as fp:
                    fp.write(output_message)
                    self._num_received += 1
                    print('Dumping {} messages from SENSR.'.format(self._num_received))

    def connect(self):
        print('Dumping SENSR output to {}...'.format(self._output_dir))    
        
        asyncio.get_event_loop().run_until_complete(self._output_stream())



def main():

    output_dir = 'sample_output'
    address = "ws://localhost:5050"

    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)

    listener = MessageListener(output_dir, address)
    listener.connect()
    

if __name__ == '__main__':
    main()