# Output Data Specification

## OutputMessage

Field | Unit | Type | Description
--- | --- | --- | ---
timestamp | - | [Timestamp](https://developers.google.com/protocol-buffers/docs/reference/csharp/class/google/protobuf/well-known-types/timestamp) | time stampe of the event was occurred.
stream | - | [StreamMessage](#streammessage) | SENSR perception result stream message.(Freq.: appx. 100ms)
event | - | [EventMessage](#eventmessage) | SENSR event message (Zone, Losing, SystemHealth)
custom | - | [CustomMessage](#custommessage) | SENSR additional result message (Field of Regard)

## StreamMessage

Field | Unit | Type | Description
--- | --- | --- | ---
objects | - | [Object](#object) | object list in the latest frame.
zones | - | [ZoneConfig](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | general zone configuration. (Freq. appx. 10s) 
health | - | [SystemHealth](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | system health in the latest frame. (Freq. appx. 10s) 

## EventMessage

Field | Unit | Type | Description
--- | --- | --- | ---
zone | - | [ZoneEvent](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | event list related to zone.
losing | - | [LosingEvent](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | event when SENSR lose tracking.
health | - | [SystemHealth](https://github.com/seoulrobotics/sensr_proto/blob/master/output.proto) | event when SENSR has trouble inside.

## CustomMessage

Field | Unit | Type | Description
--- | --- | --- | ---
field_of_regard | - | [PolygonBox](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | List of negative field of regard which represent occlusion areas.

## PointResult

Field | Unit | Type | Description
--- | --- | --- | ---
points | - | [PointCloud](https://github.com/seoulrobotics/sensr_proto/blob/master/point_cloud.proto) | point cloud stream data.
uri | - | string | address of point cloud.

## Object

Field | Unit | Type | Description
--- | --- | --- | ---
id | - | int32 | ID of the object.
label | - | [LabelType](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | Classification of the object.
confidnece | - | float | 0 to 1 probability of the object classification.
bbox | - | [BoundingBox](#boundingbox) | Bounding box of the object.
velocity | m/s | Vector3 | XYZ velocity of the object.
points | meters | bytes | List of sequential 3 float value set (XYZ).
history | - | [History](#history) | History of the object.
prediction | - | [Prediction](#prediction) | Prediction of the object.

### BoundingBox

Field | Unit | Type | Description
--- | --- | --- | ---
position | meters | Vector3 | XYZ position of the bounding box. Position is defined as (Center X, Center Y, Bottom Z).
size | meters | Vector3 | XYZ size of the bounding box.
yaw | radians | float | Bounding box rotation angle along the Z axis.

### History

Field | Unit | Type | Description
--- | --- | --- | ---
positions | meters | Vector3 (list) | List of object’s tracked XYZ position.

### Prediction

Field | Unit | Type | Description
--- | --- | --- | ---
positions | meters | Vector3 (list) | List of object’s predicted XYZ position.
reachable_set | meters | [ReachableSet](https://github.com/seoulrobotics/sensr_proto/blob/master/type.proto) | List of object's predicted range which includes uncertainty sorted by time-lapse.
