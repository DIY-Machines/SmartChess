//v27 adding ability to start a new game
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>


//setup control panel LEDs
#define controlPanelLED_PIN 4
#define controlPanelLED_COUNT 22
Adafruit_NeoPixel controlPanelLED(controlPanelLED_COUNT, controlPanelLED_PIN, NEO_GRB + NEO_KHZ800);

//chessboard LEDs
Adafruit_NeoMatrix chessboardLEDS = Adafruit_NeoMatrix(8, 8, A5,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +    //either top left or bottom right
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
chessboardLEDS.Color(255, 0, 0), chessboardLEDS.Color(0, 255, 0), chessboardLEDS.Color(0, 0, 255) };

  
const int inputButtons[] = {3,5,6,7,8,9,10,11,12,A1};
const int buttonDebounceTime  = 300;

int currentBoard[8][8]; //an array to track the current state of the board
String piStarted = "No";

String humansMove;
String pisMove;
String pisSuggestedBestMove;
String gameMode = "0";  // can be either 'Stockfish' or 'OnlineHuman' once set
String colourChoice; // can be either 'black' or 'white'
bool legalOrNot;

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define ORANGE   0xFC00
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF 

uint32_t cpWHITE = controlPanelLED.Color(255, 255, 255);
uint32_t cpDimWHITE = controlPanelLED.Color(10, 10, 10);
uint32_t cpBLACK = controlPanelLED.Color(0, 0, 0);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  //setup the buttons for inputting moves
  for (int i=0; i<10; i++)  //the less then value needs to represent how many buttons we are using for this to complete enough loops
  {
    pinMode(inputButtons[i], INPUT_PULLUP);
  }

  //setup the hint interupt pin
  attachInterrupt(digitalPinToInterrupt(3), hint, FALLING);

  setUpBoard();

  //setup for the control panels neopixels
  controlPanelLED.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  controlPanelLED.show();            // Turn OFF all pixels ASAP
  controlPanelLED.setBrightness(255); // Set BRIGHTNESS (max = 255)

  //setup for chessboard neopixels
  chessboardLEDS.begin();
  chessboardLEDS.setTextWrap(false);
  chessboardLEDS.setBrightness(255);
  chessboardLEDS.setTextColor(colors[0]);
  chessboardLEDS.show();
  
  waitForPiToStart();
  setUpGame();
  showChessboardMarkings();
}

void loop() {
  // put your main code here, to run repeatedly:
  humansMove = humansGo();
  sendToPi(humansMove, "M");
  printBoard();
  lightUpMove(humansMove,'Y');
  Serial.println("Lighting up move");
  legalOrNot = checkPiForError();
  Serial.println("Checking legality");
  if (legalOrNot == false ){
    // do nothing and start the human request again
    Serial.println("Move discarded, please return the pieces and try another move");
    printBoard();
  } else {
    Serial.println("Move is legal");
    updateBoard(humansMove);
    printBoard();
    showChessboardMarkings();
    pisMove = receiveMoveFromPi();
    if (gameMode != "OnlineHuman"){
      pisSuggestedBestMove = pisMove.substring(5);
      Serial.println("Suggested next move = " + pisSuggestedBestMove);
    }
    lightUpMove(pisMove,'N');
    Serial.println(pisMove);
    updateBoard(pisMove);
    printBoard();
    if (gameMode != "OnlineHuman"){
      controlPanelLED.setPixelColor(5, 255, 255, 255); //light up the hint button on the control panel
      controlPanelLED.show();
      checkForComputerCheckMate(pisSuggestedBestMove, pisMove);
    }
    showChessboardMarkings();
  }
}

String humansGo(){
  bool moveConfirmed = false;
  int buttonPressed = 0;
  String moveFrom;
  String moveTo;
  String humansMove;

  while (moveConfirmed == false){ 
    controlPanelLED.fill(cpWHITE, 0, 4); //turn on the control panels co-ordinate lights
    controlPanelLED.show();
    moveFrom = getCoordinates(buttonPressed); //get move from the player via the buttons
    moveTo = getCoordinates(0);
    buttonPressed = 0; //clear the variable for another time around this while loop if required
    //Serial.println("You're moving the piece on " + moveFrom + " to " + moveTo);
    humansMove = moveFrom + moveTo; //combine the moves into one four character command
    controlPanelLED.setPixelColor(4, 255, 255, 255); //light up the OK button on the control panel
    controlPanelLED.setPixelColor(5, 0, 0, 0); //turrn off the hint button on the control panel
    controlPanelLED.show();
    lightUpMove(humansMove,'Y'); //and show the move entered to the player
    while (buttonPressed == 0){  //wait for them to approve it by pressing OK or enter new co-ordinates
      buttonPressed = detectButton();
    }
    if (buttonPressed == 9){
      controlPanelLED.fill(cpBLACK, 0, 6);; ////once OK'ed proceed as normal and switch of the OK, hint light and co-ordinates lights
      controlPanelLED.show();
      return humansMove;
    } else {
      showChessboardMarkings();
    }
    
  }
  
}

