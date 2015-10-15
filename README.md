# remote2zwave
Convert remote IR signal to Vera3 Z-Wave using Arduino Ethernet

I use a Vera3 Z-Wave controller for home automation<br>
see http://getvera.com/controllers/vera3/

###Hardware###
- Vera3 up and running, connected to local network
- An old remote for unused device
- IR receiver (I use TSOP1838)
- Arduino Ethernet (+ serial to usb module for programming purposes)
- Some wires
- a box to build the project into

###Software###
- Arduino IDE 1.6.5
- Arduino-IRRemote module (https://github.com/z3t0/Arduino-IRremote)
- Ethernet module

###Attach IR receiver to Arduino###
- dat --> pin 8
- Vcc --> +5V
- GND --> GND

###Find the Vera3 deviceID###
Complete Vera3 configuration can be found in:
- http://veraip.local/port_3480/data_request?id=lu_sdata&output_format=xml

###Find the IR codes###
After uploading the source to the Arduino using the IDE, show the serial monitor. 
When pressing the desired button on the remote, the program will return unique HEX code for each button.
Use these codes in the corresponding 'case' statement.
For the setdim and setlight parameter 1 = Z-Wave device ID<br>
parameter 2 = 
- 1/0 for on/off in setlight
- 0-100 for off-on in setdim

