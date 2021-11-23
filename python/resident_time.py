from sensr_message_listener import MessageListener, ListenerType
import sensr_proto.output_pb2 as sensr_output
import sensr_proto.type_pb2 as sensr_type

import argparse
import signal
import sys
import requests

import ctypes
import numpy as np
from io import BytesIO
import matplotlib.pyplot as plt
from datetime import datetime
from operator import attrgetter

def load_object_points(obj) -> np.ndarray:
    object_point_num = len(obj.points) // (ctypes.sizeof(ctypes.c_float) * 3) # Each point is 3 floats (x,y,z)
    return np.frombuffer(obj.points, dtype=np.float32).reshape(object_point_num, 3)
    
class DebugPlotter:

    @staticmethod
    def plot_scene(residents, event_name):
        if (len(residents) == 0):
            return

        fig = plt.figure()
        ax = fig.add_subplot(1, 1, 1)

        for _, resident in residents.items():
            DebugPlotter.plot_resident(ax, resident)

        ax.set_xlim([-10, 10])
        ax.set_ylim([-10, 10])
        
        now = datetime.now()
        dt_string = now.strftime("%d-%m-%Y_%H-%M-%S")
        plt.savefig("temp/" + event_name + dt_string + ".png", dpi=500)
        plt.close()

    @staticmethod
    def plot_resident(ax, resident):
        obj = resident._histories[-1]

        points_topview = load_object_points(obj)[:,:2]
        num_points = points_topview.shape[0]

        if (num_points == 0):
            return
        points_center = np.mean(points_topview, axis=0)

        DebugPlotter.plot_cluster(ax, points_topview)
        DebugPlotter.plot_travel(ax, resident)
        DebugPlotter.plot_text(ax, resident, points_center) 

        
    @staticmethod
    def plot_cluster(ax, points):
        ax.plot(points[:,0], points[:,1], '.', markersize = 1)

    @staticmethod
    def plot_travel(ax, resident):
        travel_path = np.array([[obj.bbox.position.x, obj.bbox.position.y]  for obj in resident._histories])

        ax.plot(travel_path[:,0], travel_path[:,1], color='k', linewidth=0.2)
    
    @staticmethod
    def plot_text(ax, resident, text_position):
        obj = resident._histories[-1]
        obj_stats = resident.current_stats


        travel_distance = resident.calculate_travel_distance()
        object_info_string = "{object_label}, id {obj_id}, height {height:.2f}, dims ({x_svd:.2f},{y_svd:.2f}), dim_ratio {ratio:.2f}, travel {travel:.2f}".format(
            obj_id = obj.id,
            object_label = sensr_type.LabelType.Name(int(obj.label)),
            height = obj.bbox.size.z,
            x_svd = obj_stats.dimensions[0],
            y_svd = obj_stats.dimensions[1],
            ratio = obj_stats.dimensions[0] / obj_stats.dimensions[1],
            travel = travel_distance)
        
        text_color = 'r' if resident.is_door() else 'k'
        ax.text(text_position[0], text_position[1], object_info_string, fontsize=2.0, alpha=0.75, color=text_color)

    


        


class RESTAPI:
    def __init__(self, address):
        self._server_cert_file_path = False
        self._client_cert_file_path = ""
        self._address = address

    def _handle_request(self, resource, rest_cmd, **kargs):
        ssl = False if kargs.get('ssl') == None else kargs.get('ssl')
        kargs.pop('ssl', None)
        check_response = True if kargs.get(
            'check_response') == None else kargs.get('check_response')
        kargs.pop('check_response', None)
        protocol = 'https' if ssl else 'http'
        url = f"{protocol}://{self._address}:9080/" + resource
        response = rest_cmd(url, **kargs)
        if check_response:
            self._response_checker(response)
        return response

    def _response_checker(self, response):
        if response.status_code != 200:
            print(response.text)
            print('##Fail##')
            sys.exit(1)

    def put_request(self, resource, **kargs):
        kargs['verify'] = self._server_cert_file_path
        return self._handle_request(resource, requests.put, **kargs)

    def delete_request(self, resource, **kargs):
        kargs['verify'] = self._server_cert_file_path
        return self._handle_request(resource, requests.delete, **kargs)

    def post_request(self, resource, **kargs):
        kargs['verify'] = self._server_cert_file_path
        return self._handle_request(resource, requests.post, **kargs)

    def get_request(self, resource, **kargs):
        kargs['verify'] = self._server_cert_file_path
        return self._handle_request(resource, requests.get, **kargs)

