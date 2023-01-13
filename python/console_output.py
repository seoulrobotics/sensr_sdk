from sensr_message_listener import MessageListener, ListenerType
import sensr_proto.output_pb2 as sensr_output
import sensr_proto.point_cloud_pb2 as sensr_pcloud
import sensr_proto.type_pb2 as sensr_type

import google.protobuf.timestamp_pb2
import ctypes
import argparse
import signal
import numpy as np
from datetime import datetime

class ZoneEvenListener(MessageListener):

    def __init__(self, address):
        super().__init__(address=address, 
                         listener_type=ListenerType.OUTPUT_MESSAGE)
    
    def _on_error(self, message):
        self.reconnect()

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('event'):
            for zone_event in message.event.zone:
                print(f"event {zone_event.type}")
                print(f"object {zone_event.object.id}")
                # if zone_event.type == sensr_output.ZoneEvent.Type.ENTRY:
                #     print('Entering zone ({0}) : obj ({1}) '.format(zone_event.id, zone_event.object.id))
                # elif zone_event.type == sensr_output.ZoneEvent.Type.EXIT:
                #     print('Exiting zone ({0}) : obj ({1}) '.format(zone_event.id, zone_event.object.id))


class PointResultListener(MessageListener):

    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.POINT_RESULT)
    
    def _on_error(self, message):
        self.reconnect()

    def _on_get_point_result(self, message):
        assert isinstance(message, sensr_pcloud.PointResult), "message should be of type PointResult"
        
        for point_cloud in message.points:
            float_size = ctypes.sizeof(ctypes.c_float)
            num_points = len(point_cloud.points) // (float_size * 3) # Each point is 3 floats (x,y,z)
            
            intensity_np = np.frombuffer(point_cloud.intensities, np.float32)
            
            if len(intensity_np) != 0:
                min_intensity = np.min(intensity_np)
                median_intensity = np.median(intensity_np)
                max_intensity = np.max(intensity_np)
            else:
                min_intensity = 0.0
                median_intensity = 0.0
                max_intensity = 0.0

            if point_cloud.type == sensr_pcloud.PointResult.PointCloud.Type.RAW:
                print('Topic ({0}) no. of points - {1}. Min and max intensity is [{2}, {3}]'.format(point_cloud.id, num_points, min_intensity, max_intensity))
                print('Intensity [min, median, max] is [{0}, {1}, {2}]'.format(min_intensity, median_intensity, max_intensity))
            elif point_cloud.type == sensr_pcloud.PointResult.PointCloud.Type.GROUND:
                print('Ground points no. of points - {0}.'.format(num_points))
                print('Intensity [min, median, max] is [{0}, {1}, {2}]'.format(min_intensity, median_intensity, max_intensity))
            elif point_cloud.type == sensr_pcloud.PointResult.PointCloud.Type.BACKGROUND:
                print('Environment points no. of points - {0}'.format(num_points))
                print('Point intensity [min, median, max] is [{0}, {1}, {2}]'.format(min_intensity, median_intensity, max_intensity))


class ObjectListener(MessageListener):
    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE)

    def _on_error(self, message):
        self.reconnect()

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"
        if message.HasField('stream') and message.stream.has_objects:
            for obj in message.stream.objects:
                float_size = ctypes.sizeof(ctypes.c_float)
                object_point_num = len(obj.points) // (float_size * 3) # Each point is 3 floats (x,y,z)

                intensity_np = np.frombuffer(obj.intensities, np.float32)
                
                if len(intensity_np) != 0:
                    min_intensity = np.min(intensity_np)
                    median_intensity = np.median(intensity_np)
                    max_intensity = np.max(intensity_np)
                else:
                    min_intensity = 0.0
                    median_intensity = 0.0
                    max_intensity = 0.0
                
                print(f"Obj {obj.id} Type: {sensr_type.LabelType.Name(int(obj.label))} ")
                print(f"Obj {obj.id} Traking Status: {sensr_type.TrackingStatus.Name(int(obj.tracking_status))}")
                print(f"Obj {obj.id} Point Intensity {intensity_np}") 
                print(f"Obj {obj.id} Velocity X: {obj.velocity.x}, Y: {obj.velocity.y}")
                print(f"Obj {obj.id} Bbox Position X: {obj.bbox.position.x}, Y: {obj.bbox.position.y}, Z: {obj.bbox.position.z}")
                print(f"Obj {obj.id} Size X: {obj.bbox.size.x}, Y: {obj.bbox.size.y}, Z: {obj.bbox.size.z}")
                print(f"Obj {obj.id} Yaw angle {obj.yaw_rate}")
                print()



class HealthListener(MessageListener):

    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE)
    
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
    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE)
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
    return parser.parse_args()

current_listner = None

def signal_handler(sig, frame):
    if current_listner is not None:
        current_listner.disconnect()

if __name__ == "__main__":
    
    args = parse_arguments()

    address = args.address
    example_type = args.example_type
    
    if example_type == "zone":
        current_listner = ZoneEvenListener(address)
    elif example_type == "point":
        current_listner = PointResultListener(address)
    elif example_type == "object":
        current_listner = ObjectListener(address)
    elif example_type == "health":
        current_listner = HealthListener(address)
    elif example_type == "time":
        current_listner = TimeChecker(address)
    else:
        print("Unrecognized example type")
    if current_listner is not None:
        signal.signal(signal.SIGINT, signal_handler)
        current_listner.connect()
