from sensr_message_listener import MessageListener, ListenerType
import sensr_proto.output_pb2 as sensr_output
import sensr_proto.point_cloud_pb2 as sensr_pcloud
import sensr_proto.type_pb2 as sensr_type

import google.protobuf.timestamp_pb2
import ctypes
import argparse
import signal
import os
from pathlib import Path

home = str(Path.home())
cert_file_path = os.path.join(home, "seoulrobotics", "keys", "sensr-ca.crt")
class ZoneEvenListener(MessageListener):

    def __init__(self, address, output_port, point_port):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE,
                         use_ssl=True,
                         crt_file_path=cert_file_path,
                         output_port=output_port,
                         point_port=point_port)

    def _on_error(self, message):
        self.reconnect()

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('event'):
            for zone_event in message.event.zone:
                if zone_event.type == sensr_output.ZoneEvent.Type.ENTRY:
                    print('Entering zone ({0}) : obj ({1}) '.format(zone_event.id, zone_event.object.id))
                elif zone_event.type == sensr_output.ZoneEvent.Type.EXIT:
                    print('Exiting zone ({0}) : obj ({1}) '.format(zone_event.id, zone_event.object.id))


class PointResultListener(MessageListener):

    def __init__(self, address, output_port, point_port):
        super().__init__(address=address,
                         listener_type=ListenerType.POINT_RESULT,
                         use_ssl=True,
                         crt_file_path=cert_file_path,
                         output_port=output_port,
                         point_port=point_port)

    def _on_error(self, message):
        self.reconnect()

    def _on_get_point_result(self, message):
        assert isinstance(message, sensr_pcloud.PointResult), "message should be of type PointResult"
        
        for point_cloud in message.points:
            float_size = ctypes.sizeof(ctypes.c_float)
            num_points = len(point_cloud.points) // (float_size * 3) # Each point is 3 floats (x,y,z)

            if point_cloud.type == sensr_pcloud.PointResult.PointCloud.Type.RAW:
                print('Topic ({0}) no. of points - {1}'.format(point_cloud.id, num_points))
            elif point_cloud.type == sensr_pcloud.PointResult.PointCloud.Type.GROUND:
                print('Ground points no. of points - {0}'.format(num_points))
            elif point_cloud.type == sensr_pcloud.PointResult.PointCloud.Type.BACKGROUND:
                print('Environment points no. of points - {0}'.format(num_points))


class ObjectListener(MessageListener):

    def __init__(self, address, output_port, point_port):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE,
                         use_ssl=True,
                         crt_file_path=cert_file_path,
                         output_port=output_port,
                         point_port=point_port)

    def _on_error(self, message):
        self.reconnect()

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('stream'):
            for obj in message.stream.objects:
                float_size = ctypes.sizeof(ctypes.c_float)
                object_point_num = len(obj.points) // (float_size * 3) # Each point is 3 floats (x,y,z)

                print('Obj ({0}): point no. {1}'.format(obj.id, object_point_num))
                print('Obj ({0}): velocity {1}'.format(obj.id, obj.velocity))
                print('Obj ({0}): bbox {1}'.format(obj.id, obj.bbox))
                print('Obj ({0}): tracking status {1}'.format(obj.id, sensr_type.TrackingStatus.Name(int(obj.tracking_status))))
                print('Obj ({0}): Object type {1}'.format(obj.id, sensr_type.LabelType.Name(int(obj.label))))
                print('Obj ({0}): prediction {1}'.format(obj.id, obj.prediction))


class HealthListener(MessageListener):

    def __init__(self, address, output_port, point_port):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE,
                         use_ssl=True,
                         crt_file_path=cert_file_path,
                         output_port=output_port,
                         point_port=point_port)

    def _on_error(self, message):
        self.reconnect()

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('stream') and message.stream.HasField('health'):
            
            system_health = message.stream.health
            print('System health: {0}'.format(system_health.master))

            if len(system_health.nodes) > 0:
                for node_key in system_health.nodes:
                    node_health = system_health.nodes[node_key]
                    print('  Node ({0}) health: {1}'.format(node_key, node_health.status))

                    if len(node_health.sensors) > 0:
                        for sensor_key in node_health.sensors:
                            sensor_health = node_health.sensors[sensor_key]
                            print('    Sensor ({0}) health: {1}'.format(sensor_key, sensor_health))
                    else:
                        print('    No sensors are connected')
            else:
                print('  No nodes are connected')


class TimeChecker(MessageListener):
    def __init__(self, address, output_port, point_port):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE,
                         use_ssl=True,
                         crt_file_path=cert_file_path,
                         output_port=output_port,
                         point_port=point_port)

    def _on_error(self, message):
        self.reconnect()

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        current_time = google.protobuf.timestamp_pb2.Timestamp()
        current_time.GetCurrentTime()
        time_diff = current_time.ToMilliseconds() - message.timestamp.ToMilliseconds()

        print('Diff: {0} ms'.format(time_diff))


def parse_arguments():
    parser = argparse.ArgumentParser(description='Sample code for the SENSR Python SDK.')
    parser.add_argument('--address', type=str, default='localhost')
    parser.add_argument('--example_type', type=str, default="zone", 
                help='Has to be set to one of the following: \"zone\", \"point\", \"object\", \"health\", \"time\".')
    parser.add_argument("--output_port", type=str, default="5450")
    parser.add_argument("--point_port", type=str, default="5451")
    return parser.parse_args()

current_listner = None

def signal_handler(sig, frame):
    if current_listner is not None:
        current_listner.disconnect()

if __name__ == "__main__":
    
    args = parse_arguments()

    address = args.address
    example_type = args.example_type
    output_port = args.output_port
    point_port = args.point_port

    if example_type == "zone":
        current_listner = ZoneEvenListener(address, output_port, point_port)
    elif example_type == "point":
        current_listner = PointResultListener(address, output_port, point_port)
    elif example_type == "object":
        current_listner = ObjectListener(address, output_port, point_port)
    elif example_type == "health":
        current_listner = HealthListener(address, output_port, point_port)
    elif example_type == "time":
        current_listner = TimeChecker(address, output_port, point_port)
    else:
        print("Unrecognized example type")
    if current_listner is not None:
        signal.signal(signal.SIGINT, signal_handler)
        current_listner.connect()