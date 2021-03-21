# Interactive Chessboard - www.DIYmachines.co.uk
# This codes includes large sections kindly shared on www.chess.fortherapy.co.uk, which itself incorporates alot of other peoples code.
# Please feel free to modify, adapt and share. Any excisting licenses included must remain intact as well as nod to those who have contribued.
# This program plays chess using Stockfish the open source chess engine, using the ChessBoard library to manage the board.
# It is written in Python 2.7 because chessboard is.
# It assumes you have got the python libraries chessboard, subprocess and time


# initiate chessboard
from ChessBoard import ChessBoard
import subprocess, time, serial
maxchess = ChessBoard()

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)   # for Pi Zero use '/dev/ttyAMA0' and for others use '/dev/ttyUSB0'.
    ser.flush()

# initiate stockfish chess engine

engine = subprocess.Popen(
    'stockfish',
    universal_newlines=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,)

updateScriptToRun = ["python3", "update-online.py"]


remotePlayer = subprocess.Popen(updateScriptToRun,
    universal_newlines=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
                                )

def putAdafruit(command):
    print('\nTrying to send to AdafruitIO:\n\t'+command)
    remotePlayer.stdin.write("send\n")
    remotePlayer.stdin.write(command+'\n')
    while True :
        text = remotePlayer.stdout.readline().strip()
        if text[0:11] == 'Piece moved':
            print(text)
            break


def getAdafruit():
    print('\nTrying to read remote boards move from AdafruitIO:\n\t')
    remotePlayer.stdin.write("receive\n")
    while True :
        text = remotePlayer.stdout.readline().strip()
        #if text[0:11] == 'Piece moved':
        print(text)
        print('Was the remote move')
        return text


def getboard():
    """ gets a text string from the board """
    print("\n Waiting for command from the Board")
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


def sendtoboard(stxt):
    """ sends a text string to the board """
    print("\n Sent to board: heyArduino" + stxt)
    stxt = bytes(str(stxt).encode('utf8'))
    time.sleep(2)
    ser.write(b"heyArduino" + stxt + "\n".encode('ascii'))


def newgame():
    maxchess.resetBoard()
    maxchess.setPromotion(maxchess.QUEEN)
    print("Promotion set to ")
    print(maxchess.getPromotion())
    fmove=""
    brdmove=""
    putAdafruit("ready")
    return fmove





def bmove(fmove):
    """ assume we get a command of the form ma1a2 from board"""
    fmove=fmove
    print ("F move is now set to ")
    print(fmove)
    # Get a move from the board
    brdmove = bmessage[1:5].lower()
    print ("Brdmove is now set to ")
    print(brdmove)
    # now validate move
    # if invalid, get reason & send back to board
    #  maxchess.addTextMove(move)
    if maxchess.addTextMove(brdmove) == False :
        print("The move is illegal")
        etxt = "error"+ str(maxchess.getReason())+brdmove
        maxchess.printBoard()
        sendtoboard(etxt)
        return fmove

#  elif valid  make the move and send to board

    else:
        print("The move is legal")
        maxchess.printBoard()
        print ("brdmove")
        print(brdmove)
        putAdafruit(brdmove)

        fmove =fmove+" " +brdmove

        cmove = "position startpos moves"+fmove
        print (cmove)


        #retrive the rmeote boards move.


        text = getAdafruit()
        print (text)
        smove = text
        hint = "xxxx"
        temp=fmove
        fmove =temp+" " +smove
        stx = smove+hint
        maxchess.printBoard()
        print ("Remote players move: " +smove)
        smove ="m"+smove
        sendtoboard(smove +"-"+ hint)
        return fmove


def put(command):
    print('\nyou:\n\t'+command)
    engine.stdin.write(command+'\n')

def sendToScreen(line1,line2,line3,size = '14'):
    """Send three lines of text to the small OLED screen"""
    screenScriptToRun = ["python3", "printToOLED.py", '-a '+ line1, '-b '+ line2, '-c '+ line3, '-s '+ size]
    subprocess.Popen(screenScriptToRun) 
    
time.sleep(1)
sendtoboard("ReadyOnlinePlay")

sendToScreen ('This','Small','Fish')
time.sleep(3)
sendToScreen ('Select a colour:','1 = White/First','2 = Black/Second')


# get intial settings (such as level etc)
print ("Waiting for initial command to be received from Arduino")
initialMessage = getboard()

print ("Initial command received from Arduino")
print(initialMessage)

skillFromArduino = initialMessage[1:3].lower()
print ("Requested skill level:")
print(skillFromArduino)
movetimeFromArduino = initialMessage[3:].lower()
print ("Requested time out setting:")
print(movetimeFromArduino)


# assume new game
print ("\n Chess Program \n")
skill = skillFromArduino
movetime = movetimeFromArduino #6000
fmove = newgame()
while True:

    # Get  message from board
    bmessage = getboard()
    print ("Move command received from Arduino")
    print(bmessage)
    # Message options   Move, Newgame, level, style
    code = bmessage[0]



    # decide which function to call based on first letter of txt
    fmove=fmove
    if code == 'm':
        print("Fmove is currently: ")
        print(fmove)
        fmove = bmove(fmove)
    elif code == 'n':
        fmove = newgame()
    #elif code == 'l':
    #    level()
    #elif code == 's':
    #    style()
    else :
        sendtoboard('error at option')
