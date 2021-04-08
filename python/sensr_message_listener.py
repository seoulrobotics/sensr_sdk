import websockets
import asyncio
import ssl
import pathlib
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
    def __init__(self, 
                 address="localhost", 
                 listener_type=ListenerType.BOTH, 
                 output_port = "5050", 
                 point_port = "5051"):
        self._address = "wss://" + address
        self._output_address = self._address + ':' + output_port
        self._point_address = self._address + ':' + point_port
        self._listener_type = listener_type
        self._ssl_context = ssl.SSLContext(ssl.PROTOCOL_SSLv23)
        self._ssl_context.load_verify_locations("../keys/sensr-sdk-ca.crt")

    def is_output_message_listening(self):
        return self._listener_type == ListenerType.OUTPUT_MESSAGE or self._listener_type == ListenerType.BOTH

    def is_point_result_listening(self):
        return self._listener_type == ListenerType.POINT_RESULT or self._listener_type == ListenerType.BOTH

    async def _output_stream(self):
        async with websockets.connect(self._output_address, ssl=self._ssl_context, max_size=None) as websocket:
            while self._is_running:
                message = await websocket.recv() # Receive output messages from SENSR
                
                output = OutputMessage()
                output.ParseFromString(message)
                self._on_get_output_message(output)
    

    async def _point_stream(self):
        async with websockets.connect(self._point_address, ssl=self._ssl_context, max_size=None) as websocket:
            while self._is_running:
                message = await websocket.recv() # Receive output messages from SENSR

                points = PointResult()
                points.ParseFromString(message)
                self._on_get_point_result(points)


    def connect(self):
        print('Receiving SENSR output from {}...'.format(self._address)) 
        
        self._loop = asyncio.get_event_loop()
        self._is_running = True

        if self.is_output_message_listening():
            self._loop.create_task(self._output_stream())

        if self.is_point_result_listening():
            self._loop.create_task(self._point_stream())

        self._loop.run_forever()
    
    def disconnect(self):
        self._is_running = False
        if self._loop is not None:
            self._loop.stop()

    def _on_get_output_message(self, message):
        raise Exception('on_get_output_message() needs to be implemented in the derived class')

    def _on_get_point_result(self, message):
        raise Exception('on_get_point_result() needs to be implemented in the derived class')