void sendToPi(String message, String messageType){
  String messageToSend = message;
  Serial.println("heypi" + messageType + messageToSend);
}

void updateBoard(String moveToUpdate){
  //Serial.print("Function:updateBoard - Piece to update: ");
  //Serial.println(moveToUpdate);
  
  int columnMovedFrom = columnNumber(moveToUpdate[0]);
  char rowMovedFrom = moveToUpdate[1];
  int irowMovedFrom = 7-(rowMovedFrom - '1');
  currentBoard[irowMovedFrom][columnMovedFrom] = 0;

  int columnMovedTo = columnNumber(moveToUpdate[2]);
  char rowMovedTo = moveToUpdate[3];
  int irowMovedTo = 7-(rowMovedTo - '1');
  currentBoard[irowMovedTo][columnMovedTo] = 1;
}

int columnNumber(char column){
  Serial.println("Function: columnNumber");
  //Serial.println(column);
  switch (column){
    case 'a':
    //Serial.println("Column A converted to number 0.");
    return 0;
    case 'b':
    return 1;
    case 'c':
    return 2;
    case 'd':
    //Serial.println("Column D converted to number 3.");
    return 3;
    case 'e':
    //Serial.println("Column E converted to number 4.");
    return 4;
    case 'f':
    return 5;
    case 'g':
    return 6;
    case 'h':
    return 7;
    default:
    Serial.println("No case statement found!");
  }
}

String receiveMoveFromPi(){
  Serial.print("Function:receiveMoveFromPi...   ");
    Serial.println("Waiting for response from Raspberry Pi");
    while (true){
      if (Serial.available() > 0){
        String data = Serial.readStringUntil('\n');
        Serial.println(data);
        if (data.startsWith("heyArduinom")){
          Serial.print("Move received from Pi: ");
          data = data.substring(11);
          Serial.println(data);
          return data;
        } else if (data.startsWith("heyArduinoerror")){
          errorFromPi();
          return "error";
        }
      }
    }
  }

void waitForPiToStart(){
    Serial.println("Function:waitForPiToStart...   ");
    showChessboardOpeningMarkings();
    int chessSquaresLit = 0;
    while (true){
      chessSquaresLit = loadingStatus(chessSquaresLit);
      delay(1000);
      if (Serial.available() > 0){
        String data = Serial.readStringUntil('\n');
        Serial.println(data);
        if (data.startsWith("heyArduinoChooseMode")){
          while (chessSquaresLit < 64){
            chessSquaresLit = loadingStatus(chessSquaresLit);
            delay(15);
          }
          //turn on the control panels lights
          controlPanelLED.fill(cpWHITE, 0, 5);
          controlPanelLED.show();
          while (true) {
            gameMode = detectButton();
            delay(buttonDebounceTime);
              if (gameMode == "1"){
                gameMode = "Stockfish";
                sendToPi(gameMode, "G");
                Serial.print("Pi is going to start a game with Stockfish.");
                chessboardLEDS.drawPixel(0,0,GREEN);
                chessboardLEDS.show();
                delay(500);
                chessboardLEDS.drawPixel(0,0,BLACK);
                chessboardLEDS.show();
                delay(500);
                chessboardLEDS.drawPixel(0,0,GREEN);
                chessboardLEDS.show();
                delay(500);
                chessboardLEDS.drawPixel(0,0,BLACK);
                chessboardLEDS.show();
                delay(500);
                break;
              } else if (gameMode == "2"){
                gameMode = "OnlineHuman";
                sendToPi(gameMode, "G");
                Serial.print("Pi is going to start a game playing online against another board.");
                Serial.println(gameMode);
                break;
              }    
            }
          }
          if (gameMode != "0"){
            break;
          }
        }
      }
    }

