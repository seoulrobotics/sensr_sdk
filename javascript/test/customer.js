const{getData, getStatus, getError, getEnd, init} = require("./client.js")


function tableData(response){
    console.log(response.getTimeStamp().getSeconds());
    var obj_list = response.getObjectsList();
    obj_list.forEach(element => {
        var vel = element.getVelocity();
        console.log(element.getId(), element.getConfidence(), element.getLabel())
        console.log("Velocity:", vel.getX(), vel.getY(), vel.getZ())
    });
}
init();
getData(tableData);


// getStatus((status) => {
//     console.log(status);
// })

// getError((error) => {
//     console.log(error);
// })

// getEnd((end) =>{
//     console.log(end);
// })
