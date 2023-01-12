# Output Data Specification

## OutputMessage

Field | Unit | Type | Description
--- | --- | --- | ---
timestamp | - | [Timestamp](https://developers.google.com/protocol-buffers/docs/reference/csharp/class/google/protobuf/well-known-types/timestamp) | Timestamp of the event was occurred.
stream | - | [StreamMessage](#streammessage) | SENSR perception result stream message.(Freq.: appx. 100ms)
event | - | [EventMessage](#eventmessage) | SENSR event message (Zone, Losing, SystemHealth)
custom | - | [CustomMessage](#custommessage) | SENSR additional result message (Field of Regard)

## StreamMessage

Field | Unit | Type | Description
--- | --- | --- | ---
objects | - | [Object](#object) | Object list in the latest frame.
has_objects | - | bool | A flag to check `objects` field is valid or not
zones | - | [ZoneConfig](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | General zone configuration. (Freq. appx. 10s) 
has_zones | - | bool | A flag to check `zones` field is valid or not
health | - | [SystemHealth](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | System health in the latest frame. (Freq. appx. 10s) 
static_object | - | [Object](#object) | Static object list in the latest frame. 

## EventMessage

Field | Unit | Type | Description
--- | --- | --- | ---
zone | - | [ZoneEvent](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | Event list related to zone.
losing | - | [LosingEvent](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | Event when SENSR lose tracking.
health | - | [SystemHealth](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | Event when SENSR has trouble inside.

## CustomMessage

Field | Unit | Type | Description
--- | --- | --- | ---
field_of_regard | - | [PolygonBox](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | List of object-occupied and blind area.
bg_learning_progress | - | float | Background learning progress (0.0 - 1.0).
replay | - | [ReplayInfo](#replayinfo) | Replay detailed information.
record | - | [RecordingInfo](#recordinginfo) | Recording detailed information.
profiling | - | [ProfilingResultSet](#profilingresultset) | Profiling information.

## PointResult

Field | Unit | Type | Description
--- | --- | --- | ---
points | - | [PointCloud](https://github.com/seoulrobotics/sensr_proto/blob/master/point_cloud.proto) | Point cloud stream data.
uri | - | string | Address of point cloud.

## Object

Field | Unit | Type | Description
--- | --- | --- | ---
id | - | int32 | ID of the object.
label | - | [LabelType](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | Classification of the object.
confidence | - | float | 0 to 1 probability of the object classification.
bbox | - | [BoundingBox](#boundingbox) | Bounding box of the object.
velocity | m/s | Vector3 | XYZ velocity of the object.
tracking_status | - | [TrackingStatus](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | Tracking status.
yaw_rate | - | float | Object rotation angle along the Z axis. (radian)
time_offset | - | [Duration](https://developers.google.com/protocol-buffers/docs/reference/csharp/class/google/protobuf/well-known-types/duration) | Last updated time offset relative to `first_cloud_arrival_timestamp`
retro_reflective | - | bool | A flag to show if the object is classified as retro reflective.
points | meters | bytes | List of sequential 3 float value set (XYZ).
history | - | [History](#history) | History of the object.
prediction | - | [Prediction](#prediction) | Prediction of the object.
zone_ids | - | int32 | List of zone occurpied by the object.
intensities | - | bytes | Sequential 1 float value set

### BoundingBox

Field | Unit | Type | Description
--- | --- | --- | ---
position | meters | Vector3 | XYZ position of the bounding box. Position is defined as (Center X, Center Y, Bottom Z).
size | meters | Vector3 | XYZ size of the bounding box.
yaw | radians | float | Bounding box rotation angle along the Z axis.

### History

Field | Unit | Type | Description
--- | --- | --- | ---
states | - | [History.State](#historystate) (list) | List of object’s tracked state.

### History.State

Field | Unit | Type | Description
--- | --- | --- | ---
position | meters | Vector3 | List of object’s tracked XYZ position.
timestamp | - | [Timestamp](https://developers.google.com/protocol-buffers/docs/reference/csharp/class/google/protobuf/well-known-types/timestamp) | Timestamp of the tracked XYZ position.

### Prediction

Field | Unit | Type | Description
--- | --- | --- | ---
positions | meters | Vector3 (list) | List of object’s predicted XYZ position.
reachable_set | meters | [ReachableSet](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | List of object's predicted position including uncertainty sorted by elapsed time.

### ReplayInfo

Field | Unit | Type | Description
--- | --- | --- | ---
current_index | - | int32 | Current frame index of the playing replay file.

### RecordingInfo

Field | Unit | Type | Description
--- | --- | --- | ---
saving_progress | - | float | Progress of saving recorded file into disk.(0.0 to 1.0)

### ProfilingResultSet

Field | Unit | Type | Description
--- | --- | --- | ---
master_node | - | [ProfilingResult](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | Profiling result of master node.
algo_nodes | - | [ProfilingResult](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | List of profiling result of connected algo nodes.
