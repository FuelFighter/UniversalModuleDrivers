This folder contains tests to for the UM board:

test.c: tests if the board is working by making the RGB LED blinks.
test-UM-serial.c : tests if the serial port on the board works by sending messages through it.
read_CANmessages.c: allows the board to read messages from the CAN network and send them through the serial port where they can be viewed via CANviewer
