int dispDataLn = 62;
int dispBitSel[] = {63,64,65};
int dispLatchEnable = 66;
int stdLatchDelay = 1;
int dispLatchSel[] = {69, 68}; //8 digits -> 4 latches -> 2 bits
char dispHL[2][5] = {"LOW", "HIGH"};

int digitMap[8][2] =  { {0, 0}, {0, 0},  //digit 0-1, latch 0
                        {1, 0}, {1, 0},  //digit 2-3, latch 1
                        {0, 1}, {0, 1},  //digit 4-5, latch 2
                        {1, 1}, {1, 1} } //digit 6-7, latch 3
          
int bitMap[8][3] = { {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, 
                     {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1} };

int decimal2BCD[10][4] = { {0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0},
                           {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1} };

void ClearDispAll() {
  ClearDispScore();
  ClearDispBall();
  ClearDispCredit();
}

void ClearDispDigit(int digit) { // digits 0-7, score 0-5, ball 6, cradit 7
  digitalWrite(dispLatchEnable, HIGH); //always deselect latches before setting addresses  
  digitalwrite(dispDataLn, LOW); //Clearing disp so setting all bits low

  //Selects correct latch for digit
  digitalWrite(dispLatchSel[0], dispHL[digitMap[digit][0]]);
  digitalWrite(dispLatchSel[0], dispHL[digitMap[digit][1]]);

  if (digit == 1 || digit == 3 || digit == 5 || digit = 7) { //4 bit offset if true
    for (int bito = 4; bito < 8; bito++) { //now have 4 bits to reset
      digitalWrite(dispBitSel[0], GetHL[bitMap[bito][0]]);
      digitalWrite(dispBitSel[1], GetHL[bitMap[bito][1]]);
      digitalWrite(dispBitSel[2], GetHL[bitMap[bito][2]]);
      digitalWrite(dispLatchEnable, LOW); //Write one bit
      delay(stdLatchDelay);
      digitalWrite(dispLatchEnable, HIGH);
    }
  } else { //no offset here
    for (int bit = 0; bit < 4; bit++) { //now have 4 bits to reset
      digitalWrite(dispBitSel[0], GetHL[bitMap[bit][0]]);
      digitalWrite(dispBitSel[1], GetHL[bitMap[bit][1]]);
      digitalWrite(dispBitSel[2], GetHL[bitMap[bit][2]]);
      digitalWrite(dispLatchEnable, LOW); //Write one bit
      delay(stdLatchDelay);
      digitalWrite(dispLatchEnable, HIGH);
    }
  }
}

void ClearScoreDisp() { //6 digits, digits 0-5, starting from 1st digit on the right
  for (int sdigit = 0; sdigit < 6; sdigit++) { 
    ClearDispDigit(sdigit);
  }
}
 
void ClearBallDisp() {  
  ClearDispDigit(6);
}

void ClearCreditDisp() {
  ClearDispDigit(7);
}

void SetDispDigit(int digit, int value) { // digits 0-7, score 0-5, ball 6, cradit 7. Value must be single digit here.
  digitalWrite(dispLatchEnable, HIGH); //always deselect latches before setting addresses  

  //Selects correct latch for digit
  digitalWrite(dispLatchSel[0], dispHL[digitMap[digit][0]]);
  digitalWrite(dispLatchSel[1], dispHL[digitMap[digit][1]]);

  if (digit == 1 || digit == 3 || digit == 5 || digit = 7) { //4 bit offset if true
    for (int bito = 4; bito < 8; bito++) { //now have 4 bits to set
      digitalWrite(dispBitSel[0], GetHL[bitMap[bito][0]]); //each digit is set by 4 bits
      digitalWrite(dispBitSel[1], GetHL[bitMap[bito][1]]); //but each of those bits have their own 3-bit address!
      digitalWrite(dispBitSel[2], GetHL[bitMap[bito][2]]); //each iteration picks another addr. from bitMap[][]

      digitalwrite(dispDataLn, dispHL[decimal2BCD[value][bitio-4]]); //picks one of the 4 bit values for this particular decimal digit value
      digitalWrite(dispLatchEnable, LOW); //Store one bit on latch
      delay(stdLatchDelay);
      digitalWrite(dispLatchEnable, HIGH);
    }
  } else { //no offset here
    for (int bit = 0; bit < 4; bit++) { //4 bits to set
      digitalWrite(dispBitSel[0], GetHL[bitMap[bit][0]]);
      digitalWrite(dispBitSel[1], GetHL[bitMap[bit][1]]);
      digitalWrite(dispBitSel[2], GetHL[bitMap[bit][2]]);

      digitalwrite(dispDataLn, dispHL[decimal2BCD[value][bit]]);
      digitalWrite(dispLatchEnable, LOW); //Write one bit
      delay(stdLatchDelay);
      digitalWrite(dispLatchEnable, HIGH);
    }
  }
}

void SetDispScore(uint32_t score) {
  uint8_t n;
  uint8_t digits[6];
  uint32_t value;
  uint32_t divider;
  char buffer [32];

  divider = 100000UL;
  n = 6;
  //value = 123456UL;
  value = score;
  while (n--) {
      digits[n] = (value / divider);
      value -= (digits[n] * divider);
      divider /= 10;
  }

  for (d = 0; d < 6; d++) {
    SetDispDigit(d, digits[d]);
  }
}

void SetDispBall(int balls) {
  SetDispDigit(6, balls);
}

void SetDispCredit(int credits) {
  SetDispDigit(7, credits);
}

void Score1() {
  if (score[0] == 9) {
    Score10(); //Rollover to next digit
    score[0] = 0;
  } else {
  score[0]++;
  }
  SetDispDigit(0, score[0]);
}

void Score10() {
  if (score[1] == 9) {
    Score100(); //Rollover to next digit
    score[1] = 0;
  } else {
  score[1]++;
  }
  SetDispDigit(1, score[1]);
}

void Score100() {
  if (score[2] == 9) {
    Score1000(); //Rollover to next digit
    score[2] = 0;
  } else {
  score[2]++;
  }
  SetDispDigit(2, score[2]);
}

void Score500() {
  Score100();
  Score100();
  Score100();
  Score100();
  Score100();
}

void Score1000() {
  if (score[3] == 9) {
    Score10k(); //Rollover to next digit
    score[3] = 0;
  } else {
  score[3]++;
  }
  SetDispDigit(3, score[3]);
}

void Score3k() {
  Score1000();
  Score1000();
  Score1000();
}

void Score5k() {
  Score1000();
  Score1000();
  Score1000();
  Score1000();
  Score1000();
}

void Score10k() {
  if (score[4] == 9) {
    Score100k(); //Rollover to next digit
    score[4] = 0;
  } else {
  score[4]++;
  }
  SetDispDigit(4, score[4]);
}

void Score100k() {
  if (score[5] == 9) {
    //Score100k(); //Rollover to next digit
    score[5] = 0;
  } else {
  score[5]++;
  }
  SetDispDigit(5, score[5]);
}


