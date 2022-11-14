int dispDataLn = 47;
int dispBitSel[] = {49,51, 53};
int dispLatchEnable = 46;
int stdDispLatchDelay = 0;
int dispLatchSel[] = {52, 50}; //8 digits -> 4 latches -> 2 bits. L0, L1
//char dispHL[2][5] = {"LOW", "HIGH"};
int score[] = {0, 0, 0, 0, 0, 0};
int attractDispCnt = 1;

int digitMap[8][2] =  { {0, 0}, {0, 0},   //digit 0-1, latch 0
                        {1, 0}, {1, 0},   //digit 2-3, latch 1
                        {0, 1}, {0, 1},   //digit 4-5, latch 2
                        {1, 1}, {1, 1} }; //digit 6-7, latch 3
          
int bitMap[8][3] = { {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, 
                     {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1} };

int decimal2BCD[10][4] = { {0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0},
                           {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1} };



void ClearDispDigit(int digit) { // digits 0-6, score 0-5, ball 6, credit is addressed directly
  digitalWrite(dispLatchEnable, HIGH); //always deselect latches before setting addresses  
  digitalWrite(dispDataLn, LOW); //Clearing disp so setting all bits low

  //Selects correct latch for digit 
  if (digitMap[digit][0] == 0) {
    //Serial.println("lampMap[0] == 0");  
    digitalWrite(dispLatchSel[0], LOW);              
  } else {
    digitalWrite(dispLatchSel[0], HIGH);
  }
  if (digitMap[digit][1] == 0) {
    //Serial.println("lampMap[1] == 0");
    digitalWrite(dispLatchSel[1], LOW);
  } else {
    digitalWrite(dispLatchSel[1], HIGH);
  } 
  
  if (digit == 1 || digit == 3 || digit == 5 || digit == 7) { //4 bit offset if true
    Serial.println("ClearDispdigit. bit offset true");      
    for (int bito = 4; bito < 8; bito++) { //now have 4 bits to reset
      //Setting bit adr.
      if (bitMap[bito][0] == 0) {
        digitalWrite(dispBitSel[0], LOW);              
      } else {
        digitalWrite(dispBitSel[0], HIGH);
      }
      if (bitMap[bito][1] == 0) {
        //Serial.println("lampMap[4] == 0");
        digitalWrite(dispBitSel[1], LOW);
      } else {
        digitalWrite(dispBitSel[1], HIGH);
      } 
      if (bitMap[bito][2] == 0) {
        digitalWrite(dispBitSel[2], LOW);
      } else {
        digitalWrite(dispBitSel[2], HIGH);
      }    
      
      delay(stdDispLatchDelay);      
      digitalWrite(dispLatchEnable, LOW); //Write one bit
      delay(stdDispLatchDelay);
      digitalWrite(dispLatchEnable, HIGH);
    }
  } else { //no offset here
    Serial.println("ClearDispdigit. bit offset false");        
    for (int bit = 0; bit < 4; bit++) { //now have 4 bits to reset
      //Setting bit adr.
      if (bitMap[bit][0] == 0) {
        digitalWrite(dispBitSel[0], LOW);              
      } else {
        digitalWrite(dispBitSel[0], HIGH);
      }
      if (bitMap[bit][1] == 0) {
        //Serial.println("lampMap[4] == 0");
        digitalWrite(dispBitSel[1], LOW);
      } else {
        digitalWrite(dispBitSel[1], HIGH);
      } 
      if (bitMap[bit][2] == 0) {
        digitalWrite(dispBitSel[2], LOW);
      } else {
        digitalWrite(dispBitSel[2], HIGH);
      }    
      
      delay(stdDispLatchDelay);      
      digitalWrite(dispLatchEnable, LOW); //Write one bit
      delay(stdDispLatchDelay);
      digitalWrite(dispLatchEnable, HIGH);      
    }
  }
  //digitalWrite(dispLatchSel[1], LOW);
}

void ClearDispScore() { //6 digits, digits 0-5, starting from 1st digit on the right
  for (int sdigit = 0; sdigit < 6; sdigit++) { 
    ClearDispDigit(sdigit);
  }
}
 
void ClearDispBall() {  
  ClearDispDigit(6);
}

void ClearDispCredit() {
  digitalWrite(42, LOW);
  digitalWrite(43, LOW);
  digitalWrite(44, LOW);
  digitalWrite(45, LOW);
}

void SetDispCredit(int value) { //Values 0-9
  if (decimal2BCD[value][0] == 0) {
    digitalWrite(42, LOW);              
  } else {
    digitalWrite(42, HIGH);    
  }

  if (decimal2BCD[value][1] == 0) {
    digitalWrite(43, LOW);              
  } else {
    digitalWrite(43, HIGH);    
  } 
  
  if (decimal2BCD[value][2] == 0) {
    digitalWrite(44, LOW);              
  } else {
    digitalWrite(44, HIGH);    
  } 
  
  if (decimal2BCD[value][3] == 0) {
    digitalWrite(45, LOW);              
  } else {
    digitalWrite(45, HIGH);    
  }
}

void SetDispDigit(int digit, int value) { // digits 0-7, score is 0-5, ball 6, credit 7. Value must be single digit here.
  Serial.println("SetDispDigit"); 
  if (digit < 0 || digit > 7 || value < 0 || value > 9) {
    Serial.println(" ");
    Serial.println("error in SetDispDigit, digit, value:");
    Serial.println(digit);
    Serial.println(value);
    delay(5000);
  }
  digitalWrite(dispLatchEnable, HIGH); //always deselect latches before setting addresses  

  //Selects correct latch for digit 
  if (digitMap[digit][0] == 0) {
    digitalWrite(dispLatchSel[0], LOW);              
  } else {
    digitalWrite(dispLatchSel[0], HIGH);
  }
  if (digitMap[digit][1] == 0) {
    digitalWrite(dispLatchSel[1], LOW);
  } else {
    digitalWrite(dispLatchSel[1], HIGH);
  } 

  if (digit == 1 || digit == 3 || digit == 5 || digit == 7) { //4 bit offset if true
    Serial.println("SetDispDigit. bit offset true");    
    for (int bito = 4; bito < 8; bito++) { //now have 4 bits to set
                                           //each digit is set by 4 bits
                                           //but each of those bits have their own 3-bit address!
                                           //each iteration picks another addr. from bitMap[][]
      //Setting bit adr.
      if (bitMap[bito][0] == 0) {
        digitalWrite(dispBitSel[0], LOW);              
      } else {
        digitalWrite(dispBitSel[0], HIGH);
      }
      if (bitMap[bito][1] == 0) {
        digitalWrite(dispBitSel[1], LOW);
      } else {
        digitalWrite(dispBitSel[1], HIGH);
      } 
      if (bitMap[bito][2] == 0) {
        digitalWrite(dispBitSel[2], LOW);
      } else {
        digitalWrite(dispBitSel[2], HIGH);
      }

      if (decimal2BCD[value][bito-4] == 0) { //picks one of the 4 bits for this particular decimal digit value
        digitalWrite(dispDataLn, LOW);
      } else {
        digitalWrite(dispDataLn, HIGH);
      }    
     
      delay(stdDispLatchDelay);
      digitalWrite(dispLatchEnable, LOW); //Store one bit on latch
      delay(stdDispLatchDelay);
      digitalWrite(dispLatchEnable, HIGH);
    }
  } else { //no offset here
    Serial.println("SetDispDigit. bit offset false");   
    for (int bit = 0; bit < 4; bit++) { //4 bits to set
   
      if (bitMap[bit][0] == 0) {
        digitalWrite(dispBitSel[0], LOW);              
      } else {
        digitalWrite(dispBitSel[0], HIGH);
      }
      if (bitMap[bit][1] == 0) {
        digitalWrite(dispBitSel[1], LOW);
      } else {
        digitalWrite(dispBitSel[1], HIGH);
      } 
      if (bitMap[bit][2] == 0) {
        digitalWrite(dispBitSel[2], LOW);
      } else {
        digitalWrite(dispBitSel[2], HIGH);
      }

      if (decimal2BCD[value][bit] == 0) { //picks one of the 4 bits for this particular decimal digit value
        digitalWrite(dispDataLn, LOW);
      } else {
        digitalWrite(dispDataLn, HIGH);
      }    
     
      delay(stdDispLatchDelay);
      digitalWrite(dispLatchEnable, LOW); //Store one bit on latch
      delay(stdDispLatchDelay);
      digitalWrite(dispLatchEnable, HIGH);
    }
  }
}

void ClearDispAll() {
  ClearDispScore();
  ClearDispBall();
  ClearDispCredit();
}


void SetDispBall(int balls) {
  SetDispDigit(6, balls);
}

void Score100k() {
  if (score[5] == 9) {
    //No more digits to roll over to, so disp. will just stay at 9xxxxx once it maxes out. 
    //Another solution would be to add a "rollover" lamp, and reset this digit when score rolls over.
  } else {
    score[5]++; SetDispDigit(5, score[5]);
  }
}

void Score10k() {
  if (score[4] == 9) {
    score[4] = 0; SetDispDigit(4, score[4]);
    Score100k(); //Rollover to next digit
  } else {
    score[4]++; SetDispDigit(4, score[4]);
  }
}

void Score1k() {
  if (score[3] == 9) {
    score[3] = 0; SetDispDigit(3, score[3]);
    Score10k(); //Rollover to next digit
  } else {
    score[3]++; SetDispDigit(3, score[3]);
  }
}

void Score100() {
  if (score[2] == 9) {
    score[2] = 0; SetDispDigit(2, score[2]);
    Score1k(); //Rollover to next digit
  } else {
    score[2]++; SetDispDigit(2, score[2]);
  }
  
}

void Score10() {
  if (score[1] == 9) {
    score[1] = 0; SetDispDigit(1, score[1]);
    Score100(); //Rollover to next digit
  } else {
    score[1]++; SetDispDigit(1, score[1]);
  }
}

void Score1() {
  if (score[0] == 9) {
    score[0] = 0; SetDispDigit(0, score[0]);
    Score10(); //Rollover to next digit
  } else {
    score[0]++; SetDispDigit(0, score[0]);
  }
}


void Score500() {
  Score100();
  Score100();
  Score100();
  Score100();
  Score100();
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

void AttractDisp() {
  Serial.println("AttractDisp, attractDispCnt:");     
  Serial.println(attractDispCnt);
  //Serial.println("AttractLamps now, attractLmpCnt:  ");
  //Serial.println(attractLmpCnt);
  if (attractDispCnt > 10) {
    attractDispCnt = 1;
    ClearDispAll();
    //digitalWrite(53, HIGH);  
    //digitalWrite(68, HIGH);  
    Serial.println("AttractDisp reset"); 
    //SetDispDigit(0, attractDispCnt);                
  } else {
    
    SetDispDigit(0, attractDispCnt-1);  // digits 0-6, score is 0-5, ball 6, 
    SetDispDigit(1, attractDispCnt-1);  // digits 0-6, score is 0-5, ball 6, 
    SetDispDigit(2, attractDispCnt-1);  // digits 0-6, score is 0-5, ball 6,   
    SetDispDigit(3, attractDispCnt-1); 
    SetDispDigit(4, attractDispCnt-1); 
    SetDispDigit(5, attractDispCnt-1); 
    SetDispDigit(6, attractDispCnt-1); 
    digitalWrite(42, LOW);  
    digitalWrite(43, HIGH);    
    digitalWrite(44, HIGH);
    digitalWrite(45, LOW);        
    
     //int outputs3[] = {46, 47, 48, 49, 50, 51, 52, 53};
    // digitalWrite(46, LOW); //62    
    // digitalWrite(47, LOW);  //63    
    // digitalWrite(48, LOW);    //64
    // digitalWrite(49, LOW); //65   
    // digitalWrite(50, LOW); //66
    // digitalWrite(51, LOW); 
    // digitalWrite(52, LOW);    //69
    // digitalWrite(53, LOW); //68    
  }
  attractDispCnt = attractDispCnt+1; 
}
