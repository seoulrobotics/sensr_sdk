'use strict';

// Arg parsing
const yargs = require('yargs')

// Parsing
const parsing = require('./parse_output')


main()



function main() {
  parseCmdArgs();
}

function fetchArgs() {
  return yargs
    .command('parse', 'Parse from binary file.', {
      filename: {
        description: "File to be parsed",
        alias: 'f',
        type: 'string',
      }
    })
    .command('receive', 'Receive messages from SENSR.', {
      todo: {
        description: "todo",
        alias: 't',
        type: 'boolean',
      }
    })
    .check(function (argv) {
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

function parseCmdArgs() {
  const argv = fetchArgs();

  if (argv._.includes('parse')) {
    parsing.parseOutputFile(argv.filename)
  }

  if (argv._.includes('receive')) {
    console.log("RECEIVING... rip this doesn't actually do anything yet.")
  }

}