bool checkPiForError(){  //check five times during the next 03 seconds to see if we received an error from maxchess on the pi - if so run errorFromPi()
    Serial.print("Function:checkPiForError...   ");
    Serial.println("Waiting for response from Raspberry Pi");
    int checked = 0;
    while (checked<30){
      if (Serial.available() > 0){
        String data = Serial.readStringUntil('\n');
        Serial.println(data);
        if (data.startsWith("heyArduinoerror")){
          Serial.print("Error received from Pi: ");
          Serial.println(data);
          errorFromPi();
          return false;
         }
       } else {
        delay(100);
        Serial.println(checked);
        checked++;
      }
    }
    return true;
  }

void errorFromPi(){
  //Serial.println("Error received from Raspberry Pi");
  int times = 0;
  while (times < 3){
    chessboardLEDS.fillRect(0,0,8,8, BLUE);
    chessboardLEDS.show(); 
    delay(500);
    chessboardLEDS.drawLine(0, 7, 7, 0, RED);
    chessboardLEDS.drawLine(0, 0, 7, 7, RED);
    chessboardLEDS.show();
    delay(500);
    times++;   
  }
  showChessboardMarkings();
}


String getCoordinates(int buttonAlreadyPressed){
  //Serial.println("Getting co-ordinates...");
  int temp;
  String coordinates;
  String column = "x";
  String row = "x";
  
  while (column == "x"){
    //Serial.println("Waiting for user to input column via button press...");
    if (buttonAlreadyPressed != 0){
      temp = buttonAlreadyPressed;
    } else {
      temp = detectButton();
    }
    
    switch (temp){
      case 1:
      column = "a";
      break;
      case 2:
      column = "b";
      break;
      case 3:
      column = "c";
      break;
      case 4:
      column = "d";
      break;
      case 5:
      column = "e";
      break;
      case 6:
      column = "f";
      break;
      case 7:
      column = "g";
      break;
      case 8:
      column = "h";
      break;
      default:
      break;
    }
  }
  
  delay(buttonDebounceTime);
  
  while (row == "x"){
    //Serial.println("Waiting for user to input row via button press...");
    temp = detectButton();
    switch (temp){
      case 1:
      row = "1";
      break;
      case 2:
      row = "2";
      break;
      case 3:
      row = "3";
      break;
      case 4:
      row = "4";
      break;
      case 5:
      row = "5";
      break;
      case 6:
      row = "6";
      break;
      case 7:
      row = "7";
      break;
      case 8:
      row = "8";
      break;
      default:
      break;
    }
  }
  coordinates = column + row;
  //Serial.println("Co-ordinates are " + coordinates);
  delay(buttonDebounceTime);
  return coordinates;
}

int detectButton(){
  int val;
  //Serial.println("Waiting for a button to be detected...");
  while (true) {  
    if (digitalRead(5) == LOW){
      //Serial.println("Button 1 (for A/1) detected");
      val = 1;
      break;
    } else if (digitalRead(6) == LOW){
      //Serial.println("Button 2 (for B/2) detected");
      val = 2;
      break;
    } else if (digitalRead(7) == LOW){
      //Serial.println("Button 3 (for C/3) detected");
      val = 3;
      break;
    } else if (digitalRead(8) == LOW){
      //Serial.println("Button 4 (for D/4) detected");
      val = 4;
      break;
    } else if (digitalRead(9) == LOW){
      //Serial.println("Button 5 (for E/5) detected");
      val = 5;
      break;
    } else if (digitalRead(10) == LOW){
      //Serial.println("Button 6 (for F/6) detected");
      val = 6;
      break;
    } else if (digitalRead(11) == LOW){
      //Serial.println("Button 7 (for G/7) detected");
      val = 7;
      break;
    } else if (digitalRead(12) == LOW){
      //Serial.println("Button 8 (for H/8) detected");
      val = 8;
      break;
    } else if (digitalRead(A1) == LOW){
      //Serial.println("Button connected to A1 (for 'Yes') detected");
      val = 9;
      break;
    }
  }
  return val;
}

void printBoard(){
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      Serial.print(currentBoard[i][j]);
      Serial.print(",");
    }
    Serial.println();
  }
}

