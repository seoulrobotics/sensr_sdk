import os
import sys
import websockets
import asyncio
from enum import Enum

from sensr_proto.output_pb2 import OutputMessage
from sensr_proto.point_cloud_pb2 import PointResult

class ListenerType(Enum):
    NONE = 0
    OUTPUT_MESSAGE = 1
    POINT_RESULT = 2
    BOTH = 3

class MessageListener:
    def __init__(self, address, output_dir, listener_type=ListenerType.BOTH, max_message=10):
        self._output_dir = output_dir
        self._address = address
        self._max_message = max_message
        self._num_received = 0
        self._listener_type = listener_type

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

    def on_get_output_message(self):
        # TODO
        pass

    def on_get_point_result(self):
        # TODO
        pass



def main():

    output_dir = 'sample_output'
    address = "ws://localhost:5050"

    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)

    listener = MessageListener(address, output_dir)
    listener.connect()
    

if __name__ == '__main__':
    main()