const pointMsg = require('./../js_proto/sensr_proto/point_cloud_pb');
const sensrType = require('./../js_proto/sensr_proto/type_pb');

const get_zone_data = (client) => {
  client.listenToObjectUpdate(null, response => {
    let zone_lists = response.getZoneList();
    if (zone_lists.length !== 0){
      zone_lists.forEach(zone_list =>{
        if(zone_list.getType() == 1){
          console.log("Entering zone (%d) : %d", zone_list.getId(), zone_list.getObject().getId());
        } else {
          console.log("Exiting zone (%d) : %d", zone_list.getId(), zone_list.getObject().getId());
        }
      });
    }
  });
}
  
const get_point_data = (client) => {
  client.listenToPointCloudUpdate(response => {
      const pointClouds = response.getPointsList();
      if(pointClouds.length !== 0){
        pointClouds.forEach(pointCloud =>{
          let num_points = Math.floor((pointCloud.getPoints_asU8().length)/(3.0*4));// 4-indicate sizeof(float)
          if(pointCloud.getType() == pointMsg.PointResult.PointCloud.Type.RAW){
            console.log("Topic {%s} no. of points - {%f}", pointCloud.getId(), num_points);
          }
          else if(pointCloud.getType() == pointMsg.PointResult.PointCloud.Type.GROUND){ 
            console.log("Ground points no. of points - {%f}", num_points);
          }
          else if(pointCloud.getType() == pointMsg.PointResult.PointCloud.Type.ENVIRONMENT){
            console.log("Environment points no. points - {%f}", num_points);
          }
          const intensities = new Float32Array(pointCloud.getIntensities().slice().buffer).sort()
          const mid = Math.floor((intensities.length - 1) / 2)
          console.log("Intensity [min, median, max] is [%f, %f, %f]", intensities[0], intensities[mid], intensities[intensities.length-1]);
      });
      }
  });
}
  
const get_object_data = (client) => {
  client.listenToObjectUpdate(response => {
    const objects = response.getObjectsList();
    if(objects.length !== 0){
      objects.forEach(object => {
        if(object.getPoints().length !== 0){
          let object_point_num = Math.floor((object.getPoints().length)/3);

          let type = Object.keys(sensrType.LabelType).find(
            key => sensrType.LabelType[key] === parseInt(object.getLabel()))
          
          // print object id and object type
          console.log(`Obj ${object.getId} type: ${type}`)
          
          // print tracking status
          console.log('Obj (%d) Tracking Status: %s',
          object.getId(),
          Object.keys(sensrType.TrackingStatus).find(
            key => sensrType.TrackingStatus[key] === parseInt(object.getTrackingStatus())
          ));

          //print point number of object
          console.log('Obj (%d) point number: %f',object.getId(), object_point_num);

          //print velocity of object
          console.log('Obj (%d) Velocity X: %f, Y: %f', object.getId(), object.getVelocity().getX(), object.getVelocity().getY());

          //print bbox
          console.log('Obj (%d) Bbox position X: %f, Y: %f, Z: %f, Bbox size X: %f, Y: %f, Z: %f, Bbox yaw: %f', 
                      object.getId(), 
                      object.getBbox().getPosition().getX(),
                      object.getBbox().getPosition().getY(),
                      object.getBbox().getPosition().getZ(),
                      object.getBbox().getSize().getX(),
                      object.getBbox().getSize().getY(),
                      object.getBbox().getSize().getZ(),
                      object.getBbox().getYaw())
          
          // print yaw anagle
          console.log(`Obj ${object.getId()} Yaw angle: ${object.yawRate()}`)
        }
      });
    }
  });
}
  
const get_health_data = (client) => {
  client.listenToObjectUpdate(response => {
    const healths = response.getHealth();
    if (healths !== undefined) {
      let health = healths.getNodesMap();
      console.log('System health: %d', healths.getMaster());
      health.forEach((v, k) =>{
        console.log(' AlgoNode(%s) health: %d', k, v.getStatus());
        let sensor_info = v.getSensorsMap();
        if (sensor_info.arr_ !== undefined){
          sensor_info.forEach((v1, k1) => {
            console.log('   Sensors(%s) health: %d', k1, v1);
          });
        } else {
          console.log('No Sensors are connected');
        }
      });
       
    }
  });
}
  
const get_time_data = (client) => {
    client.listenToObjectUpdate(null, null, response => { 
      let cur_time = new Date();
      console.log('Diff: %f ms', cur_time.getTime()- (response.array[0]*1000));
    });
}

module.exports = {
    get_zone_data,
    get_point_data,
    get_object_data,
    get_health_data,
    get_time_data
};