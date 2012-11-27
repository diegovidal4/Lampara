import speech
import serial
import time
import sys


s=serial.Serial("COM9") 
def respuesta(f, l):
    speech.say(f)
    if f == "Uno":
        s.write("1")
    else:
        s.write("0")

l=speech.listenfor(['Uno','Dos'],respuesta)
raw_input()
