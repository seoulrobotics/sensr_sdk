const google_protobuf_timestamp_pb = require('google-protobuf/google/protobuf/timestamp_pb.js');
const pointMsg = require('./../js_proto/sensr_proto/point_cloud_pb');

const get_zone_data = (client) => {
    client.listenToObjectUpdate(null, response => {
      let zone_lists = response.array;
      if(zone_lists[0][0] !== undefined){
        if(zone_lists[0][0][2] == 1){
          console.log("Entering zone (%d) : %d", zone_lists[0][0][1], zone_lists[0][0][3][0]);
        } else {
          console.log("Exiting zone (%d) : %d", zone_lists[0][0][1], zone_lists[0][0][3][0]);
        }
      }
    });
  }
  
const get_point_data = (client) => {
    client.listenToPointCloudUpdate(response => {
        const pointClouds = response.getPointsList();
        
        pointClouds.forEach(pointCloud =>{
            let pointcloud_info = pointCloud.array;
            
            let num_points = Math.floor((pointcloud_info[2].length)/(3.0*4));// 4-indicate sizeof(float)
            if(pointcloud_info[0] == pointMsg.PointResult.PointCloud.Type.RAW){
            console.log("Topic {%s} no. of points - {%f}", pointcloud_info[1], num_points);
            }
            else if(pointcloud_info[0] == pointMsg.PointResult.PointCloud.Type.GROUND){ 
            console.log("Ground points no. of points - {%f}", num_points);
            }
            else if(pointcloud_info[0] == pointMsg.PointResult.PointCloud.Type.ENVIRONMENT){
            console.log("Environment points no. points - {%f}", num_points);
            }
        });
    });
}
  
const get_object_data = (client) => {
    client.listenToObjectUpdate(response => {
      const objects = response.getObjectsList();
  
      objects.forEach(object => {
        let object_info = object.array;
        if(object.getPoints().length !== 0){
          let object_point_num = Math.floor((object.getPoints().length)/3);
          console.log('Obj (%d): point no. %f',object_info[0], object_point_num);
        }
      });
    });
}
  
const get_health_data = (client) => {
    client.listenToObjectUpdate(response => {
      const healths = response.getHealth();
      if (healths !== undefined) {
        console.log('System health: %d', healths.array[0]);
        for(i=0; i < healths.wrappers_['2'].arr_.length; i ++){
          console.log(' AlgoNode(%s) health: %d', healths.wrappers_['2'].arr_[i][0], healths.wrappers_['2'].arr_[i][1][0]);
            if (healths.wrappers_['2'].arr_[0][1][2] !== undefined){
              for(index=0; index < healths.wrappers_['2'].arr_[0][1][2].length; index++){
                console.log('   Sensors(%s) health: %d',healths.wrappers_['2'].arr_[0][1][2][index][0], 
                                                      healths.wrappers_['2'].arr_[0][1][2][index][1]);
              }
            } else {
              console.log(' No Sensors are connected');
            }
        }
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