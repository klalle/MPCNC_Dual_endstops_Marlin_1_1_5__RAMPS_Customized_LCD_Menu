# MPCNC - custom LCD-menu and external joystick.
My customized LCD-menu for the MPCNC with dual endstops and a Joystick (shield for Arduino UNO)

This Repo contains both the Repo for the Marlin-code, and the Joystick repo.
Marlin is modified to show custom menu on LCD, and as well take commands from the Joystick slave Arduino on RX1(/TX1 but not used)
There is a readme file in both repos explaining more in detail what and how they do it.

The Marlin version I am using is not the newest (1.1.5), Marlin 2.x has gone through a huge design remake, and I havn't got time to investigate how to customize the menu with the new version. The joystick should work under 2.x as well (you can define Marlin 2 to use Serial1 as a second serial port)

This is an example of the none-endstop version with my menu (no joystick): 
https://github.com/klalle/Marlin_RC7_LCD_Customization/tree/master/Example%20of%20use

/Kalle
