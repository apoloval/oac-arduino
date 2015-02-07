## v0.2

* `oacbtn.h`: enable default constructor for `Button` class. 
* `oacled.h`: add a `writeDec()` function that allows to write decimal numbers
coded in separated values for integer and decimal part. 
* `oacled.h`: add a `setDecodeMode()` overload to operate in a range of digits.
* `oackb.h`: support OE signal to be unspecified. If so, it is assumed that
OE pins of 7219's are connected to ground and therefore continuously ouput
enabled. 

## v0.1

Initial version
