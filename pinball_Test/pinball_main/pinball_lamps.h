int lampLatchDelay = 1;
int lampDataLn = 14;
int lampBitSel[] = {15,16,17};
int lampLatchEnable = 18;
int lampLatchSel[] = {21,20,19}; //Driver board has 6 latches for 3 banks, but currently only two banks used, so only actually need 4 latches -> 2 select lines

int attractLmpCnt = 0;
char getHL[2][5] = {"LOW", "HIGH"};

int gOverLmp = 0; //arduino pin# for relay control
int tiltLmp = 2;  //arduino pin# for relay control
int coinMtrPin = 6; //arduino pin# for relay control, 5V feed on this relay
int topLaneLmp6 = 7; //arduino pin# for relay control, 5V feed on this relay

int shootAgnLmp = 1;

int bonus2xLmp = 3;
int bonus3xLmp = 4;
int bonus4xLmp = 5;
int bonus5xLmp = 6;
int bonus1kLmp = 7;
int bonus3KLmp = 8;
int bonus5kLmp = 9;
int bonus7kLmp = 10;
int bonus9kLmp = 11;
int bBonus10kLmp = 12;
int bonus20kLmp = 13;
int bonus10kLmp = 14;
int bonus8kLmp = 15;
int bonus6kLmp = 16;	
int bonus4kLmp = 17;	
int bonus2kLmp = 18;	
//LAMPS
int creditStartLmp = 0;
int outlaneLmps = 2;
int specialOutrLmps = 19;
int specialInnrLmps = 20;
int specialCnterLmp = 21;
int rightLne3kLmp = 22;
int rightLne5kLmp = 23;
int rightTgtsLmps = 24;  
int lftTgtsLmps = 25; 



int lftRollovrLmps = 26;	
int topRollovrLmp1 = 27;	
int topRollovrLmp2 = 28;	
int topRollovrLmp3 = 29;	
int topRollovrLmp4 = 30;	
int topRollovrLmp5 = 31;	

int lampStates[] = {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  //Bank 1
                    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0}; //Bank 2

//First 3 digits are latch adr, next 3 are bit adr. Treat as binary values, 0 or 1 only. 
//these need to be updated to the "right" addresses, as the lamp driver board is wired up a bit randomly
int lampMap[32][6] =  { {0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 1, 0},    //lamps 0-3
                        {0, 0, 0, 0, 0, 1}, {0, 0, 0, 1, 0, 1}, {0, 0, 0, 0, 1, 1}, {0, 0, 0 , 1, 1, 1},   //lamps 4-7
                        {1, 0, 0, 0, 0, 0}, {1, 0, 0, 1, 0, 0}, {1, 0, 0, 0, 1, 0}, {1, 0, 0, 1, 1, 0},    //lamps 8-11
                        {1, 0, 0, 0, 0, 1}, {1, 0, 0, 1, 0, 1}, {1, 0, 0, 0, 1, 1}, {1, 0, 0 , 1, 1, 1},   //lamps 12-15
                        {0, 1, 0, 0, 0, 0}, {0, 1, 0, 1, 0, 0}, {0, 1, 0, 0, 1, 0}, {0, 1, 0, 1, 1, 0},    //lamps 16-19
                        {0, 1, 0, 0, 0, 1}, {0, 1, 0, 1, 0, 1}, {0, 1, 0, 0, 1, 1}, {0, 1, 0 , 1, 1, 1},   //lamps 20-23
                        {1, 1, 0, 0, 0, 0}, {1, 1, 0, 1, 0, 0}, {1, 1, 0, 0, 1, 0}, {1, 1, 0, 1, 1, 0},    //lamps 24-27
                        {1, 1, 0, 0, 0, 1}, {1, 1, 0, 1, 0, 1}, {1, 1, 0, 0, 1, 1}, {1, 1, 0 , 1, 1, 1} }; //lamps 28-31


//state is 0 or 1, on or off. lampNo is the lamp's number. 
//Used to pick a record from lampMap[32][6] containing the address data for the lamp bit
void SetLamp(int state, int lampNo) {
    digitalWrite(lampLatchEnable, HIGH); //always deselect latches before setting addresses  
    digitalWrite(lampLatchSel[0], getHL[lampMap[lampNo][0]]);
    digitalWrite(lampLatchSel[1], getHL[lampMap[lampNo][1]]);
    digitalWrite(lampLatchSel[2], getHL[lampMap[lampNo][2]]);

    digitalWrite(lampBitSel[0], getHL[lampMap[lampNo][3]]);
    digitalWrite(lampBitSel[1], getHL[lampMap[lampNo][4]]);
    digitalWrite(lampBitSel[2], getHL[lampMap[lampNo][5]]);

    digitalWrite(lampDataLn, getHL[state]); //Lamp relays trigger on LOW, so 'state = 0' -> lamp on
    delay(lampLatchDelay);
    digitalWrite(lampLatchEnable, LOW); //Sets state of lamp as configured above 
    delay(lampLatchDelay);
    digitalWrite(lampLatchEnable, HIGH);
    lampStates[lampNo] = state; //Update map with the state we just set this lamp to
}


void ToggleLamp(int lampNo) { //Flips the state of the given lamp
    int state = lampStates[lampNo];
    if (state == 0) {
        state = 1;
    } else {
        state = 0;
    }

    digitalWrite(lampLatchEnable, HIGH); //always deselect latches before setting addresses  
    digitalWrite(lampLatchSel[0], getHL[lampMap[lampNo][0]]);
    digitalWrite(lampLatchSel[1], getHL[lampMap[lampNo][1]]);
    digitalWrite(lampLatchSel[2], getHL[lampMap[lampNo][2]]);

    digitalWrite(lampBitSel[0], getHL[lampMap[lampNo][3]]);
    digitalWrite(lampBitSel[1], getHL[lampMap[lampNo][4]]);
    digitalWrite(lampBitSel[2], getHL[lampMap[lampNo][5]]);
                                        
    digitalWrite(lampDataLn, getHL[state]); //Lamp relays trigger on LOW, so 'state = 0' -> lamp on
    delay(lampLatchDelay);
    digitalWrite(lampLatchEnable, LOW); //Flips state of lamp as configured above 
    delay(lampLatchDelay);
    digitalWrite(lampLatchEnable, HIGH);
    lampStates[lampNo] = state; //Update map with the state we just set this lamp to
}


void ClearLamps() { //CLEAR LAMP RAM (All off)
  //These 3 lamps are on Bank1 (Active HIGH)
  digitalWrite(gOverLmp, LOW);  //G.Over lmp(12V)
//  digitalWrite(hScoreLmp, LOW);  //High Score lmp(12V) 
  digitalWrite(tiltLmp, LOW);  //Tilt lamp(12V)
  digitalWrite(topLaneLmp6, LOW); //5V
  
  for (int i = 0; i < 32; i++) { //Do for each lamp
    int state = 1; //set lamps off (Active LOW)
    SetLamp(state, i);
  }
}


void AttractLamps() {  //Attract mode lamp show  
  if (attractLmpCnt > 47) {
    attractLmpCnt = 0;
    ClearLamps();
  }
  
  SetLamp(0, attractLmpCnt);//Lamp relays trigger on gnd, and we want to turn lamps on

  attractLmpCnt = attractLmpCnt+1;
}