class CumulativeAvg:
    def __init__(self):
        self._avg = 0.0
        self._count = 0
    
    def update(self, value):
        self._count += 1
        self._avg = self._cumulative_average(self._avg, value, self._count)
        
    def get(self):  
        return self._avg 

    def _cumulative_average(self, prev_avg, new_number, list_length):
        old_weight = (list_length - 1) / list_length
        new_weight = 1 / list_length
        return (prev_avg * old_weight) + (new_number * new_weight)
    
class ATM:
    def __init__(self, target_zone_id):
        # Parameters
        self._time_to_assume_noisy_obj = 2.0
        # Internal variables
        self._residents = {}
        self._target_zone_id = target_zone_id
        self._resident_avg = CumulativeAvg()

    def on_enter(self, obj_id, timestamp):
        if not obj_id in self._residents:
            self._residents[obj_id] = timestamp

    def on_exit(self, obj_id, timestamp):
        start_time = self._residents.get(obj_id)
        if start_time != None:
            resident_time = timestamp.ToSeconds() - start_time.ToSeconds()
            if resident_time > self._time_to_assume_noisy_obj:
                self._resident_avg.update(resident_time)
            del self._residents[obj_id]
        else:
            print(f"Error: {obj_id} is not exist in ATM {self.id()}.")

    def get_avg_resident_time(self):
        return self._resident_avg.get()

    def id(self):
        return self._target_zone_id


class ObjectStatistics:
    def __init__(self, obj):
        self.height = obj.bbox.size.z

        points_topview = load_object_points(obj)[:,:2]
        self.num_points = points_topview.shape[0]
        if (self.num_points > 0):
            svd_ratio, svd_dimensions = ObjectStatistics.calculate_svd(points_topview)
            self.svd_ratio = svd_ratio # TODO just use dimension ratio instead?
            self.dimensions = svd_dimensions
        else:
            self.svd_ratio = None
            self.dimensions = None
            
        
    
    @staticmethod
    def calculate_svd(points_topview):
        obj_center = np.mean(points_topview, axis=0)
        centered_points_topview = points_topview - obj_center

        U, sigma, _ = np.linalg.svd(centered_points_topview, full_matrices=False, compute_uv=True)

        svd_ratio = sigma[0] / sigma[1]

        svd_points = np.dot(U, np.diag(sigma))
        svd_dimensions = svd_points.max(axis=0) - svd_points.min(axis=0)

        return svd_ratio, svd_dimensions


class ResidentPerson:
    def __init__(self, id, obj, timestamp):
        self._id = id
        self._enter_zone = None
        self._histories = []
        self.current_stats = None
        self._born_time = timestamp
        
        self.push_history(obj)

        # Parameters for checking if object is door
        self.travel_threshold = 1.5 # meter
        self.height_threshold = 2.2 # meter
        self.length_threshold = 1.0 # meter


    def is_door(self):
        travel_distance = self.calculate_travel_distance()
        current_height = self.current_stats.height
        current_num_points = self.current_stats.num_points

        check_door = (travel_distance < self.travel_threshold) and (current_height > self.height_threshold)
        if current_num_points > 2:
            current_dimensions = self.current_stats.dimensions
            check_door = check_door and current_dimensions[0] < self.length_threshold
        return check_door
        
    def is_misc(self):
        is_misc = True
        for obj in self._histories:
            if obj.label != sensr_type.LabelType.LABEL_MISC:
                is_misc = False
                break
        return is_misc

    def starting_zone(self):
        return self._enter_zone

    def visiting_zones(self):
        ret = set()
        for obj in self._histories:
            for zone_id in obj.zone_ids:
                ret.add(zone_id)
        return ret

    def born_timestamp(self):
        return self._born_time

    def push_history(self, obj):
        self._histories.append(obj)
        self._update_starting_zone(obj)
        self.current_stats = ObjectStatistics(obj)

    def _update_starting_zone(self, obj):
        if not self._enter_zone:
            for zone_id in obj.zone_ids:
                self._enter_zone = zone_id
                break

    def calculate_travel_distance(self):
        travel_distance = 0.0
        for index in range(len(self._histories)-1):
            current_pos, next_pos = self._histories[index].bbox.position, self._histories[index+1].bbox.position
            travel_distance += np.sqrt((current_pos.x - next_pos.x)**2 + (current_pos.y - next_pos.y)**2)
        return travel_distance
    


