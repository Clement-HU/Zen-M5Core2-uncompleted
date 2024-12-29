# Zen-M5Core2

A library for **M5Stack Core2** to run ZEN files.


## About ZEN

ZEN is a veeeeeeeeeeeeeeeeeery-lightweight language, which can be easily customized. 

## Why ZEN?
- ZEN is truly FAST! Its speed is much higher than micropython.
- Flexible. Modifying basic code(which is in C++) can make it able to do **ANYTHING**.

## Why NOT ZEN?
- ZEN is very complicated and low-level. For Example, using the original M5Core2 library to draw a line may look like this:
```M5.Lcd.drawLine(0,0,240,240,0xffff);```
But in ZEN-M5Core2, it looks like this...
```DL 0 0 240 240 255 255 255```
Moreover, if you want to use variables, you should allocate some memory first.
```VA I 10``` -> Allocates 10 int variables.
- ZEN has very strict format limits. An additional whitespace causes the program to crash, or run incorrectly.

## Fun facts
Well...you can actually do this.
```VA I 1 VM I 0 0 DL 0 0 240 IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII0 255 255 255 DU $Q```
It draws a line from (0,0) to (240,0), because the ```IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII0```
works as 0 since Int 0 is 0.

## License
Not licensed yet...(pity)