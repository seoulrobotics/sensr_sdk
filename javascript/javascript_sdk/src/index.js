'use strict';

// Arg parsing
const yargs = require('yargs');

// Message receiver
const receiver = require('./message_receiver');

// Parsing
const parsing = require('./parse_output');

// Example
const example = require('./../../example/social_distancing');


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
        output_dir: {
          description: 'Directory of where the received files are dumped',
          alias: 'o',
          type: 'string',
        },
      })
      .command('example', 'Run example.', {
        input_dir: {
          description: 'Directory of binary files to be read',
          alias: 'i',
          type: 'string',
        },
      })
      .check(function(argv) {
        if (argv._.includes('parse') && argv.filename != undefined) {
          return true;
        } else if (argv._.includes('receive') && argv.output_dir != undefined) {
          return true;
        } else if (argv._.includes('example') && argv.input_dir != undefined) {
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
    const outputDir = argv.output_dir;
    parsing.mkdir(outputDir);

    console.log(`Dumping SENSR output to ${outputDir}...`);
    const numExported = await receiveOutputs(outputDir);
    console.log(`Finished dumping ${numExported} messages from SENSR.`);
  }

  if (argv._.includes('example')) {
    const inputDir = argv.input_dir;

    example.runSocialDistancing(inputDir);
  }
}

async function receiveOutputs(outputDir) {
  const messageReceiver = new receiver.MessageReceiver(outputDir);
  const numExported = await messageReceiver.receive();
  return numExported;
}


