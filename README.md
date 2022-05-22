Eggduino
====

Arduino Firmware for Eggbot / Spherebot with Inkscape-Integration

Version 1.6a
tested with Inkscape Portable 0.91, Eggbot Extension and patched eggbot.py

Regards: Eggduino-Firmware by Joachim Cerny, 2015

Thanks for the nice libs ACCELSTEPPER and SERIALCOMMAND, which made this project much easier. Thanks to the Eggbot-Team for such a funny and enjoyable concept! Thanks to my wife and my daughter for their patience. :-)

Features:

- Implemented Eggbot-Protocol-Version 2.1.0
- Turn-on homing: switch-on position of pen will be taken as reference point.
- No collision-detection!!
- Supported Servos: At least one type ;-) I use Arduino Servo-Lib with TG9e- standard servo.
- Full Arduino-Compatible. I used an Arduino Uno
- Button-support (3 buttons)

Tested and fully functional with Inkscape.

Installation:

- Upload Eggduino.ino with Arduino-IDE or similar tool to your Arudino (i.e. Uno)
- Disable Autoreset on Arduinoboard (there are several ways to do this... Which one does not matter...)
- Install Inkscape Tools wit Eggbot extension. Detailed instructions: (You yust need to complete Steps 1 and 2)
http://wiki.evilmadscientist.com/Installing_software

- Because of an bug in the Eggbot-extension (Function findEiBotBoards()), the Eggduino cannot be detected by default.
	Hopefully, the guys will fix this later on. But we can fix it on our own.
    It is quiete easy:
	
        - Go to your Inkscape-Installationfolder and navigate to subfolder .\App\Inkscape\share\extensions
		- open File "eggbot.py" in texteditor and search for line:
			"Try any devices which seem to have EBB boards attached"
                - comment that block with "#" like this:
                		# Try any devices which seem to have EBB boards attached
				# for strComPort in eggbot_scan.findEiBotBoards():
				#	serialPort = self.testSerialPort( strComPort )
				#	if serialPort:
				#		self.svgSerialPort = strComPort
				#		return serialPort
		- In my version lines 1355-1360
 
- In a more recent version of the Inkscape Eggbot-extension, the detection is done by the `findPort()` function in ebb_serial.py:57 (v0.3 - June 28, 2016), which filters on VID:PID.
The first `startswith()` checks for an Eibot board; just add a second one with the VID/PID of your board, in my case "2341:0043" for an Arduino Uno.
You can find it with lsusb.
```
                if EBBport is None:
                        for port in comPortsList:
                                if port[2].startswith("USB VID:PID=04D8:FD92")\
                                or port[2].startswith("USB VID:PID=2341:0043"): # Eggduino on Arduino Uno 
                                        EBBport = port[0] #Success; EBB found by VID/PID match.
                                        break   #stop searching-- we are done.                          
```
- Another way to make an Arduino Uno be detected
