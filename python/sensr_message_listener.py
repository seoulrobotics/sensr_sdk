import websockets
import asyncio
import ssl
import os
from enum import Enum
from abc import ABCMeta, abstractmethod # Abstract base classes
import certifi

from sensr_proto.output_pb2 import OutputMessage, SystemHealth
from sensr_proto.point_cloud_pb2 import PointResult

class ListenerType(Enum):
    NONE = 0
    OUTPUT_MESSAGE = 1
    POINT_RESULT = 2
    BOTH = 3


class MessageListener(metaclass=ABCMeta):
    class State(Enum):
        READY = 0
        RUNNING = 1
        STOP_REQUESTED = 2
        STOPPED = 3
    
    @abstractmethod
    def __init__(self, 
                 address="localhost", 
                 listener_type=ListenerType.BOTH, 
                 output_port = "5050", 
                 point_port = "5051",
                 use_ssl=False,
                 crt_file_path=""):
        # use wss protocol
        if address.startswith("wss") or address.startswith("https"):
            if os.path.exists(certifi.where()):
                crt_file_path = certifi.where()
                self._ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
                self._ssl_context.load_verify_locations(crt_file_path)
                self._ssl_context.verify_mode = ssl.CERT_REQUIRED
            else:
                self._ssl_context = ssl.SSLContext(ssl.PROTOCO_TLS)

            if address.startswith("https"):
                address = address.replace("https", "wss")
        # use ws protocol
        elif address.startswith("ws"):
            self._ssl_context = None
        # use ws protocol
        else:
            self._ssl_context = None
            if address.startswith("http"):
                address = address.replace("http", "ws")
            else:
                address = "ws://" + address

        self._address = address
        self._output_address = self._address + ":" + output_port
        self._point_address = self._address + ":" + point_port
        self._listener_type = listener_type
        self._output_ws = None
        self._point_ws = None
        self._state = MessageListener.State.STOPPED
        

    def is_output_message_listening(self):
        return self._listener_type == ListenerType.OUTPUT_MESSAGE or self._listener_type == ListenerType.BOTH

    def is_point_result_listening(self):
        return self._listener_type == ListenerType.POINT_RESULT or self._listener_type == ListenerType.BOTH
    
    def check_oveflow_error(self, output):
        if output.HasField('event') and output.event.HasField('health'):
            if output.event.health.master == SystemHealth.Status.OUTPUT_BUFFER_OVERFLOW:
                return True
        return False

    async def _output_stream(self):
       while self._state != MessageListener.State.STOPPED and self._state != MessageListener.State.STOP_REQUESTED:
            async with websockets.connect(self._output_address, ssl=self._ssl_context, compression=None, max_size=None, ping_interval=None) as websocket:
                if self._output_ws != websocket:
                    self._output_ws = websocket
                while self._output_ws is not None and not self._output_ws.closed:
                    try:
                        message = await asyncio.wait_for(self._output_ws.recv(), timeout=1.0) # Receive output messages from SENSR                    
                        output = OutputMessage()
                        output.ParseFromString(message)
                        if self.check_oveflow_error(output):
                            self._on_error("Output Buffer Overflow.")
                        self._on_get_output_message(output)
                    except asyncio.TimeoutError:
                        pass
                    except websockets.ConnectionClosedOK:
                        pass
                    except websockets.ConnectionClosedError:
                        self._on_error("Closed by error.")

    async def _point_stream(self):
        while self._state != MessageListener.State.STOPPED and self._state != MessageListener.State.STOP_REQUESTED:
            async with websockets.connect(self._point_address, ssl=self._ssl_context, compression=None, max_size=None, ping_interval=None) as websocket:
                if self._point_ws != websocket:
                    self._point_ws = websocket
                while not self._point_ws.closed:
                    try:
                        message = await asyncio.wait_for(self._point_ws.recv(), timeout=1.0) # Receive output messages from SENSR
                        points = PointResult()
                        points.ParseFromString(message)
                        self._on_get_point_result(points)
                    except asyncio.TimeoutError:
                        pass
                    except websockets.ConnectionClosedOK:
                        pass
                    except websockets.ConnectionClosedError:
                        self._on_error("Closed by error.")

    async def _main(self):
        while self._state != MessageListener.State.STOPPED:
            if self._state == MessageListener.State.STOP_REQUESTED:
                await self.close_connection()
                self._state = MessageListener.State.STOPPED
            elif self._state == MessageListener.State.READY:
                await self.close_connection()
                self._state = MessageListener.State.RUNNING
            else:
                await asyncio.sleep(0.1)
        
        # Finalize
        self._loop.stop()

    async def close_connection(self):
        if self._output_ws != None and self.is_output_message_listening():
            await self._output_ws.close()
            self._output_ws = None
        if self._point_ws != None and self.is_point_result_listening():
            await self._point_ws.close()
            self._point_ws = None

    def connect(self):
        if self._state == MessageListener.State.STOPPED:
            print('Receiving SENSR output from {}...'.format(self._address))
            try:
                self._loop = asyncio.get_event_loop()
            except asyncio.RuntimeError:
                print('Fail to create event loop')
                return False
            self._state = MessageListener.State.READY

            if self.is_output_message_listening():
                self._loop.create_task(self._output_stream())

            if self.is_point_result_listening():
                self._loop.create_task(self._point_stream())

            self._loop.create_task(self._main())
            self._loop.run_forever()
            return True
        return False
    
    def disconnect(self):
        self._state = MessageListener.State.STOP_REQUESTED
    
    def reconnect(self):
        self._state = MessageListener.State.READY

    def _on_get_output_message(self, message):
        raise Exception('on_get_output_message() needs to be implemented in the derived class')
    
    def _on_error(self, message):
        raise Exception('on_error() needs to be implemented in the derived class')

    def _on_get_point_result(self, message):
        raise Exception('on_get_point_result() needs to be implemented in the derived class')