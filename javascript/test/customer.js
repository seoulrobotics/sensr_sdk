const{getData} = require("./client.js")

// import{registerObjectUpdate, callback} from './client.js'

// getData(function(tableData) {
//     console.log(tableData); 
//     console.log("Here")
//   });

var x = 0
function tableData(response){
    console.log(response)
    x = response
    console.log(x)
    console.log("Hello World")
   
    // console.log(x)
    x.forEach(element => {
        console.log(element.getId())
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