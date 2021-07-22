## Blinky project

The **Blinky** project is a simple example that can be used to verify the
basic tool setup.

It is compliant to the Cortex Microcontroller Software Interface Standard (CMSIS)
and uses the CMSIS-RTOS2 API interface for RTOS functionality. The CMSIS-RTOS2 API
is available with various real-time operating systems, for example RTX5 or FreeRTOS.

## Operation

 - At start the vioLED0 blinks in 1 sec interval.
 - The vioBUTTON0 changes the blink frequency and start/stops vioLED1.

The board hardware mapping of vioLED0, vioLED1, and vioBUTTON0 depends on the 
configuration of the CMSIS-Driver VIO.

## IMPORTANT NOTES

Board is initially loaded with application which disables debug 
functionality, so it is necessary to erase flash before using the board.

### Procedure

- change SW7 switch position to different than initial which is:
  OFF, ON, ON, OFF
- select Release target in uVision project
- connect USB cable to J28 connector (CMSIS-DAP)
- reset the board by SW4 push-button
- click on Flash -> Erase soon after releasing SW4 push-button
- after flash erase finishes power-off the board (disconnect from J28)
- return switch SW7 to initial position
- connect USB cable to J28 and board is now ready for debugging