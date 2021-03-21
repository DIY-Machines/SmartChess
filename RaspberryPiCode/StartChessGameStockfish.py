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
    stdout=subprocess.PIPE
    )

def get():

    # using the 'isready' command (engine has to answer 'readyok')
    # to indicate current last line of stdout
    stx=""
    engine.stdin.write('isready\n')
    print('\nengine:')
    while True :
        text = engine.stdout.readline().strip()
        if text == 'readyok':
            break
        if text !='':
            print('\t'+text)
        if text[0:8] == 'bestmove':

            return text
def sget():

    # using the 'isready' command (engine has to answer 'readyok')
    # to indicate current last line of stdout
    stx=""
    engine.stdin.write('isready\n')
    print('\nengine:')
    while True :
        text = engine.stdout.readline().strip()
        #if text == 'readyok':
         #   break
        if text !='':
            print('\t'+text)
        if text[0:8] == 'bestmove':
            mtext=text
            return mtext

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
    #btxt = raw_input("\n Enter a board message: ").lower()
    #btxt = line + "/r"


def sendboard(stxt):
    """ sends a text string to the board """
    print("\n Sent to board: heyArduino" + stxt)
    stxt = bytes(str(stxt).encode('utf8'))
    time.sleep(2)
    ser.write(b"heyArduino" + stxt + "\n".encode('ascii'))


def newgame():
    sendToScreen ('NEW','GAME','','30')
    get ()
    put('uci')
    get ()
    put('setoption name Skill Level value ' +skill)
    get ()
    put('setoption name Hash value 128')
    get()
    #put('setoption name Best Book Move value true')
    #get()
    #put('setoption name OwnBook value true')
    #get()
    #put('uci')
    #get ()
    put('ucinewgame')
    maxchess.resetBoard()
    maxchess.setPromotion(maxchess.QUEEN)
    print("Promotion set to ")
    print(maxchess.getPromotion())
    fmove=""
    brdmove=""
    time.sleep(2)
    sendToScreen ('Please enter','your move:','')
    return fmove





def bmove(fmove):
    """ assume we get a command of the form ma1a2 from board"""
    fmove=fmove
    # Get a move from the board
    brdmove = bmessage[1:5].lower()
    # now validate move
    # if invalid, get reason & send back to board
      #  maxchess.addTextMove(move)
    if maxchess.addTextMove(brdmove) == False :
                        etxt = "error"+ str(maxchess.getReason())+brdmove
                        maxchess.printBoard()
                        sendToScreen ('Illegal move!','Enter new','move...','14')
                        sendboard(etxt)
                        return fmove

#  elif valid  make the move and send Fen to board

    else:
        maxchess.printBoard()
        # maxfen = maxchess.getFEN()
        # sendboard(maxfen)
       # remove line below when working
        #raw_input("\n\nPress the enter key to continue")
        #print ("fmove")
        #print(fmove)
        print ("brdmove")
        print(brdmove)
        sendToScreen (brdmove[0:2] + '->' + brdmove[2:4] ,'','Thinking...','20')

        fmove =fmove+" " +brdmove

        cmove = "position startpos moves"+fmove
        print (cmove)

            #        if fmove == True :
            #                move = "position startpos moves "+move
            #        else:
            #               move ="position fen "+maxfen

        # put('ucinewgame')
        # get()


        put(cmove)
        # send move to engine & get engines move


        put("go movetime " +movetime)
        # time.sleep(6)
        # text = get()
        # put('stop')
        text = sget()
        print (text)
        smove = text[9:13]
        hint = text[21:25]
        if maxchess.addTextMove(smove) != True :
                        stxt = "e"+ str(maxchess.getReason())+smove
                        maxchess.printBoard()
                        sendboard(stxt)

        else:
                        temp=fmove
                        fmove =temp+" " +smove
                        stx = smove+hint
                        maxchess.printBoard()
                        # maxfen = maxchess.getFEN()
                        print ("computer move: " +smove)
                        sendToScreen (smove[0:2] + '->' + smove[2:4] ,'','Your go...','20')
                        smove ="m"+smove
                        sendboard(smove +"-"+ hint)
                        return fmove


def put(command):
    print('\nyou:\n\t'+command)
    engine.stdin.write(command+'\n')
    
def sendToScreen(line1,line2,line3,size = '14'):
    """Send three lines of text to the small OLED screen"""
    screenScriptToRun = ["python3", "printToOLED.py", '-a '+ line1, '-b '+ line2, '-c '+ line3, '-s '+ size]
    subprocess.Popen(screenScriptToRun) 

time.sleep(1)
sendboard("ReadyStockfish")
time.sleep(2)

# get intial settings (such as level)
print ("Waiting for level command to be received from Arduino")
sendToScreen ('Choose computer','difficulty level:','(0 -> 8)')
skillFromArduino = getboard()[1:3].lower()
print ("Requested skill level:")
print(skillFromArduino)

# get intial settings (such as move time)
print ("Waiting for move time command to be received from Arduino")
sendToScreen ('Choose computer','move time:','(0 -> 8)')
movetimeFromArduino = getboard()[1:].lower()
print ("Requested time out setting:")
print(movetimeFromArduino)


# assume new game
print ("\n Chess Program \n")
sendToScreen ('NEW','GAME','','30')
time.sleep(2)
sendToScreen ('Please enter','your move:','')
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
        fmove = bmove(fmove)
    elif code == 'n':
        fmove = newgame()
    #elif code == 'l':
    #    level()
    #elif code == 's':
    #    style()
    else :
        sendboard('error at option')
