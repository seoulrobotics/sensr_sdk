const pointMsg = require('./../js_proto/sensr_proto/point_cloud_pb');

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
          console.log('Obj (%d): point no. %f',object.getId(), object_point_num);
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