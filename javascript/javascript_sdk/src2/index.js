const yargs = require('yargs');
const output_sel = require('./output_selector.js')
function run() {
    parseCmdArgs();
  }
  
  function fetchArgs() {
    return yargs
        .command('show',
            'Spcifify Type you want to visualize.', {
              typename: {
                description: 'show proto msg received from master node',
                alias: 's',
                type: 'string',
              },
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
        output_sel.outputSelector(argv.typename, 'localhost');      
    }
}


run()