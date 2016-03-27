This is the fork of cocktailyogi/EggDuino firmware with added support for servo lowering/raising speed and pin assignment for Protoneer CNC Shield.


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
- Supported Servos: standard analog mini servos like TG9e, SG90, ES08MA, HXT900, etc.
- Full Arduino-Compatible. I used an Arduino Uno
- Button-support (3 buttons)

Tested and fully functional with Inkscape.

Installation:

- Upload Eggduino.ino with Arduino-IDE or similar tool to your Arudino (i.e. Uno)
- Disable Autoreset on Arduinoboard (there are several ways to do this... Which one does not matter...)
- Install Inkscape Tools wit Eggbot extension. Detailed instructions: (You yust need to complete Steps 1 and 2)
http://wiki.evilmadscientist.com/Installing_software

- Eggduino cannot be detected by default by the Eggbot-extension
	Hopefully, the guys will fix this later on. But we can fix it on our own.
	Go to your Inkscape-Installation folder and navigate to subfolder .\App\Inkscape\share\extensions
	
    For version 2.5.0:
		- open file "eggbot.py" in text editor and search for the line:
			"Try any devices which seem to have EBB boards attached"
        - comment that block with "#" like this:
                # Try any devices which seem to have EBB boards attached
				# for strComPort in eggbot_scan.findEiBotBoards():
				#	serialPort = self.testSerialPort( strComPort )
				#	if serialPort:
				#		self.svgSerialPort = strComPort
				#		return serialPort
		- In my version lines 1355-1360
		
	For version 2.7.1:
		- open file "ebb_serial.py" in text editor and search for the following block:
			
			EBBport = None
			for port in comPortsList:
				if port[1].startswith("EiBotBoard"):
					EBBport = port[0] 	#Success; EBB found by name match.
					break	#stop searching-- we are done.
			if EBBport is None:
				for port in comPortsList:
					if port[2].startswith("USB VID:PID=04D8:FD92"):
						EBBport = port[0] #Success; EBB found by VID/PID match.
						break	#stop searching-- we are done.		
						
		- replace "04D8:FD92" with the VID/PID of your Arduino device.	
		
		- alternatively, you can replace "EBBport = None" with your specific port number:
			EBBport = "COMxx"				#Windows
			EBBport = "/dev/tty[something]"	#Linux/Mac	

		
 
