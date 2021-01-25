const sensr = require('sensr_js_sdk');
const output_sel = sensr.output_sel;
const sensr_client = sensr.sensr_client;
const yargs = sensr.yargs


function run() {
    parseCmdArgs();
  }
  
  function fetchArgs() {
    return yargs
        .command('show',
            'Specify type you want to visualize.', {
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
        var client = sensr_client.SensrClient(argv.ipaddress);
        output_sel.outputSelector(argv.typename, client);    
    }
}

process.on('exit',  ()=>{client.disconnect()});
run();