# Overview

The fluidhubapi-testing is a micro REST webservice for testing the OpenFLUID FluidHub client.
Once deployed, it proposes fake news and wares lists as it could be proposed by a real FluidHub. 


# Installation

## Requirements

* A running Apache 2.2+ server
* The Slim framework accessible as a PHP framework (e.g. in the include_path variable of PHP configuration)

## Deployment

1. Create a directory served by your Apache server
2. Copy content of the fluidhubapi-testing directory into the Apache served directory 
(including the hidden .htaccess file)
3. Fix access right if needed

## Usage

### Using web browser

Point to the full URL to apache served directory with your web browser and test the installation.

As an example, if the Apache served directory is accessible through the http://my.server.org/fluidhubapi-testing/ URL,
go to:
* http://my.server.org/fluidhubapi-testing/ to get informations about the test FluidHub 
* http://my.server.org/fluidhubapi-testing/wares to get the list of wares hosted by the test FluidHub
* http://my.server.org/fluidhubapi-testing/wares?detailed=yes to get the detailed list of wares hosted by the test FluidHub
* http://my.server.org/fluidhubapi-testing/news to get the news broadcasted by the test FluidHub as an RSS feed
* etc ...

### In OpenFLUID unit tests

To use the test FluidHub in OpenFLUID unit tests, put the following line (adapted to your configuration) in a `CMake.in.config.cmake` file:

```
SET(TESTS_FLUIDHUB_URL "http://my.server.org/fluidhubapi-testing/")
```
