Eggduino
====

Arduino Firmware for Eggbot / Spherebot with Inkscape-Integration

Version 1.2

Regards: Eggduino-Firmware by Joachim Cerny, 2014

Thanks for the nice libs ACCELSTEPPER and SERIALCOMMAND, which made this project much easier. Thanks to the Eggbot-Team for such a funny and enjoyable concept! Thanks to my wife and my daughter for their patience. :-)

Features:

- Implemented Eggbot-Protocol-Version 2.1.0
- Turn-on homing: switch-on position of pen will be taken as reference point.
- No collision-detection!!
- Supported Servos: At least one type ;-) I use Arduino Servo-Lib with TG9e- standard servo.
- Full Arduino-Compatible. I used an Arduino Uno

Tested and fully functional with Inkscape.

Installation:

- Upload Eggduino.ino with Arduino-IDE or similar tool to your Arudino (i.e. Uno)
- Disable Autoreset on Arduinoboard (there are several ways to do this... Which one does not matter...)
- Install Inkscape Tools wit Eggbot extension. Detailed instructions: (You yust need to complete Steps 1 and 2)          http://wiki.evilmadscientist.com/Installing_software

- Because of an bug in the Eggbot-extension (Function findEiBotBoards()), the Eggduino cannot be detected by default.     Hopefully, the guys will fix this later on. But we can fix it on our own.
    It is quiete easy:
    Instructions: Go to your Inkscape-Installationfolder and navigate to subfolder
    .\App\Inkscape\share\extensions
    open File "eggbot.py" in texteditor and search for:
    "Try any devices which seem to have EBB boards attached" delete or uncomment the following 6 lines
    (example: in Version 2.3.4 line 1353-1357)
    save file and enjoy your selfmade Eggbot/Spherebot with full Inkscape-Integration.
    Prepare some Eggs and have fun.

Todos and Feature-Wishlist:

- collision control via penMin/penMax
- implement homing sequence via microswitch or optical device
- implement hardware-button , EGGBOT-Guys call it "PRG-Button"
  
