import serial
import time

ser = serial.Serial("COM9", 9600)

inD = ""
encodio = 0

while 1:
    while ser.in_waiting > 0:
        inByte = ser.read()

        inD = inD + inByte.decode()

    print(inD)

    inD = ""

    sendD = "This is a test of pySerial"

    print("Message to Send: %s" % sendD)

    sendD = sendD.encode(encoding="ascii")

    ser.write(sendD)
    
    time.sleep(1)
