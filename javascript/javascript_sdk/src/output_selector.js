const get_data = require("./get_data.js")


const outputSelector = (type, client) => {
    switch(type){ 
      case "zone":
        get_data.get_zone_data(client);
        break;
      case "point":
        get_data.get_point_data(client);
        break;
      case "object":
        get_data.get_object_data(client);
        break
      case "health":
        get_data.get_health_data(client);
        break
      case "time":
        get_data.get_time_data(client);
        break
      default:
        console.log("no corresponding type detected!")
    }
}
  
module.exports = { outputSelector }