class Bank(MessageListener):

    def __init__(self, address):
        # Parameters
        self._too_long_resident_time = 60.0 * 60.0 # 1 hour
        self._main_gate_id = 1003
        # Internal variables
        self._residents = {}
        self._resident_avg = CumulativeAvg()
        self._atm_visitor_count = 0
        self._visitor_count = 0
        self._ATMs = {1007: ATM(1007),
                      1008: ATM(1008),
                      1009: ATM(1009),
                      1010: ATM(1010),
                      1011: ATM(1011)}
        # Get zone info from SENSR
        self._zone_info = {}
        self._REST = RESTAPI(address)
        response = self._REST.get_request("settings/zone")
        zone_id_list = response.json()
        for id in zone_id_list:
            response = self._REST.get_request(
                "settings/zone", params={"zone-id": id})
            zone_info = response.json()
            self._zone_info[zone_info["id"]] = zone_info["name"]

        super().__init__(address=address,
                         listener_type=ListenerType.OUTPUT_MESSAGE)

    @staticmethod
    def time_diff_in_s(time1, time2):
        return time1.ToSeconds() - time2.ToSeconds()

    def _zone_name(self, zone_id):
        if zone_id != None:
            return self._zone_info[zone_id]
        else:
            return "No Zone"

    def _on_entry_event_handler(self, zone_id, obj_id, timestamp):
        # ATM entering
        found_atm = self._ATMs.get(zone_id)
        if found_atm != None:
            found_atm.on_enter(obj_id, timestamp)

    def _on_exit_event_handler(self, zone_id, obj_id, timestamp):
        # objs = [res._histories[-1] for id, res in self._residents.items()]
        DebugPlotter.plot_scene(self._residents, "exit-")
        # ATM exiting
        found_atm = self._ATMs.get(zone_id)
        if found_atm != None:
            found_atm.on_exit(obj_id, timestamp)
            print(
                f"ATM({found_atm.id()}) avg: {found_atm.get_avg_resident_time():.2f}s.")
        # Catch the purpose of visit.
        else:
            if zone_id == self._main_gate_id:
                resident = self._residents.get(obj_id)
                if not resident.is_misc() and not resident.is_door():
                    is_ATM_user = False
                    for visit_zone in resident.visiting_zones():
                        if visit_zone in self._ATMs.keys():
                            is_ATM_user = True
                            break
                    self._visitor_count += 1
                    if is_ATM_user:
                        self._atm_visitor_count += 1
                

    def _on_losing_event_handler(self, obj_id, timestamp):
        # objs = [res._histories[-1] for id, res in self._residents.items()]
        DebugPlotter.plot_scene(self._residents, "lose-")
        # Calc resident time
        resident = self._residents.get(obj_id)
        if not resident.is_misc() and not resident.is_door():
            resident_time = Bank.time_diff_in_s(timestamp, resident.born_timestamp())
            self._resident_avg.update(resident_time)
            print(f"Obj({obj_id}) \
                    reident_time: {resident_time}, \
                    Starting Zone: {self._zone_name(resident.starting_zone())}")
            print(f"ATM/Total : {self._atm_visitor_count} / {self._visitor_count}")
        else:
            if resident.is_misc():
                last_word = ") is misc."
            else:
                last_word = ") is door."
            print("Obj(" + str(obj_id) + last_word)
        del self._residents[obj_id]

    def _on_get_output_message(self, message):
        assert isinstance(
            message, sensr_output.OutputMessage), "message should be of type OutputMessage"

        if message.HasField('stream'):
            for obj in message.stream.objects:
                found_obj = self._residents.get(obj.id)
                if found_obj == None:
                    self._residents[obj.id] = ResidentPerson(
                        obj.id, obj, message.timestamp)
                else:
                    found_obj.push_history(obj)
                    if Bank.time_diff_in_s(message.timestamp, found_obj.born_timestamp()) > self._too_long_resident_time:
                        print(f"Obj({obj.id}) lives in bank too long.")
                        del self._residents[obj.id]
        if message.HasField('event'):
            for zone_event in message.event.zone:
                if zone_event.type == sensr_output.ZoneEvent.Type.ENTRY:
                    self._on_entry_event_handler(
                        zone_event.id, zone_event.object.id, zone_event.timestamp)
                elif zone_event.type == sensr_output.ZoneEvent.Type.EXIT:
                    self._on_exit_event_handler(
                        zone_event.id, zone_event.object.id, zone_event.timestamp)
            for losing_event in message.event.losing:
                self._on_losing_event_handler(
                    losing_event.id, losing_event.timestamp)


def parse_arguments():
    parser = argparse.ArgumentParser(description='Sample code for KB bank.')
    parser.add_argument('--address', type=str, default='localhost')
    return parser.parse_args()


current_listner = None


def signal_handler(sig, frame):
    if current_listner is not None:
        current_listner.disconnect()


if __name__ == "__main__":

    args = parse_arguments()

    address = args.address
    current_listner = Bank(address)
    if current_listner is not None:
        signal.signal(signal.SIGINT, signal_handler)
        current_listner.connect()
