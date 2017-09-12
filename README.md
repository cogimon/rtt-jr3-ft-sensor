# rtt-jr3-ft-sensor
RTT interface for JR3 force-troque sensor. This, hopefully will provide realtime communication with the sensor. Currently we have a ROS interface thanks to Arne and [David Oeser](https://github.com/Oeser) (maintainer) will bring this to RTT.

## Usage

## Hardware and software dependencies
### Hardware:
The sensor connects to a PCI port through a cable that looks like a phone cable. We need to check there are any gimmicks.

### Software:
Apart from standard orocos/rtt libraries and rst-rt data types, the driver which is a single header file is shipped with this repo.

## TODO
   1. Add credit for Arne   
   2. Check the frequency of the sensor and see if it make sense for the component not to have cycle time.
