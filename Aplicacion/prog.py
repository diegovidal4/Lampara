import speech
import serial
import time
import sys
s=serial.Serial(sys.argv[1]) 
def respuesta(f, l):
    if f == "Prende":
        s.write("1")
        print("ON")
    else:
        s.write("0")
        print("OFF")

l=speech.listenfor(['Prende','Apaga'],respuesta)
raw_input()
