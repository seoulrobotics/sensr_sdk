import os
import sys
import tqdm
import zmq
import zipfile
import shutil

import output_pb2

def zipdir(path, ziph):
    for root, dirs, files in os.walk(path):
        for file in files:
            ziph.write(os.path.join(root, file))

class MessageReceiver(object):
    def __init__(self, output_dir, files_per_zip=10000, address='localhost'):
        self._output_dir = output_dir
        self._files_per_zip = files_per_zip
        self._context = zmq.Context()

        socket = self._context.socket(zmq.SUB)
        self._current_address = f"tcp://{address}:5050"
        print('Connecting to {}'.format(self._current_address))
        socket.connect(self._current_address)
        socket.setsockopt(zmq.SUBSCRIBE, b'')

        self._temp_dirs = ['tmp1', 'tmp2']
        self._active_temp_dir = 0
        for temp_dir in self._temp_dirs:
            if not os.path.isdir(temp_dir):
                os.makedirs(temp_dir)

        self._socket = socket

    def reset_timeout(self):
        self._socket.setsockopt(zmq.LINGER, -1)
        self._socket.setsockopt(zmq.RCVTIMEO, -1)

    def set_timeout(self, timeout):
        self._socket.setsockopt(zmq.LINGER, 0)
        self._socket.setsockopt(zmq.RCVTIMEO, timeout)

    def swap_temp_folders(self):
        self._active_temp_dir = (self._active_temp_dir + 1) % 2
    
    def active_folder(self):
        return self._temp_dirs[self._active_temp_dir]

    def inactive_folder(self):
        return self._temp_dirs[(self._active_temp_dir+1)%2]

    def clear_inactive_folder(self):
        shutil.rmtree(self.inactive_folder())
        if not os.path.isdir(self.inactive_folder()):
            os.makedirs(self.inactive_folder())

    def zip_to_output_folder(self, zip_fname):
        zip_path = os.path.join(self._output_dir, zip_fname)
        zipf = zipfile.ZipFile(zip_path, 'w', zipfile.ZIP_DEFLATED)
        zipdir(self.inactive_folder(), zipf)
        zipf.close()
        return

    def subscribe(self, *, timeout=1000):
        self.reset_timeout()

        num_received = 0
        num_since_swap = 0
        num_zips = 0
        recv_once = False
        while True:
            try:
                msg = self._socket.recv()

                # Remove points
                output = output_pb2.OutputMessage()
                output.ParseFromString(msg)
                output.ground_points = b''
                output.background_points = b''
                for obj in output.objects:
                    obj.points = b''
                del output.region_of_interest[:]
    
                output_fn = os.path.join(self.active_folder(), f'{num_received:09}.bin')
                with open(output_fn, 'wb') as fp:
                    fp.write(output.SerializeToString())

                if not recv_once:
                    recv_once = True
                    self.set_timeout(timeout)
                    sys.stdout.flush()

                num_received += 1
                num_since_swap += 1

                if (num_since_swap > self._files_per_zip-1):
                    self.swap_temp_folders()
                    print(f'Compressing output to zip file output_zip_{num_zips:09}.zip.')
                    self.zip_to_output_folder(f'output_zip_{num_zips:09}.zip')
                    self.clear_inactive_folder()
                    num_zips += 1
                    num_since_swap = 0
                
                if (num_received%10000 == 0):
                    print(f'Recieved {num_received} messages overall.')

            except zmq.Again:
                break

        print('No message received for {} ms, stopping collection of messages'.format(timeout))

        return num_received

def main():
    output_dir = 'sample_output'
    if not os.path.isdir(output_dir):
        os.makedirs(output_dir)

    print('Dumping SENSR output to {}...'.format(output_dir))
    msg_rcv = MessageReceiver(output_dir, files_per_zip=10000)
    num_received = msg_rcv.subscribe(timeout=-1)
    print('Finished dumping {} messages from SENSR.'.format(num_received))

if __name__ == '__main__':
    main()