ChessBoard 2.05

To try this example, simply run the ChessClient.py

> python ChessClient.py

Keys:
- Left  ->  Undo last move.
- Right ->  Redo move.
- F     ->  Print current board as FEN.
- A     ->  Print all moves as Algebraic Notation.
- S     ->  Print all moves as Standard Algebraic Notation.
- L     ->  Print all moves as Long Algebraic Notation.

Please send andy comments and bug reports regarding the code to john@arainyday.se.

ChessBoard.py and ChessClient.py is made by John Eriksson (http://arainyday.se) and is released under the GPL.
Chess graphics are created by Paul Gorman (http://patzer.paulgorman.org). Read separate README.txt file in the img folder for information about licence.

Special thanks to Jasper Stolte for testing and suggesting improvements!


--------------------------------------------------------------------------------
Changes since 2.04
- Fixed a bug in the getFEN method. (Thanks to Alfredo Alessandrini for finding it)

Changes since 2.03
- Fixed a bug in the setFEN method. (Thanks to Alfredo Alessandrini for finding it)

Changes since 2.02
- Minor bugfix in the setFEN method.

Changes since 2.01
- Added the method getLastMove()
- Changed the behavior of the promotion value. The promotion value set by setPromotion is always remembered until 
  setPromotion(0) resets it.
- Added the method getPromotion() to get the current promotion value.
- Added the method getLastMoveType() to indicate if the last move was a "special move" like en passant or castling.
- Fixed a bug in getLastTextMove(...). It now returns the correct code for castling.
- Fixed a bug in the addMove() method. It pushed the state before it determined the game result causing it to loose the game result when using undo+redo. 

Changes since 2.0b

- Fixed a bug in resetBoard() and setFEN(). The game_result variable wasn't reset.
- Fixed a bug in addTextMove() that caused a crash sometimes when invalid data was entered.
- Added a new reson, AMBIGUOUS_MOVE, for text moves that could be perfomed by two different pieces.

Changes since v1.1:

- Fixed som bugs.
- Added the methods:    
    setFEN()
    getFEN()
    addTextMove()
    getAllTextMoves()
    getLastTextMove()
    getMoveCount()  
    getCurrentMove()
    gotoMove()
    gotoFirst()
    gotoLast()
    undo()
    redo()

Changes since v1.0:

- Fixed some bugs
- Added the methods getReason() and setPromotion().
