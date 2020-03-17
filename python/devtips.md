# Development Tips

Here are some code snippets to understand how to work with SENSR.

## Communicate with SENSR

### Initialize ZMQ socket
```python
import zmq

context = zmq.Context()
socket = context.socket(zmq.SUB)

socket.connect(f"tcp://{address}:5050")
socket.setsockopt(zmq.SUBSCRIBE, b'')
```

### Receive output messages from SENSR
```python
# assume socket has been initialized

while True:
    msg = socket.recv()
```

### Convert an output message to a protobuf object
```python
import output_pb2

# assume socket has been initialized
msg = socket.recv()

output = output_pb2.OutputMessage()
output.ParseFromString(msg)
```

## Parse SENSR output message

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
