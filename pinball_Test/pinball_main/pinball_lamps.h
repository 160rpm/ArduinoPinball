int lampLatchDelay = 0;
int lampDataLn = 14;
int lampBitSel[] = {15,16,17};
int lampLatchEnable = 18;
int lampLatchSel[] = {21,20,19}; //Driver board has 6 latches for 3 banks, but currently only two banks used, so only actually need 4 latches -> 2 select lines

int attractLmpCnt = 0;
int leftTgtLmpsState = 1;
int rightTgtLmpsState = 1;

//G. Over lamp now tied to Game Relay, so lamp on when Game Relay is off
//int gOverLmp = 0; //arduino pin# for relay control

int topLaneLmp6 = 7;     //arduino pin# for relay control, 5V feed on this relay
int specialCnterLmp = 6; //arduino pin# for relay control, 5V feed on this relay
int rightLne5kLmp = 5;   //arduino pin# for relay control, 5V feed on this relay

int creditStartLmp = 0;
int shootAgnLmp = 15;
int outlaneLmps = 1;
int bonus2xLmp = 14;
int bonus3xLmp = 2;
int bonus4xLmp = 13;
int bonus5xLmp = 3;
int bonus1kLmp = 12;
int bonus3KLmp = 7;
int bonus5kLmp = 8;
int bonus7kLmp = 6;
int bonus9kLmp = 9;
int bonus10kLmp = 4;
//int bBonus10kLmp = 5;
int bonus20kLmps = 10;
int bonus8kLmp = 11;
int bonus6kLmp = 16;	
int bonus4kLmp = 31;	
int bonus2kLmp = 17;	
int specialOutrLmps = 30;
int specialInnrLmps = 18;
int rightLne3kLmp = 19;
int rightTgtsLmps = 23;  
int leftTgtsLmps = 24; 
int lftRollovrLmps = 22;	
int topRollovrLmp1 = 25;	
int topRollovrLmp2 = 21;	
int topRollovrLmp3 = 26;	
int topRollovrLmp4 = 20;	
int topRollovrLmp5 = 27;	

int lampStates[] = {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  //Bank 1
                    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, //Bank 2
                    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0}; //Bank  3                   

//First 3 digits are latch adr, next 3 are bit adr. Treat as binary values, 0 or 1 only. 
//these need to be updated to the "right" addresses, as the lamp driver board is wired up a bit randomly
int lampMap[32][6] =  { {0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 1, 0},    //lamps 0-3
                        {0, 0, 0, 0, 0, 1}, {0, 0, 0, 1, 0, 1}, {0, 0, 0, 0, 1, 1}, {0, 0, 0 , 1, 1, 1},   //lamps 4-7
                        {1, 0, 0, 0, 0, 0}, {1, 0, 0, 1, 0, 0}, {1, 0, 0, 0, 1, 0}, {1, 0, 0, 1, 1, 0},    //lamps 8-11
                        {1, 0, 0, 0, 0, 1}, {1, 0, 0, 1, 0, 1}, {1, 0, 0, 0, 1, 1}, {1, 0, 0 , 1, 1, 1},   //lamps 12-15
                        {0, 1, 0, 0, 0, 0}, {0, 1, 0, 1, 0, 0}, {0, 1, 0, 0, 1, 0}, {0, 1, 0, 1, 1, 0},    //lamps 16-19
                        {0, 1, 0, 0, 0, 1}, {0, 1, 0, 1, 0, 1}, {0, 1, 0, 0, 1, 1}, {0, 1, 0 , 1, 1, 1},   //lamps 20-23
                        {1, 1, 0, 0, 0, 0}, {1, 1, 0, 1, 0, 0}, {1, 1, 0, 0, 1, 0}, {1, 1, 0, 1, 1, 0},    //lamps 24-27
                        {1, 1, 0, 0, 0, 1}, {1, 1, 0, 1, 0, 1}, {1, 1, 0, 0, 1, 1}, {1, 1, 0, 1, 1, 1}  }; //lamps 28-31
                                  


