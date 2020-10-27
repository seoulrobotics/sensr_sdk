const{getData} = require("./client.js")

// import{registerObjectUpdate, callback} from './client.js'

// getData(function(tableData) {
//     console.log(tableData); 
//     console.log("Here")
//   });

var x = 0
function tableData(response){
    console.log(response.getTimeStamp().getSeconds());
    var obj_list = response.getObjectsList();
    obj_list.forEach(element => {
        var vel = element.getVelocity();
        console.log(element.getId(), element.getConfidence(), element.getLabel())
        console.log("Velocity:", vel.getX(), vel.getY(), vel.getZ())
    })
    // console.log(response.getObjectList())
}

// getData((tableData) => {
//     console.log(tableData)
//     return tableData
// })\

getData(tableData)



// let x = response.getObjectList()
//     x.forEach(element => {
//         let y = element.getId()
//         console.log(y)
//     });