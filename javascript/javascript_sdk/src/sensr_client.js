const WebSocket = require('ws');
const outputMsg = require('./../js_proto/sensr_proto/output_pb.js');
const pointMsg = require('./../js_proto/sensr_proto/point_cloud_pb');

const SensrClient = endpoint => {
    const result_socket = new WebSocket(`ws://${endpoint}:5050`);
    result_socket.binaryType = 'arraybuffer';
    const point_socket = new WebSocket(`ws://${endpoint}:5051`);
    point_socket.binaryType = 'arraybuffer';
  
    let objectUpdateListener, pointCloudUpdateListener;
  
    return {
      listenToObjectUpdate: (streamCallback, eventCallback, timestampCallback) => {
        if (!streamCallback && !eventCallback && !timestampCallback) {
          return;
        }
        
        if (objectUpdateListener) {
          result_socket.removeEventListener('message', objectUpdateListener);
          objectUpdateListener = undefined;
        }
  
        objectUpdateListener = event => {
          const response = outputMsg.OutputMessage.deserializeBinary(event.data);
          if (response.hasStream() && streamCallback) {
            streamCallback(response.getStream());
          } else if (response.hasEvent() && eventCallback) {
            eventCallback(response.getEvent());
          } else if (response.hasTimestamp()  && timestampCallback){
            timestampCallback(response.getTimestamp());
          }
        };
        result_socket.addEventListener('message', objectUpdateListener);
      },
      listenToPointCloudUpdate: callback => {
        if (!callback) return;

        if (pointCloudUpdateListener) {
          point_socket.removeEventListener('message', pointCloudUpdateListener);
          pointCloudUpdateListener = undefined;
        }
  
        pointCloudUpdateListener = event =>
          callback(pointMsg.PointResult.deserializeBinary(event.data));
        point_socket.addEventListener('message', pointCloudUpdateListener);
      },
      disconnect: () => {
        result_socket.removeEventListener('message', objectUpdateListener);
        point_socket.removeEventListener('message', pointCloudUpdateListener);
        result_socket.close();
        point_socket.close();
      }
    };
};


module.exports = { SensrClient };