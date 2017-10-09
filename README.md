# LED-animation
This is an Arduino code that listens to input commands sent through serial communication to control the animation of an Adafruit NeoPixel LED strip accordingly. Make sure you have the NeoPixel library installed and the appropriate hardware setup (more information at https://learn.adafruit.com/adafruit-neopixel-uberguide/overview).

I used a 1m 144 pixel LED strip, but the code can work with other NeoPixel strips.

This code is part of my Master's Thesis work entitled "Effective Non-Verbal Communication for Mobile Robots using Expressive Lights" by Kim Baraka available at http://ri.cmu.edu/publications/effective-non-verbal-communication-for-mobile-robots-using-expressive-lights/, used to communicate the internal states of an autonomous mobile service robot to humans at a distance.

The file of interest is in the folder Cobot_LED_improved, to be uploaded on an Arduino board and with which you can communicate using a serial command following the protocol described in Arduino_commands.txt

The folder Cobot_LED_Base contains the code to display navigation intent (left/right/straight) and color coding for a NeoPixel strip. It is appplication specific, so I would recommend sticking with Cobot_LED_improved which was designed to allow for high generalizability.

For more information on my work, visit kimbaraka.com
