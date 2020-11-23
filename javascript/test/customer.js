const{init, getStreamMessage, getEventMessage, getPointResults } = require("./client.js")


function streamData(response){
    //  var obj_list = response.getObjectsList();
    //   obj_list.forEach(element => {
    //       var point_list = element.getPoints_asU8();
    //       console.log(element.getId(), element.getBbox().getYaw(), element.getLabel(), point_list.length)
    //   });
}

function pointData(response){
    var pcloud_list = response.getPointsList();
    pcloud_list.forEach(element => {
         var point_list = element.getPoints_asU8();
         console.log(element.getId(), point_list.length)
    });
}

init();
getStreamMessage(streamData);
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
