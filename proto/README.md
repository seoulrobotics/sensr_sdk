# Output Data Specification

## OutputMessage

Field | Unit | Type | Description
--- | --- | --- | ---
time_stamp | - | [Timestamp](https://developers.google.com/protocol-buffers/docs/reference/csharp/class/google/protobuf/well-known-types/timestamp) | time stampe of the event was occurred.
stream | - | [StreamMessage](sensr_proto/output.proto) | SENSR perception result stream message.(Freq.: appx. 100ms)
event | - | [EventMessage](sensr_proto/output.proto) | SENSR event message (Zone, Losing, SystemHealth)

## StreamMessage

Field | Unit | Type | Description
--- | --- | --- | ---
objects | - | [Object](sensr_proto/type.proto) | object list in the latest frame.
zones | - | [ZoneConfig](sensr_proto/output.proto) | general zone configuration. (Freq. appx. 10s) 
health | - | [SystemHealth](sensr_proto/output.proto) | system health in the latest frame. (Freq. appx. 10s) 

## EventMessage

Field | Unit | Type | Description
--- | --- | --- | ---
zone | - | [ZoneEvent](sensr_proto/output.proto) | event list related to zone.
losing | - | [LosingEvent](sensr_proto/output.proto) | event when SENSR lose tracking.
health | - | [SystemHealth](sensr_proto/output.proto) | event when SENSR has trouble inside.

## PointResult

Field | Unit | Type | Description
--- | --- | --- | ---
points | - | [PointCloud](sensr_proto/point_cloud.proto) | point cloud stream data.
uri | - | string | address of point cloud.

## Object

Field | Unit | Type | Description
--- | --- | --- | ---
id | - | int32 | ID of the object.
label | - | [LabelType](sensr_proto/type.proto) | Classification of the object.
confidnece | - | float | 0 to 1 probability of the object classification.
bbox | - | [BoundingBox](#id-boundingbox) | Bounding box of the object.
velocity | m/s | Vector3 | XYZ velocity of the object.
points | meters | bytes | List of sequential 3 float value set (XYZ).
history | meters | Vector3 (list) | List of bounding box’s XYZ position values along the tracking history.

### BoundingBox {#id-boundingbox}

Field | Unit | Type | Description
--- | --- | --- | ---
position | meters | Vector3 | XYZ position of the bounding box. Position is defined as (Center X, Center Y, Bottom Z).
size | meters | Vector3 | XYZ size of the bounding box.
yaw | radians | float | Bounding box rotation angle along the Z axis.

