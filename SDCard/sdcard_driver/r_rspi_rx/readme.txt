PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

RX RSPI Driver
==============

Document Number 
---------------
N/A

Version
-------
v1.40

Overview
--------
Basic RSPI Master Mode driver. 

Features
--------
* Functions to init, read, and write the RSPI channels
* Ability to lock channels to a task to ensure no other tasks try to use peripheral while it is already being used.
 
Supported MCUs
--------------
* RX621, RX62N Group
* RX630 Group
* RX631, RX63N Group 

Boards Tested On
----------------
* RSK+RX62N
* RDKRX62N
* RSKRX63N
* RDKRX63N

Limitations
-----------
* This code is not re-entrant but does protect against multiple concurrent function calls.

Peripherals Used Directly
-------------------------
* RSPI0
* RSPI1
* RSPI2

Required Packages
-----------------
* none

How to add to your project
--------------------------
* Add src\r_rspi_rx.c to your project.
* Add an include path to the 'r_rspi_rx' directory. 
* Add an include path to the 'r_rspi_rx\src' directory.
* Configure middleware through r_rspi_rx_config.h.
* Add a #include for r_rspi_rx_if.h to files that need to use this package. 

Toolchain(s) Used
-----------------
* Renesas RX v1.02

File Structure:
---------------
r_rspi_rx
|   readme.txt
|   r_rspi_rx_config.h
|   r_rspi_rx_if.h
|
\---src
        r_rspi_rx.c
                
