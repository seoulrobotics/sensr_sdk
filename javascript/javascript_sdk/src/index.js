'use strict';

// Arg parsing
const yargs = require('yargs');

// Message receiver
const receiver = require('./message_receiver');

// Parsing
const parsing = require('./parse_output');

module.exports = {
  run,
  receiver,
  parsing,
};

function run() {
  parseCmdArgs();
}

function fetchArgs() {
  return yargs
      .command('parse',
          'Parse from binary file.', {
            filename: {
              description: 'File to be parsed',
              alias: 'f',
              type: 'string',
            },
          })
      .command('dump',
          'Dump all receive messages from SENSR to a specified folder.', {
            output_dir: {
              description: 'Directory of where the received files are dumped',
              alias: 'o',
              type: 'string',
            },
          })
      .check(function(argv) {
        if (argv._.includes('parse') && argv.filename != undefined) {
          return true;
        } else if (argv._.includes('dump') && argv.output_dir != undefined) {
          return true;
        } else {
          return false;
        }
      })
      .help()
      .alias('help', 'h')
      .argv;
}

async function parseCmdArgs() {
  const argv = fetchArgs();

  if (argv._.includes('parse')) {
    parsing.parseOutputFile(argv.filename);
  }

  if (argv._.includes('dump')) {
    const outputDir = argv.output_dir;
    parsing.mkdir(outputDir);

    console.log(`Dumping SENSR output to ${outputDir}...`);
    const numExported = await receiveOutputs(outputDir);
    console.log(`Finished dumping ${numExported} messages from SENSR.`);
  }
}

async function receiveOutputs(outputDir) {
  const messageReceiver = new receiver.MessageReceiver();
  const numExported = await messageReceiver.dumpAllReceived(outputDir);
  return numExported;
}