//state is 0 or 1, on or off. lampNo is the lamp's number. 
//Used to pick a record from lampMap[32][6] containing the address data for the lamp bit
void SetLamp(int state, int lampNo) {
  
  // Serial.println(" ");
  // Serial.println("SetLamp() with state/lampNo");
  // Serial.println(state);
  // Serial.println(lampNo);    
  
  digitalWrite(lampLatchEnable, HIGH); //always deselect latches before setting addresses  
  
  //Setting latch adr.
  if (lampMap[lampNo][0] == 0) {
    //Serial.println("lampMap[0] == 0");  
    digitalWrite(lampLatchSel[0], LOW);              
  } else {
    digitalWrite(lampLatchSel[0], HIGH);
  }
  if (lampMap[lampNo][1] == 0) {
    //Serial.println("lampMap[1] == 0");
    digitalWrite(lampLatchSel[1], LOW);
  } else {
    digitalWrite(lampLatchSel[1], HIGH);
  } 
  if (lampMap[lampNo][2] == 0) {
    //Serial.println("lampMap[2] == 0");
    digitalWrite(lampLatchSel[2], LOW);
  } else {
    digitalWrite(lampLatchSel[2], HIGH);
  }
  
  //Setting bit adr.
  if (lampMap[lampNo][3] == 0) {
    //Serial.println("lampMap[3] == 0");
    digitalWrite(lampBitSel[0], LOW);              
  } else {
    digitalWrite(lampBitSel[0], HIGH);
  }
  if (lampMap[lampNo][4] == 0) {
    //Serial.println("lampMap[4] == 0");
    digitalWrite(lampBitSel[1], LOW);
  } else {
    digitalWrite(lampBitSel[1], HIGH);
  } 
  if (lampMap[lampNo][5] == 0) {
    //Serial.println("lampMap[5] == 0");
    digitalWrite(lampBitSel[2], LOW);
  } else {
    digitalWrite(lampBitSel[2], HIGH);
  }
  
  //Set data line 
  if (state == 0) { //Lamp relays trigger on LOW, so 'state = 0' -> lamp on
    digitalWrite(lampDataLn, LOW); 
    //Serial.println("lamp state == 0/LOW/ON");
  } else {
    digitalWrite(lampDataLn, HIGH); 
    //Serial.println("lamp state == 1/HIGH/OFF");
  }
  
  delay(lampLatchDelay);
  digitalWrite(lampLatchEnable, LOW); //Sets state of lamp as configured above 
  delay(lampLatchDelay);
  digitalWrite(lampLatchEnable, HIGH);
  //lampStates[lampNo] = state; //Update map with the state we just set this lamp to
}



void ClearLamps() { //CLEAR LAMP RAM (All off)
  Serial.println(" ");  
  Serial.println("ClearLamps now");
  //delay(5000);  
  //These lamps are on Bank1 (Active HIGH)
  digitalWrite(topLaneLmp6, LOW); //5V
  digitalWrite(specialCnterLmp, LOW); //5V Bank 1 act. HIGH
  digitalWrite(rightLne5kLmp, LOW); //5V Bank 1 act. HIGH  
  
  for (int i = 0; i < 32; i++) { //Clear all latches
    //int state = 1; 
    SetLamp(1, i); //set lamps off by setting outputs HIGH (Active LOW)
  }
}


void AttractLamps() {  //Attract mode lamp show  
  digitalWrite(topLaneLmp6, HIGH); //5V Bank 1 act. HIGH 
  digitalWrite(specialCnterLmp, HIGH); //5V Bank 1 act. HIGH
  digitalWrite(rightLne5kLmp, HIGH); //5V Bank 1 act. HIGH
  //Serial.println("AttractLamps now, attractLmpCnt:  ");
  //Serial.println(attractLmpCnt);
  int lmpNr = 31;
  if (attractLmpCnt > 31) {
    attractLmpCnt=0;
    ClearLamps();
  }
    SetLamp(0, attractLmpCnt);//Lamp relays trigger on gnd, and we want to turn lamps on

  attractLmpCnt++;
}
