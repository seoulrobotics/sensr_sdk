const WebSocket = require('ws');
const outputMsg = require('./../js_proto/sensr_proto/output_pb.js');
const pointMsg = require('./../js_proto/sensr_proto/point_cloud_pb');

const SensrClient = endpoint => {
    const socket_result = new WebSocket(`ws://${endpoint}:5050`);
    socket_result.binaryType = 'arraybuffer';
    const socket_point = new WebSocket(`ws://${endpoint}:5051`);
    socket_point.binaryType = 'arraybuffer';
  
    let objectUpdateListener, pointCloudUpdateListener;
  
    return {
      listenToObjectUpdate: (streamCallback, eventCallback, timestampCallback) => {
        if (!streamCallback && !eventCallback && !timestampCallback) return;
        
        if (objectUpdateListener) {
          socket_result.removeEventListener('message', objectUpdateListener);
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
        socket_result.addEventListener('message', objectUpdateListener);
      },
      listenToPointCloudUpdate: callback => {
        if (!callback) return;

        if (pointCloudUpdateListener) {
          socket_point.removeEventListener('message', pointCloudUpdateListener);
          pointCloudUpdateListener = undefined;
        }
  
        pointCloudUpdateListener = event =>
          callback(pointMsg.PointResult.deserializeBinary(event.data));
        socket_point.addEventListener('message', pointCloudUpdateListener);
      },
      disconnect: () => {
        socket_result.removeEventListener('message', objectUpdateListener);
        socket_point.removeEventListener('message', pointCloudUpdateListener);
        socket_result.close();
        socket_point.close();
      }
    };
};


module.exports = { SensrClient };