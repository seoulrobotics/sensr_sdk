import os
import sys
import websockets
import asyncio

output_dir = 'sample_output'
num_received = 0
max_message = 1000


async def output_stream():
    uri = "ws://localhost:5050"
    async with websockets.connect(uri) as websocket:
        global num_received
        while num_received < max_message:
            output_message = await websocket.recv() # Receive output messages from SENSR
            output_fn = os.path.join(output_dir, f'frame_{num_received:06}.bin')
            with open(output_fn, 'wb') as fp:
                fp.write(output_message)
                num_received += 1
                print('Dumping {} messages from SENSR.'.format(num_received))

def main():
    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)

    print('Dumping SENSR output to {}...'.format(output_dir))    
    asyncio.get_event_loop().run_until_complete(output_stream())
    print('Finished dumping {} messages from SENSR.'.format(num_received))

if __name__ == '__main__':
    main()