const yargs = require('yargs');
const sensr = require('sensr_js_sdk');
const output_sel = require('./output_selector.js');
const sensr_client = sensr.sensr_client;




function run() {
    parseCmdArgs();
  }
  
  function fetchArgs() {
    return yargs
        .command('show',
            'specify type and ip address', {
              typename: {
                description: 'type of data(ex. zone, object, health, time, point)',
                alias: 't',
                type: 'string',
              },
              ipaddress: {
                description: 'specify server',
                alias: 's',
                type: 'string',
              }
            })
        .check(function(argv) {
            return true;
        })
        .help()
        .alias('help', 'h')
        .argv;
  }
  
  async function parseCmdArgs() {
    const argv = fetchArgs();
  
    if (argv._.includes('show')) {
        var client = sensr_client.SensrClient(argv.ipaddress, true);
        output_sel.outputSelector(argv.typename, client);    
    }
}

run();
process.on('exit',  ()=>{client.disconnect()});
