# MPCNC_Dual_endstops_Marlin_1_1_5__RAMPS_Customized_LCD_Menu
My customized LCD-menu for the MPCNC now works with dual endstops
Use preferably PlatformIO for compiling/uploading sketch (ArduinoIDE works as well)

This is a customization of the LCD-menu that's in the Marlin version that's configured to work with an MPCNC from vicious1.com
The project version used is the: "DualDual_1_1_5" downloaded from the Firmware-page

I only use the MPCNC as a CNC for cutting stuff out, and not for 3D-printing, so all of those settings are stripped from the menu. 

I was going crazy when trying to access the menu objects I needed through the LCD, so I came up with a couple of improvements that customized the menu for me.

#### This is what I aimed for:

* Get rid of all junk I don’t use (I’m only using the machine as a CNC-router)
* Reorder the menu to put the mostly used commands on top
* Confirm functions to not accedently run the wrong function
* Access the z-probe offset from the LCD-menu
  * The height of the piece of metal I let the tool go down against to probe for z (connected to GND and z-min)
* Make a temporary Z-offset-menu item
  * Useful if e.g. an engraving ends up too shallow – just make a temporary z-offset of -0.5 mm and run the same file again (make sure to * not lose X and Y!!)
  * So that you don’t have to alter the z-probe offset all the time (the probe is most likely the same one...!)
* “Go to home” command in menu
* Set current position to home (without offset) menu item
* Make the 2nd option on a submenu pre-selected
  * If I e.g. enter the submenu “Move Axis” from the “Prep-menu”– most likely my next action won’t be to go back to the prep-menu, but to * press the first real option (2nd option)

#### And this is my custom menu that I ended up with: <br>
* Startscreen
  * Back
  * Move Axis
    * Set coordinates 
      * X,Y,Z (set coordinates for X,Y,Z and then execute move)
    * 10mm 
      * X,Y
    * 1mm 
      * Z,X,Y
    * 0.1mm 
      * Z,X,Y
  * Set home
    * X0 Y0 Endstops (separate endstops for X1,X2,Y1,Y2 to strighten the gantry)
    * Z0 Here (runs "G92 Z0" to set Z0 at current position
    * Z Probe (lets you use a probe to zero Z)
      * Zero only Z
      * Zero all (this zeros the X and Y to current position as well as probing for Z)
  * Go To Home
    * All (starts by elevating Z 10mm to hopefully not crash before going to X0 & Y0 and then Z0) 
    * X
    * Y
    * Z
  * Run From SD
    * bla bla bla 
    * Confirmation before it starts to run a file!
    * added "Mount Card" if SD-card is removed and replaced
  * Disable steppers
  * Temp Z offset (handy if you're running an engraving and it didn't go deep enough, just offset z a bit an rerun the code)
  * Z Homing offset (this is the thickness of your probe bottom piece that gets substracted from the endstop) -store in eeprom!
  * Speed [%]
  * Store to EEPROM
  * LOAD from EEPROM
  * Control
    * acceleration, jerk, speeds.....

Make sure you change the configuration files to suit your setup, I'm using different nr of teeth on my gears than what comes with Ryan's setup! Search for "kalle" in configuration.h and you'll see what I've changed and what it used to be... 

Use this with my modifyed V9 postprocessor for F360 found here: https://github.com/klalle/Marlin_RC7_LCD_Customization/tree/master/Files%20to%20modify/Post%20Processor 
It retracts z to +30 and then moves back to X0Y0 and keeps the steppers on!
remember this if you're cutting deeper than 30mm, then you'd like to change this to something higher or skip this part in the gcode!

This is an example of the none-endstop version: 
https://github.com/klalle/Marlin_RC7_LCD_Customization/tree/master/Example%20of%20use

/Kalle
