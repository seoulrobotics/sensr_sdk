
const get_data = require("./get_data.js")


const outputSelector = (type, network) => {
    switch(type){ 
      case "zone":
        get_data.get_zone_data(network);
        break;
      case "point":
        get_data.get_point_data(network);
        break;
      case "object":
        get_data.get_object_data(network);
        break
      case "health":
        get_data.get_health_data(network);
        break
      case "time":
        get_data.get_time_data(network);
        break
      default:
        console.log("no corresponding type detected!")
    }
}
  
module.exports = { outputSelector }