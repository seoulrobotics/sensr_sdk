const{init, getStreamMessage, getEventMessage, getPointResults } = require("./client.js")


function streamData(response){
    //  var obj_list = response.getObjectsList();
    //   obj_list.forEach(element => {
    //       var point_list = element.getPoints_asU8();
    //       console.log(element.getId(), element.getBbox().getYaw(), element.getLabel(), point_list.length)
    //   });
}

function eventData(response){
    var zone_list = response.getZoneList();
    zone_list.forEach(element => {
        var obj = element.getObject();
        if (element.getType() == 1) {
            console.log("Enter zone(", element.getId() ,") : ", obj.getId());
        }
        else if (element.getType() == 2) {
            console.log("Exit zone(", element.getId() ,") : ", obj.getId());
        }
    });
    // var losing_list = response.getLosingList();
    // losing_list.forEach(element => {
    //     console.log("losing", element.getId());
    // });
}

function pointData(response){
    // var pcloud_list = response.getPointsList();
    // pcloud_list.forEach(element => {
    //      var point_list = element.getPoints_asU8();
    //      console.log(element.getId(), element.getType(), point_list.length)
    // });
    // console.log("Frame done.")
}

init();
getStreamMessage(streamData);
getEventMessage(eventData);
getPointResults(pointData);

// getStatus((status) => {
//     console.log(status);
// })

// getError((error) => {
//     console.log(error);
// })

// getEnd((end) =>{
//     console.log(end);
// })
