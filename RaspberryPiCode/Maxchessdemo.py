#
#This program plays chess using Stockfish the open source chess engine, using the ChessBoard library to manage the board.
# it is written in Python 2.7 because chessboard is. Python 3 might work but I havn't tested it.
# it assumes you have got the python libraries chessboard, subprocess and time
# it assumes stockfish is looded in the python directory.
# In this program the routines Getboard and Sendboard get a move in the for me2e4, by simple keyboard input. In the working system these get replaced by serial coms with the board
# it runs Stockfish using a list of moves not FEN. I couldn't get the FEN routines to work in ChessBoard fails after h2 h4 so #'d out
# Instead they use a move list, which is less eligent, but works.
# See full working system at : www.chess.fortherapy.co.uk
# to start try running the program and type me2e4 at the first prompt.
# This program is built using lots of examples from around the web, so do what you want with it. A credit to www.chess.fortherapy.co.uk would be nice.



# initiate chessboard
from ChessBoard import ChessBoard
import subprocess, time
maxchess = ChessBoard()


# initiate stockfish chess engine

engine = subprocess.Popen(
    'stockfish',
    universal_newlines=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,)

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
    btxt = raw_input("\n Enter a board message: ").lower()
    return btxt
    
def sendboard(stxt):
    """ sends a text string to the board """
    print("\n send to board: " +stxt)

def newgame():
    get ()
    put('uci')
    get ()
    put('setoption name Skill Level value ' +skill)
    get ()
    put('setoption name Hash value 128')
    get()
    put('setoption name Best Book Move value true')
    get()
    put('setoption name OwnBook value true')
    get()
    put('uci')
    get ()
    put('ucinewgame')
    maxchess.resetBoard()
    fmove=""
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
                        sendboard(etxt)
                        return fmove
                       
#  elif valid  make the move and send Fen to board
    
    else:
        maxchess.printBoard()
        # maxfen = maxchess.getFEN()
        # sendboard(maxfen)
       # remove line below when working
        raw_input("\n\nPress the enter key to continue")
        print ("fmove")
        print(fmove)
        print ("brdmove")
        print(brdmove)
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
                        stxt = "e"+ str(maxchess.getReason())+move
                        maxchess.printBoard()
                        sendboard(stxt)

        else:
                        temp=fmove
                        fmove =temp+" " +smove
                        stx = smove+hint      
                        sendboard(stx)
                        maxchess.printBoard()
                        # maxfen = maxchess.getFEN()
                        print ("computer move: " +smove)
                        return fmove
        

def put(command):
    print('\nyou:\n\t'+command)
    engine.stdin.write(command+'\n')



# assume new game
print ("\n Chess Program \n")
skill = "10"
movetime = "6000"
fmove = newgame()
while True:

    # Get  message from board
    bmessage = getboard()
    # Message options   Move, Newgame, level, style
    code = bmessage[0]
    
   
    
    # decide which function to call based on first letter of txt
    fmove=fmove
    if code == 'm': fmove = bmove(fmove)
    elif code == 'n': newgame()
    elif code == 'l': level()
    elif code == 's': style()
    else :  sendboard('error at option')




