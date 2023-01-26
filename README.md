# Nibble Light

aka the most expensive rgb light possible

I unfortunately absolutely destroyed my nibble while desoldering some stuff trying to fix a bug. The LED side of things worked fine, but the IC/keyboard side was irreparable. I learnt my lesson and got a replacement board, but I didn't want the old one to go to waste. So, I decided to make it into an rgb light sorta thing.

![example image](/image.jpg)

It uses the Rotary Encoder to drive the rgb leds in the back. I also recommend keeping the acryllic cover on since the lights are fairly spaced out.

If you somehow find yourself in a similar situation, connect one lead of the rotary encoder's button to ground and the other to digital pin 6. Once that is done, it should be able to be controlled by it. Additionally if you used a bit-c, you might want to flash the Caterina bootloader for easier programming.

## Control from Computer

If you want to control it from your computer, you'll want to hook it up to a serial port and send it some commands: 

- e - toggle enable
- v+/- - change value
- h+/- - change hue

I've set up an autohotkey script to do this automatically for F20-F24 on serial port COM9
