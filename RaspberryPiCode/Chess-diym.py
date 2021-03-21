import serial
import time


if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.flush()
    
def getboardMessage():
    """ gets a text string from the board """
    print("\n Waiting for start up command from the Board")
    while True:
        if ser.in_waiting > 0:
            btxt = ser.readline().decode('utf-8').rstrip().lower()
            if btxt.startswith('heypi'):
                btxt = btxt[len('heypi'):]
                print(btxt)
                return btxt
                break
            else:
                continue
            
initialMessage = getboardMessage()

print ("Initial command received from Arduino")
print(initialMessage)

import StartChessGame
