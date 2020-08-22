
PAC193x Linux Driver
====================
  
In order to build this driver you must have a prebuilt kernel available that
contains the configuration and header files used in the build.
Also, the kernel must have been built with IIO class and modules enabled.

Command Syntax
==============
The command to build the driver as a module is:
	$ make modules

To install the module:
	$ sudo make install

Device-Tree bindings
====================
In order to have a fully functional driver the proper device-tree entries must
be created.
The "pac193x.txt" file describes the device-tree bindings required by this
driver.
An example device-tree binding for SAMA5D2-XULT board is provide in the file:
"at91-sama5d2_xplained.dts"

Using the driver
================
If everything works fine, after the Linux system finishes booting, there 
should be entries within the sys subsystem under /sys/bus/iio/devices

For the SAMA5D2-XULT board, the driver exported data will be found at:
/sys/bus/iio/devices/iio\:device1/

The driver provided data can be accessed in the same way as with other IIO
class drivers