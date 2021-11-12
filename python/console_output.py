from sensr_message_listener import MessageListener, ListenerType
import sensr_proto.output_pb2 as sensr_output
import sensr_proto.point_cloud_pb2 as sensr_pcloud
import sensr_proto.type_pb2 as sensr_type

import google.protobuf.timestamp_pb2
import ctypes
import argparse
import signal
import sys


class ZoneEvenListener(MessageListener):

    def __init__(self, address):
        super().__init__(address=address, 
                         listener_type=ListenerType.OUTPUT_MESSAGE)

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('event'):
            for zone_event in message.event.zone:
                if zone_event.type == sensr_output.ZoneEvent.Type.ENTRY:
                    print('Entering zone ({0}) : obj ({1}) '.format(zone_event.id, zone_event.object.id))
                elif zone_event.type == sensr_output.ZoneEvent.Type.EXIT:
                    print('Exiting zone ({0}) : obj ({1}) '.format(zone_event.id, zone_event.object.id))


class ResidentTime(MessageListener):

    def __init__(self, address, target_zone_id):
        self._resident_objs = {}
        self._target_zone_id = target_zone_id
        self._resident_avg = 0.0
        self._count = 0
        super().__init__(address=address, 
                         listener_type=ListenerType.OUTPUT_MESSAGE)
    
    def _cumulative_average (self, prevAvg, newNumber, listLength):
        oldWeight = (listLength - 1) / listLength
        newWeight = 1 / listLength
        return (prevAvg * oldWeight) + (newNumber * newWeight)

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('event'):
            for zone_event in message.event.zone:
                if zone_event.type == sensr_output.ZoneEvent.Type.ENTRY:
                    if zone_event.id == self._target_zone_id:
                        self._resident_objs[zone_event.object.id] = zone_event.timestamp
                elif zone_event.type == sensr_output.ZoneEvent.Type.EXIT:
                    start_time = self._resident_objs.get(zone_event.object.id)
                    if start_time != None:
                        # Calc resident time of an object in the desited zone
                        resident_time = zone_event.timestamp.ToMilliseconds() - start_time.ToMilliseconds()
                        self._count += 1
                        self._resident_avg = self._cumulative_average(self._resident_avg, resident_time, self._count)
                        del self._resident_objs[zone_event.object.id]
                        print("avg:" + str(self._resident_avg))
            for losing_event in message.event.losing:
                start_time = self._resident_objs.get(losing_event.id)
                if start_time != None:
                    # Calc resident time of an object in the desited zone
                    resident_time = message.event.losing.timestamp.ToMilliseconds() - start_time.ToMilliseconds()
                    # Calc avrage resident time of the desited zone
                    self._count += 1
                    self._resident_avg = self._cumulative_average(self._resident_avg, resident_time, self._count)
                    del self._resident_objs[losing_event.id]
                    print("avg:" + str(self._resident_avg))


class PointResultListener(MessageListener):

    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.POINT_RESULT)
    
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

    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE)

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

    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE)

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('stream') and message.stream.HasField('health'):
            
            system_health = message.stream.health
            #print('System health: {0}'.format(system_health.master))

            found_abnormal = False
            if not system_health.master == sensr_output.SystemHealth.Status.OK:
                print("Master node died.")
                found_abnormal = True
            else:
                print("Master node is alive.")
            for key, node in system_health.nodes.items():
                if not node.status == sensr_output.SystemHealth.Node.Status.OK:
                    print(f"Algo node({key}) died.")
                    found_abnormal = True
                else:
                    print(f"Algo node({key}) is alive.")            
            if found_abnormal:
                self.disconnect()


class TimeChecker(MessageListener):
    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE)

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
    elif example_type == "resident_time":
        current_listner = ResidentTime(address, 1001)
    else:
        print("Unrecognized example type")
    if current_listner is not None:
        signal.signal(signal.SIGINT, signal_handler)
        current_listner.connect()