const google_protobuf_timestamp_pb = require('google-protobuf/google/protobuf/timestamp_pb.js');
const sensr_client = require('./sensr_client');

const get_zone_data = (network) => {
    let client = sensr_client.SensrClient(network);
    client.listenToObjectUpdate(response => {
      let zone_lists = response.getZonesList();
      // console.log(zone_lists)
      zone_lists.forEach(zone_list => {
          let obj = zone_list.getObject()
          console.log(obj)
          if (zone_list.getType() == 1) {
              console.log("Enter zone(", zone_list.getId() ,") : ", obj.getId());
          }
          else if (zone_list.getType() == 2) {
              console.log("Exit zone(", zone_list.getId() ,") : ", obj.getId());
          }
      });
    });
  }
  
const get_point_data = (network) => {
    let client = sensr_client.SensrClient(network);
    client.listenToPointCloudUpdate(response => {
        const pointClouds = response.getPointsList();
        
        pointClouds.forEach(pointCloud =>{
            let pointcloud_info = pointCloud.array;
            
            let num_points = Math.floor((pointcloud_info[2].length)/(3.0));
            if(pointcloud_info[0] == pointMsg.PointResult.PointCloud.Type.RAW){
            console.log("Topic {%f} no. of points - {%f}", pointcloud_info[1], num_points);
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
  
const get_object_data = (network) => {
    let client = sensr_client.SensrClient(network);

    client.listenToObjectUpdate(response => {
      const objects = response.getObjectsList();
  
      objects.forEach(object => {
        let object_info = object.array;
        if(object_info[100] !== undefined){
          let object_point_num = Math.floor((object_info[100].length)/3);
          console.log('Obj (%d): point no. %f',object_info[0], object_point_num);
        }
      });
    });
}
  
const get_health_data = (network) => {
    let client = sensr_client.SensrClient(network);

    client.listenToObjectUpdate(response => {
      const healths = response.getHealth();
      if (healths !== undefined) {
        console.log('AlgoNode(%s) health: %d', healths.wrappers_['2'].arr_[0][0], healths.wrappers_['2'].arr_[0][1][0]);
        if (healths.wrappers_['2'].arr_[0][1][2].length !== 0){
          for(index=0; index<healths.wrappers_['2'].arr_[0][1][2].length; index++){
            console.log(' Sensors(%s) health: %d',healths.wrappers_['2'].arr_[0][1][2][index][0], 
                                                  healths.wrappers_['2'].arr_[0][1][2][index][1]);
          }
        } else {
          console.log(' No Sensors are connected');
        }
      }
    });
  }
  
const get_time_data = (network) => {
    let client = sensr_client.SensrClient(network);
  
    client.listenToObjectUpdate(response => { 
      const curtime = new google_protobuf_timestamp_pb.Timestamp(response);
      curtime.fromDate(new Date());
      console.log('Diff: %f ms',(curtime.array["0"]-curtime.array["1"])/1000000);
    });
}

module.exports = {
    get_zone_data,
    get_point_data,
    get_object_data,
    get_health_data,
    get_time_data
};