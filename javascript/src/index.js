'use strict';

// Arg parsing
const yargs = require('yargs');

// Message receiver
const receiver = require('./message_receiver');

// Parsing
const parsing = require('./parse_output');


main();


function main() {
  parseCmdArgs();
}

function fetchArgs() {
  return yargs
      .command('parse', 'Parse from binary file.', {
        filename: {
          description: 'File to be parsed',
          alias: 'f',
          type: 'string',
        },
      })
      .command('receive', 'Receive messages from SENSR.', {
        todo: {
          description: 'todo',
          alias: 't',
          type: 'boolean',
        },
      })
      .check(function(argv) {
        if (argv._.includes('parse') && argv.filename != undefined) {
          return true;
        } else if (argv._.includes('receive')) {
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

  if (argv._.includes('receive')) {
    // TODO make directory if it does not exist
    const outputFolder = './javascript/testing_output';

    const outputs = await receive();

    console.log(`Successfully received ${outputs.length} messages from SENSR.`);
    console.log('Exporting to binary files...');

    parsing.exportToBinary(outputs, outputFolder);
    console.log('Done!');
  }
}

async function receive() {
  const messageReceiver = new receiver.MessageReceiver();
  messageReceiver.connect();
  const outputs = await messageReceiver.subscribe();
  messageReceiver.disconnect();
  return outputs;
}


