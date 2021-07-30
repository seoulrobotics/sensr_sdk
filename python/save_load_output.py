from sensr_message_listener import MessageListener, ListenerType
import sensr_proto.output_pb2 as sensr_output
import sensr_proto.point_cloud_pb2 as sensr_pcloud

import google.protobuf.timestamp_pb2
import ctypes
import argparse
import os

class FileSaver(MessageListener):

    def __init__(self, address, dir):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE,
                         crt_file_path="../keys/sensr-sdk-ca.crt")
        self._dir = dir
        self._frame_count = 0
        if not os.path.exists(self._dir):
            os.mkdir(self._dir)
    
    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"
        
        output_fn = os.path.join(self._dir, f'{self._frame_count:09}.bin')
        with open(output_fn, 'wb') as fp:
            fp.write(message.SerializeToString())
        print('Saved {0} ({1} objects)'.format(output_fn, len(message.stream.objects)))
        self._frame_count += 1


class FileLoader():

    def __init__(self, dir):
        self._dir = dir
        self._frame_count = 0
    
    def run(self,):
        files = [f for f in os.listdir(self._dir) if os.path.splitext(f)[1] == '.bin']
        files.sort()
        for f in files:
            file_path = os.path.join(self._dir, f)
            with open(file_path, 'rb') as fp:
                message = sensr_output.OutputMessage()
                message.ParseFromString(fp.read())
                print('Loaded {0} ({1} objects)'.format(file_path, len(message.stream.objects)))


def parse_arguments():
    parser = argparse.ArgumentParser(description='Sample code for the SENSR Python SDK.')
    parser.add_argument('--mode', type=str, default='', help='Mode: \"save\" or \"load\"')
    parser.add_argument('--address', type=str, default='localhost', help='IP address of the SENSR computer')
    parser.add_argument('--dir', type=str, default='', help='Full path of the directory to save/load files')
    return parser.parse_args()


if __name__ == "__main__":
    
    args = parse_arguments()

    mode = args.mode
    address = args.address
    dir = args.dir
    
    if mode == "save":
        saver = FileSaver(address, dir)
        saver.connect()
    elif mode == "load":
        loader = FileLoader(dir)
        loader.run()
    else:
        print("Unrecognized mode")