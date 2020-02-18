# Output Data Specification

## Object

Field | Unit | Type | Description
--- | --- | --- | ---
id | - | int32 | ID of the object.
bbox | - | BoundingBox | Bounding box of the object.
label | - | int32 | Classification of the object. see [labels.proto](labels.proto)
track | - | TrackingInfo | Tracking information of the object.
points | meters | bytes | List of sequential 3 float value set (XYZ).

### BoundingBox

Field | Unit | Type | Description
--- | --- | --- | ---
position | meters | Vector3 | XYZ position of the bounding box. Position is defined as (Center X, Center Y, Bottom Z).
size | meters | Vector3 | XYZ size of the bounding box.
yaw | radians | float | Bounding box rotation angle along the Z axis.

### TrackingInfo

Field | Unit | Type | Description
--- | --- | --- | ---
probability | - | float | 0 to 1 probability of the object classification.
tracking_reliable | - | bool | True if the object’s tracking is reliable.
velocity | m/s | Vector3 | XYZ velocity of the object.
history | meters | Vector3 (list) | List of bounding box’s XYZ position values along the tracking history.
prediction | meters | Vector3 (list) | List of bounding box’s XYZ position values in the future.
