import os
import sys
import websockets
import asyncio
from enum import Enum
from abc import ABCMeta, abstractmethod # Abstract base classes

from sensr_proto.output_pb2 import OutputMessage
from sensr_proto.point_cloud_pb2 import PointResult

class ListenerType(Enum):
    NONE = 0
    OUTPUT_MESSAGE = 1
    POINT_RESULT = 2
    BOTH = 3

class MessageListener(metaclass=ABCMeta):
    
    @abstractmethod
    def __init__(self, address, listener_type=ListenerType.BOTH, output_port = "5050", point_port = "5051", max_message=5):
        self._address = "ws://" + address
        self._output_address = self._address + ':' + output_port
        self._point_address = self._address + ':' + point_port
        self._listener_type = listener_type

        self._max_message = max_message
        self._num_received = 0

    def __del__(self):
        print('Finished receiving {} messages from SENSR.'.format(self._num_received))


    async def _output_stream(self):
        async with websockets.connect(self._output_address) as websocket:
            while self._num_received < self._max_message:
                message = await websocket.recv() # Receive output messages from SENSR
                self._num_received += 1
                print('Received {} output messages from SENSR.'.format(self._num_received))

                output = OutputMessage()
                output.ParseFromString(message)
                self._on_get_output_message(output)
    

    async def _point_stream(self):
        async with websockets.connect(self._point_address) as websocket:
            while self._num_received < self._max_message:
                message = await websocket.recv() # Receive output messages from SENSR
                self._num_received += 1
                print('Received {} point messages from SENSR.'.format(self._num_received))

                points = PointResult()
                points.ParseFromString(message)
                self._on_get_point_result(points)


    def connect(self):
        print('Receiving SENSR output from {}...'.format(self._address))    
        
        if self._listener_type == ListenerType.OUTPUT_MESSAGE:
            asyncio.get_event_loop().run_until_complete(self._output_stream())

        if self._listener_type == ListenerType.POINT_RESULT:
            asyncio.get_event_loop().run_until_complete(self._point_stream())


    def _on_get_output_message(self, message):
        if (self._listener_type == ListenerType.OUTPUT_MESSAGE or self._listener_type == ListenerType.BOTH):
            raise Exception('on_get_output_message() needs to be implemented in the derived class')

    def _on_get_point_result(self, message):
        if (self._listener_type == ListenerType.POINT_RESULT or self._listener_type == ListenerType.BOTH):
            raise Exception('on_get_point_result() needs to be implemented in the derived class')

