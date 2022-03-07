import websockets
import asyncio
import ssl
import os
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
                 point_port = "5051",
                 use_ssl=False,
                 crt_file_path=""):
        protocol = 'ws'
        if use_ssl:
            protocol = 'wss'
            self._ssl_context = ssl.SSLContext(ssl.PROTOCOL_SSLv23)
            self._ssl_context.verify_mode = ssl.CERT_REQUIRED
            assert os.path.exists(crt_file_path), "Please indicate a valid certificate file."
            self._ssl_context.load_verify_locations(crt_file_path)
        else:
            self._ssl_context = None
        self._address = f"{protocol}://{address}"
        self._output_address = self._address + ':' + output_port
        self._point_address = self._address + ':' + point_port
        self._listener_type = listener_type
        self._output_ws = None
        self._point_ws = None
        

    def is_output_message_listening(self):
        return self._listener_type == ListenerType.OUTPUT_MESSAGE or self._listener_type == ListenerType.BOTH

    def is_point_result_listening(self):
        return self._listener_type == ListenerType.POINT_RESULT or self._listener_type == ListenerType.BOTH
    
    async def _output_stream(self):
        async with websockets.connect(self._output_address, ssl=self._ssl_context, compression=None, max_size=None, ping_interval=None) as websocket:
            self._output_ws = websocket
            while not self._output_ws.closed:
                try:
                    message = await asyncio.wait_for(websocket.recv(), timeout=1.0) # Receive output messages from SENSR                    
                    output = OutputMessage()
                    output.ParseFromString(message)
                    self._on_get_output_message(output)
                except asyncio.TimeoutError:
                    pass

    async def _point_stream(self):
        async with websockets.connect(self._point_address, ssl=self._ssl_context, compression=None, max_size=None, ping_interval=None) as websocket:
            self._point_ws = websocket
            while not self._point_ws.closed:
                try:
                    message = await asyncio.wait_for(websocket.recv(), timeout=1.0) # Receive output messages from SENSR
                    points = PointResult()
                    points.ParseFromString(message)
                    self._on_get_point_result(points)
                except asyncio.TimeoutError:
                    pass
    
    
    async def _main(self):
        # Initialize
        await asyncio.sleep(1)
        # Main loop (_output_stream and _point_stream)
        while self._is_running:
            await asyncio.sleep(0.1)
        # Finalize
        if self.is_output_message_listening():
            await self._output_ws.close()
        if self.is_point_result_listening():
            await self._point_ws.close()
        #self._is_running = False
        self._loop.stop()


    def connect(self):
        print('Receiving SENSR output from {}...'.format(self._address))
        try:
            self._loop = asyncio.get_event_loop()
        except asyncio.RuntimeError:
            print('Fail to create event loop')
            return False
        self._is_running = True

        if self.is_output_message_listening():
            self._loop.create_task(self._output_stream())

        if self.is_point_result_listening():
            self._loop.create_task(self._point_stream())

        self._loop.create_task(self._main())
        self._loop.run_forever()
        self._loop = None
        return True
    
    def disconnect(self):
        self._is_running = False

    def _on_get_output_message(self, message):
        raise Exception('on_get_output_message() needs to be implemented in the derived class')

    def _on_get_point_result(self, message):
        raise Exception('on_get_point_result() needs to be implemented in the derived class')

