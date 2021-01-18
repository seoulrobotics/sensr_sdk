# Development Tips

Here are some code snippets to understand how to work with SENSR.

## Communicate with SENSR

### Initialize websocket and 
```python
import asyncio
import websockets
import output_pb2

async def output_stream():
    uri = "ws://localhost:5050"
    async with websockets.connect(uri) as websocket:
        output_message = await websocket.recv() # Receive output messages from SENSR
        output = output_pb2.OutputMessage()
        output.ParseFromString(output_message) # Convert output message to a protobuf object
        print(f"< {output}")
asyncio.get_event_loop().run_until_complete(output_stream())
```
### Dump the message into a file
```python
import output_pb2

output_msg = output_pb2.OutputMessage()

filename = 'output.bin'
with open(filename, 'wb') as fp:
    fp.write(output_msg.SerializeToString())
```

### Count each object class
```python
from labels_pb2 import LabelType
import output_pb2

obj_cnts = [0] * len(LabelType.items())

output_msg = output_pb2.OutputMessage()

for obj in output_msg.objects:
    obj_cnts[obj.label] += 1

for name, label in LabelType.items():
    print(f'{name}: {obj_cnts[label]}')
```
