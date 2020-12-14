import os
import sys
import tqdm
import asyncio
import websockets
import sensr_proto.output_pb2

async def update_output_message():
    address = 'localhost'
    uri = f"ws://{address}:5050"
    async with websockets.connect(uri) as websocket:
        while True:
            greeting = await websocket.recv()
            output = sensr_proto.output_pb2.OutputMessage()
            output.ParseFromString(greeting)
            # if output.stream:
            #     for obj in output.stream.objects:
            #         print(f'Object #{obj.id}')
            #         print(obj.bbox)

            if output.event:
                for zone_event in output.event.zone:
                    print(zone_event)
                    

asyncio.get_event_loop().run_until_complete(update_output_message())

def main():
    update_output_message()

if __name__ == '__main__':
    main()