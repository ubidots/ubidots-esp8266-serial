# Ubidots-ESP8266-serial

ESP8266 offers a complete and self-contained Wi-Fi networking solution, allowing it to either host the application or to offload all Wi-Fi networking functions from another application processor.

When the ESP8266 hosts an application and it is the only application processor in the device, and therefore able to boot up directly from an external flash. The ESP8266 has integrated cache to improve the performance and minimize the memory requirements.

Alternately, serving as a Wi-Fi adapter, wireless internet access can be added to any microcontroller-based design with simple connectivity through UART interface or the CPU AHB bridge interface.

This library will let you use the **ESP8266** module as a telemetry unit with any microcontroller connected to it via serial as logger. Please note that if the logger does not have more than one serial port you will __not__ able to visualize the response, but it can send data without any issues.

![hola](https://cdn2.hubspot.net/hubfs/329717/Ubidots_telemetryUnit.png)

Before to start, you must choose which logger to use, in this case we decided use an **Arduino MEGA**.

**IMPORTANT NOTE:** It's very important know that we're going to manage two codes, one for the logger(Arduino MEGA), and a second for the telemetry unit(ESP8266).

## Requirements

* Any microcontroller with more than one serial port. e.i: [Arduino MEGA](https://www.arduino.cc/en/Main/arduinoBoardMega)
* [An ESP8266 module](http://www.aliexpress.com/wholesale?catId=0&initiative_id=AS_20160302130000&SearchText=esp8266)
* [UartSBee](http://wiki.seeed.cc/UartSBee_V4/) or any Uart to USB device.
* [Arduino IDE 1.6.0 or higher](https://www.arduino.cc/en/Main/Software)
* [Ubidots Arduino ESP8266 library](https://github.com/ubidots/ubidots-esp8266-serial/archive/master.zip)
* [Ubidots Account](https://ubidots.com/). 

## Command request

We have designed a simple payload format you can use to send/get data from your devices to/from Ubidots using this protocol.

To get a better idea of protocol and learn how to manage more than one value or the management of timestamp/context, please reference the article below:

* [Send data to Ubidots over TCP or UDP](http://help.ubidots.com/developers/send-data-to-ubidots-over-tcp-or-udp).

The first part of the structure of the Payload is:

> USER_AGENT/VERSION|METHOD|TOKEN

Where:

* **USER_AGENT**: An identifier of your choice to help us to log your data for possible debugging in the future. 
* **VERSION**: It's a good practice to include a version in case you're wrapping these requests within a library.

**Here are some examples of a USER_AGENT/VERSION**: "CompanyIoT/1.0" or "Particle/1.3"

* **METHOD(POST/GET/LV)**: Of the following protocol methods, you must choose one. To send values to Ubidots use the method `POST`. To get value from Ubidots you have to use the methods `GET` or `LV`.
* **TOKEN**: Your Ubidots account TOKEN.
* **DEVICE_NAME**: The name of the device at Ubidots.
* **DEVICE_LABEL**: The unique label of your Device. [Check out our API docs](https://ubidots.com/docs/api/index.html#send-values) to learn how to send data to Ubidots.
* **VARIABLE_LABEL**: The unique label of your Variable. [Check out our API docs](https://ubidots.com/docs/api/index.html#send-values) to learn how to send data to Ubidots.
* **VARIABLE_ID**: This is the unique ID of your Device. [Check out our API docs](https://ubidots.com/docs/api/index.html#send-values) to learn how to send data to Ubidots

The library will verify the request to Ubidots´ server using `init#` and `#final` at the start and final of the payload/command. Reference: as you can see -> `init#     command      #final`

Also, we setup an `ONLINE?` command to let you know if the everything is OK with the connection. 

These **request examples** will help you understand how to build the command:

* Verify the connection (**ONLINE?**):

> init#ONLINE?#final

* Send values to Ubidots (**POST**):
> init#USER_AGENT/VERSION|POST|TOKEN|DEVICE_LABEL:DEVICE_NAME=>VARIABLE:VALUE|end#end

**Command request example**:

> init#UbidotsESP8266/2.0|POST|4fgSKmYujOJZ2x63s7376xbFtaSLvq0|telemetry:ESP8266=>temperature:91.00000|end#final

* Get value from Ubidots (**GET / LV**):

**GET**:

> init#USER_AGENT/VERSION|GET|TOKEN|VARIABLE_ID|end#final

**Command request example**:

> init#UbidotsESP8266/2.0|GET|4fgSKmYujOJZ2x63s7376xbFtaSLvq0|5931cd2d762542136491c29f|end#final

**LV**:

> init#USER_AGENT/VERSION|LV|TOKEN|DEVICE_LABEL:VARIABLE_LABEL|end#final

**Command request example**:

> init#UbidotsESP8266/1.1|LV|9DriKmYujOJZ2x637376xbFtaSLvq0|telemetry:temperature|end#final

## Setup

1. Setup the **telemetry unit - ESP8266**. Reference to the [Documentation](https://github.com/ubidots/ubidots-esp8266-serial/tree/master/docs/TelemetryUnit_ESP8266.md) to find all the steps required. 
2. Setup the **Logger - Arduino MEGA**. Reference to the [Documentation](https://github.com/ubidots/ubidots-esp8266-serial/tree/master/docs/Logger.md) to find all the steps required.
3. Once the you complete the steps above, go to your [Ubidots account](https://ubidots.com/) to visualize the data received. 
