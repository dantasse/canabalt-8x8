### canabalt-8x8

This is my attempt to recreate Adam Saltsman's great
[Canabalt](http://www.canabalt.com) on an 8x8 LED matrix.

So, without the sound, most of the graphics, special buildings, boxes you
can stumble on, glass windows, airplanes flying overhead, shuddering, zooming
in cameras, cool animations, or
[neat tweaks](http://blog.semisecretsoftware.com/tuning-canabalt). In short,
without most of what makes Canabalt awesome. So if you don't like it,
please don't judge a book by its tiny tiny clone. Go play (and buy!) real
[Canabalt](http://www.canabalt.com) instead.

Nevertheless, here's a [video of it in action.](https://www.youtube.com/watch?v=_63aN0GHZRk)

This is made on an Arduino Nano 3.0 with an [8x8 LED matrix from Ningbo Flying
Electronics](http://www.led-display-manufacturers.com/pid125397/104766901.htm).

Wiring is straightforward (although not simple): a digital output pin to each
row of the matrix, through a 470ohm resistor; a digital output pin to each
column of the matrix; and a tactile switch (push button) wired up as in
[this tutorial](http://arduino.cc/en/tutorial/button).

Class project for [05-833 Gadgets](http://www.hcii.cmu.edu/courses/applied-gadgets-sensors-and-activity-recognition-hci)
taught by Scott Hudson.  
