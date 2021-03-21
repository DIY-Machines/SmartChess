void showChessboardMarkings(){
    for(int i = 0; i < 8; i=i+2) {
    for(int j = 0; j < 8; j++) {
       if (j % 2 == 0){
        chessboardLEDS.drawPixel(i,j,WHITE);
        chessboardLEDS.drawPixel(i+1,j,BLACK);
       }
       else{
        chessboardLEDS.drawPixel(i,j,BLACK);
        chessboardLEDS.drawPixel(i+1,j,WHITE);
       }
    }
  }
  chessboardLEDS.show();
}

int loadingStatus(int chessSquaresAlreadyLit){
  int targetRow = 0;
  int targetColumn = 0;
  if (chessSquaresAlreadyLit < 8){
    targetRow = 0;
    targetColumn = chessSquaresAlreadyLit;
  } else if (chessSquaresAlreadyLit > 7){
    targetRow = chessSquaresAlreadyLit / 8;
    Serial.println(targetRow);
    targetColumn = chessSquaresAlreadyLit % 8;
    Serial.println(targetColumn);
    Serial.println();
  }
  chessboardLEDS.drawPixel(targetColumn,targetRow,GREEN);
  chessboardLEDS.show();
  return chessSquaresAlreadyLit + 1;
  }



void showChessboardOpeningMarkings(){
   for(int i = 0; i < 8; i=i+2) {
    for(int j = 0; j < 2; j++) {
       if (j % 2 == 0){
        chessboardLEDS.drawPixel(i,j,WHITE);
        chessboardLEDS.drawPixel(i+1,j,BLACK);
       }
       else{
        chessboardLEDS.drawPixel(i,j,BLACK);
        chessboardLEDS.drawPixel(i+1,j,WHITE);
       }
    }
  }
   for(int i = 0; i < 8; i=i+2) {
    for(int j = 6; j < 8; j++) {
       if (j % 2 == 0){
        chessboardLEDS.drawPixel(i,j,WHITE);
        chessboardLEDS.drawPixel(i+1,j,BLACK);
       }
       else{
        chessboardLEDS.drawPixel(i,j,BLACK);
        chessboardLEDS.drawPixel(i+1,j,WHITE);
       }
    }
  }
  
  chessboardLEDS.show();
}

void lightUpMove(String moveToUpdate, char typeOfLight){    // typeOfLight options Y=AutoContinue N=user needs to dismiss H=hinted move
  //Serial.print("Function:lightUpMove - Move to display: ");
  //Serial.println(moveToUpdate);
  int times = 0;
  int columnMovedFrom = columnNumber(moveToUpdate[0]);
  char rowMovedFrom = moveToUpdate[1];
  int irowMovedFrom = 7-(rowMovedFrom - '1');

  int columnMovedTo = columnNumber(moveToUpdate[2]);
  char rowMovedTo = moveToUpdate[3];
  int irowMovedTo = 7-(rowMovedTo - '1');

  //show where it's moving from
  if (typeOfLight == 'H'){
    chessboardLEDS.drawPixel(columnMovedFrom,irowMovedFrom,CYAN); //show which piece is moving
  } else {
    chessboardLEDS.drawPixel(columnMovedFrom,irowMovedFrom,GREEN); //show which piece is moving
  }
  
  //show where it's moving to and if it is taking a piece or not
  if (currentBoard[irowMovedTo][columnMovedTo] == 1){
    while (times < 3) {
      chessboardLEDS.drawPixel(columnMovedTo,irowMovedTo,RED);
      chessboardLEDS.show();
      delay(200);
      chessboardLEDS.drawPixel(columnMovedTo,irowMovedTo,BLACK);
      chessboardLEDS.show();
      delay(200);
      times++;
    }
    chessboardLEDS.drawPixel(columnMovedTo,irowMovedTo,RED);
    chessboardLEDS.show();
  } else if (currentBoard[irowMovedTo][columnMovedTo] == 0) {
    //Serial.println("The square the piece moves into is not occupied.");
    if (typeOfLight == 'H'){
      chessboardLEDS.drawPixel(columnMovedTo,irowMovedTo,CYAN);
    } else {
      chessboardLEDS.drawPixel(columnMovedTo,irowMovedTo,GREEN);
    }
    chessboardLEDS.show();
  }
  if (typeOfLight == 'N'){
    controlPanelLED.setPixelColor(4, 255, 255, 255); //light up the OK button on the control panel
    controlPanelLED.show();
    while (detectButton() != 9){
      delay(1);
    }
    controlPanelLED.setPixelColor(4, 0, 0, 0); //turn off the OK button light on the control panel
    controlPanelLED.show();
  } else if (typeOfLight == 'H'){
    delay(4000);
  } else if (typeOfLight == 'Y'){
    //delay(1000);
  }
  //chessboardLEDS.show();
  //delay(200); 
}
