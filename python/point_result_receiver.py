import os
import sys
import tqdm
import asyncio
import websockets
import sensr_proto.point_cloud_pb2

async def update_point_result():
    address = 'localhost'
    uri = f"ws://{address}:5051"
    async with websockets.connect(uri) as websocket:
        while True:
            greeting = await websocket.recv()
            point_result = sensr_proto.point_cloud_pb2.PointResult()
            point_result.ParseFromString(greeting)
            for pc in point_result.points:
                print(f'PointCloud({pc.type}) : {pc.id}')

asyncio.get_event_loop().run_until_complete(update_point_result())

def main():
    update_point_result()

if __name__ == '__main__':
    main()