void setUpGame(){
  //turn on the control panels lights
  controlPanelLED.fill(cpWHITE, 0, 4);
  controlPanelLED.show();

 if (gameMode == "Stockfish"){
  String gameDifficulty;
  String gameTimeout;
  
  //Serial.print("Set difficulty level: ");
  chessboardLEDS.fill(BLACK, 0);
  chessboardLEDS.drawFastVLine(2,2,4,MAGENTA);
  chessboardLEDS.drawFastHLine(2,6,4,MAGENTA);
  chessboardLEDS.show();
  
  gameDifficulty = map(detectButton(), 1, 8, 1 ,20);
  delay(buttonDebounceTime);
  if (gameDifficulty.length() < 2){
    //Serial.println("Single digit");
    gameDifficulty = ("0" + gameDifficulty);
  }
  //Serial.println(gameDifficulty);
  sendToPi(gameDifficulty, "-");

  //Serial.print("Set computers move timout setting: ");
  chessboardLEDS.fill(BLACK, 0);
  chessboardLEDS.drawFastVLine(3,2,5,MAGENTA);
  chessboardLEDS.drawFastHLine(2,2,3,MAGENTA);
  chessboardLEDS.show();
  
  gameTimeout = map(detectButton(), 1, 8, 3000 ,12000);
  delay(buttonDebounceTime);
  //Serial.println(gameTimeout);
  sendToPi(gameTimeout, "-");
 } else if (gameMode == "OnlineHuman"){
  while (true){
    colourChoice = detectButton();
    delay(buttonDebounceTime);
    if (colourChoice == "1"){
      colourChoice = "White";
      Serial.println("Chossen colour is white");
      sendToPi(colourChoice, "C");
      break;
    } else if (colourChoice == "2"){
      colourChoice = "Black";
      Serial.println("Chossen colour is black");
      sendToPi(colourChoice, "C");
      break;
    }
  }
 }
  controlPanelLED.fill(cpDimWHITE, 6, 16);
  controlPanelLED.show();
}

void hint(){
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 200) 
 {
  if (digitalRead(A1) == LOW){
    Serial.println("Starting a new game....");
    sendToPi("","n");
    setUpBoard(); //reset all the chess piece positions
    controlPanelLED.setPixelColor(5, 0, 0, 0); //turn off the hint button on the control panel
    controlPanelLED.fill(cpWHITE, 0, 5);
    controlPanelLED.show();
    controlPanelLED.show();
    int var1 = 0;
    while (var1 < 64){
            var1 = loadingStatus(var1);
            delay(25);
          }
    delay(1000);
    showChessboardMarkings();
    return;
  } else if (digitalRead(12) == LOW){
    sendToPi("shutdown", "x");
  } else if (pisSuggestedBestMove.length() != 1){
    //Serial.println("Suggested best move= " + pisSuggestedBestMove);
    controlPanelLED.fill(cpBLACK, 0, 4);
    controlPanelLED.setPixelColor(5, 0, 0, 255); //light up the hint button on the control panel in blue
    controlPanelLED.show();
    lightUpMove(pisSuggestedBestMove,'H');
    showChessboardMarkings();
    controlPanelLED.setPixelColor(5, 255, 255, 255); //light up the hint button on the control panelin white again
    controlPanelLED.fill(cpWHITE, 0, 4);
    controlPanelLED.show();
  } else {
    Serial.println("No hint provided by pi yet.");
  }
 }
 last_interrupt_time = interrupt_time;
}





void checkForComputerCheckMate(String hint, String attacker){
  //Serial.println("Checking if the Pi has check mated you....");
  //Serial.println("Attacker=" + attacker);
  int attackerColumn = columnNumber(attacker[2]);
  //Serial.println(attackerColumn);
  char attackerRow = attacker[3];
  //Serial.println(attackerRow);

  
  int iattackerRow = 7-(attackerRow - '1');
  //Serial.println(attackerColumn);
  //Serial.println(iattackerRow);

  
  if (hint.length() < 3){
    chessboardLEDS.drawRect(0,0,8,8,RED);
    chessboardLEDS.show();
    delay(1000);
    chessboardLEDS.drawRect(1,1,6,6,RED);
    chessboardLEDS.show();
    delay(1000);
    chessboardLEDS.drawRect(2,2,4,4,RED);
    chessboardLEDS.show();
    delay(1000);
    chessboardLEDS.drawRect(3,3,2,2,RED);
    chessboardLEDS.show();
    delay(1000);

    int times = 0;
    while (times < 5) {
      showChessboardMarkings();
      delay(1000);
      chessboardLEDS.drawPixel(attackerColumn,iattackerRow,RED);
      chessboardLEDS.show();
      delay(1000);
      times++;
    }
  }
}

void setUpBoard(){
    //set up the inital starting positions of chess pieces in the array
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 8; j++) {
      currentBoard[i][j] = 1;
    }
  }
  for(int i = 2; i < 6; i++) {
    for(int j = 0; j < 8; j++) {
      currentBoard[i][j] = 0;
    }
  }
   for(int i = 6; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      currentBoard[i][j] = 1;
    }
  }
}
