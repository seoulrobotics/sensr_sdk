from sensr_message_listener import MessageListener, ListenerType
import sensr_proto.output_pb2 as sensr_output
import sensr_proto.point_cloud_pb2 as sensr_pcloud

import google.protobuf.timestamp_pb2
import sys
import ctypes



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
                if zone_event.type == sensr_output.ZoneEvent.Type.EXIT:
                    print('Exiting zone ({0}) : obj ({1}) '.format(zone_event.id, zone_event.object.id))


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
                print('Topic ({0}) no. of points - {1}'.format(point_cloud.id(), num_points))
            elif point_cloud.type == sensr_pcloud.PointResult.PointCloud.Type.GROUND:
                print('Ground points no. of points - {0}'.format(num_points))
            elif point_cloud.type == sensr_pcloud.PointResult.PointCloud.Type.ENVIRONMENT:
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


class HealthListener(MessageListener):

    def __init__(self,address):
        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE)

    def _on_get_output_message(self, message):
        assert isinstance(message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('stream') and message.stream.HasField('health'):
            
            system_health = message.stream.health
            print('System health: {0}'.format(system_health.master))

            if len(system_health.nodes) > 0:
                for node_key in system_health.nodes:
                    node_health = system_health.nodes[node_key]
                    print('Node ({0}) health: {1}'.format(node_key, node_health.status))

                    if len(node_health.sensors) > 0:
                        for sensor_key in node_health.sensors:
                            sensor_health = node_health.sensors[sensor_key]
                            print('Sensor ({0}) health: {1}'.format(sensor_key, sensor_health))
                    else:
                        print('No sensors are connected')
            else:
                print('No nodes are connected')


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


if __name__ == "__main__":
    
    address = "localhost"
    example_type = "zone"
    
    if example_type == "zone":
        zone_listener = ZoneEvenListener(address)
        zone_listener.connect()
    elif example_type == "point":
        point_listener = PointResultListener(address)
        point_listener.connect()
    elif example_type == "object":
        object_listener = ObjectListener(address)
        object_listener.connect()
    elif example_type == "health":
        health_listener = HealthListener(address)
        health_listener.connect()
    elif example_type == "time":
        time_checker = TimeChecker(address)
        time_checker.connect()
    else:
        print("Unrecognized example type")