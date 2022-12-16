# Javascript Guide

This guide will show you how to use the SENSR output with Javascript using our SDK. 
## Prerequisite
Before following the steps below, please make sure the SDK has been correctly deployed according to the instructions [here](../README.md)

## Instructions
We use NodeJS for these examples so make sure you have the dev environment set up.

```bash
sudo apt-get update
sudo apt-get install nodejs
sudo apt-get install npm
```
If you have ROS installed you might face some problems with installing npm, please refer to [troubleshooting.md](./troubleshooting.md).

## Setting up your own project
To use the SENSR SDK, you first need to create your own project. 

Create an empty directory for your project and navigate to it. This can be located wherever you want. To initialize your project run:
```bash 
mkdir my_project
cd my_project
npm init # Follow the instructions, the defaults should be fine.

# Add sensr_js_sdk as dependency. This is added as a local module,
# hence you need to specify the relative (or absolute) path to 
# sensr_js_sdk
npm install ../relative/path/to/sensr_sdk/javascript/javascript_sdk 
```
You should now have a main file (called `index.js` by default) that you can run using `npm start` or `node index.js`. This file is the entry-point into your project.

## Basic usage

After installation if complete, you can run the example.

- Launch SENSR
- Open the terminal then navigate to javascript/console_output folder
  - If you want to open WSS communication, then please use `javascript/console_output_secure` folder instead of `javascript/console_output`.
- install dependencies by npm install
```base
npm install
```
- Run following command
```bash
node console_output.js show -s localhost -t object
# put server ip address after -s.
# you can replace "object" with "point", "zone", "health", or "time" to run the other examples
```