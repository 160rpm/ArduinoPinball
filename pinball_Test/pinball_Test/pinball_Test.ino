#include "FTDebouncer.h"
FTDebouncer pinDebouncer(30);

int switchIN[] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40, 41,42, 43, 44, 45, 46, 47, 48, 49, 50,51,52,53};  
int inputCnt = 32; //Only 22-40 used atm.

int coinSW = 22;
int startSW = 23;
int troughSW = 24;
int holeSW = 25;
int outlanesSW = 26;
int inlanesSW = 27;
int leftTgtsSW = 28;
int rightTgtsSW= 29;
int DTdownSW = 30;
int ShooterlnSW = 31;
int stndTargetSW = 32;
int topLn6SW = 33;
int topLn5SW = 34;
int topLn4SW = 35;
int topLn3SW = 36;
int topLn2SW = 37;
int topLn1SW = 38;
int leftLneSW = 39;
int rubber22SW = 40;


int outputs1[] = {0,1,2,3,4,5,6,7, 8,9,10,11,12, 13, 14, 15, 16,17,18,19,20, 21};
int output1Cnt = 22;

int outputs2[] = {54, 55, 56, 57, 58, 59, 60, 61, 62,63,64,65,66,67,68,69};
int output2Cnt = 16;

int coilPins[] = {54, 55, 56, 57, 58, 59, 60, 61}; //Only 54-57 used
int coilCnt = 8;
int gameRelayPin = 54; 
int troughSolPin = 55;
int HoleSolPin = 56;
int trgetBnkRstPin = 57;

// Bank1 is active HIGH
int relays12VPins[] = {0, 1, 2, 3, 4, 5}; int relays12Vcnt = 6;
int GOverLmp = 0;
int HScoreLmp = 1;
int TiltLmp = 2;

int coinMtrPin = 6; //5V feed on this relay
int topLaneLmp6 = 7; //5V feed on this relay

int dispDataLn = 62;
int dispBitSel[] = {63,64,65};
int dispLatchSel[] = {66,67}; //8 digits -> 4 latches
int dispLatchEnable = 68;

int lampDataLn = 14;
int lampBitSel[] = {15,16,17};
int lampLatchEnable = 18;
int lampLatchSel[] = {19,20,21}; //Driver board has 6 latches for 3 banks, but currently only two banks used, so only need 4 latches -> 2 select lines

int sndSel[] = {8, 9, 10, 11, 12};
int sndPlay = 13;


bool inPlay = false; //Game running or in attract mode?
bool ballInTrough = false; //Is ball ready when starting game?
int attractLmpCnt = 0;
int stdLatchDelay = 100; //Std. delay for latch ops in millisecs
int dispDataBit = 0; //default value for init cycle
int lampDataBit = 0; //default value for init cycle
int credits = 0;
int highScore = 0;
int score = 0;
int ballCnt = 0; //game over state. Game only contains one physical ball.
bool topLane1 = false;
bool topLane2 = false;
bool topLane3 = false;
bool topLane4 = false;
bool topLane5 = false;
bool topLane6 = false;
int leftTgtCnt = 0; //win someting if hit 5 times
int rightTgtCnt = 0; //win someting if hit 5 times


void clearSystem() { 
  //clear coil lines, game relay first
  for (int cc = 0; cc < coilCnt; cc++) {
    digitalWrite(coilPins[cc], LOW);
  }
  digitalWrite(HoleSolPin, HIGH); //Just in case ball was left here
  delay(500);
  digitalWrite(HoleSolPin, LOW);

  //clear sound lines
   for (int cs = 0; cs < 5; cs++) {
    digitalWrite(sndSel[cs], LOW);
  }
  digitalWrite(sndPlay, HIGH); //Enable is active LOW
  
  //Clear display ram
  digitalWrite(dispDataLn, LOW);
  digitalWrite(dispLatchEnable, HIGH); //Active LOW
  digitalWrite(63, LOW); //int dispBitSel[] = {63,64,65};
  digitalWrite(64, LOW);
  digitalWrite(65, LOW);
  
  digitalWrite(66, LOW); //dispLatchSel[] = {66,67}; //8 digits -> 4 latches -> 2 bits
  digitalWrite(67, LOW);

  for (int cd = 0; cd < 4; cd++) { //Do for each of 4 latches (two digits per latch)
    switch (cd) {
      case 0: //1st latch
        digitalWrite(66, LOW);
        digitalWrite(67, LOW);
        break;
      case 1: //2nd latch
        digitalWrite(66, HIGH);
        digitalWrite(67, LOW);
        break;
      case 2: //3rd latch
        digitalWrite(66, LOW);
        digitalWrite(67, HIGH);
        break;
      case 3: //4th latch
        digitalWrite(66, HIGH);
        digitalWrite(67, HIGH);
        break;
      default:
          Serial.println("this display latch not defined yet:");
          Serial.println(cd);
          break;
    }
  
    //Start setting bits in latches
    digitalWrite(dispLatchEnable, LOW); //Active LOW? //set bit 0
    //wait a bit here
    digitalWrite(dispLatchEnable, HIGH); //Active LOW?
    digitalWrite(63, HIGH);
    digitalWrite(dispLatchEnable, LOW); //Active LOW? //set bit 1
    //wait a bit here
    digitalWrite(dispLatchEnable, HIGH); //Active LOW?
    digitalWrite(63, LOW);
    digitalWrite(64, HIGH);
    digitalWrite(dispLatchEnable, LOW); //Active LOW? //set bit 2
    //wait a bit here
    digitalWrite(dispLatchEnable, HIGH); //Active LOW?
    digitalWrite(64, HIGH);
    digitalWrite(dispLatchEnable, LOW); //Active LOW?
    //wait a bit here
    digitalWrite(dispLatchEnable, HIGH); //Active LOW?
  }
  ClearLamps();
}

void ClearLamps() {
  //CLEAR LAMP RAM
  //lampDataLn = 14;
  //lampBitSel[] = {15,16,17};
  ///lampLatchSel[] = {19,20, 21};
  //lampLatchEnable = 18;
  digitalWrite(lampDataLn, HIGH); //Lamp relays trigger on gnd
  digitalWrite(lampLatchEnable, HIGH); //Active LOW

  for (int cd = 0; cd < 6; cd++) { //Do for each of 6 latches (8 lamps per latch -> two latches per bank)
    switch (cd) {
      case 0: //1st latch addressed (0)
        digitalWrite(lampLatchSel[0], LOW);
        digitalWrite(lampLatchSel[1], LOW);
        digitalWrite(lampLatchSel[2], LOW);
        break;
      case 1: //2nd latch addressed (1)
        digitalWrite(lampLatchSel[0], HIGH);
        digitalWrite(lampLatchSel[1], LOW);
        digitalWrite(lampLatchSel[2], LOW);
        break;
      case 2: //3rd latch addressed (2)
        digitalWrite(lampLatchSel[0], LOW);
        digitalWrite(lampLatchSel[1], HIGH);
        digitalWrite(lampLatchSel[2], LOW);
        break;
      case 3: //4th latch addressed (3)
        digitalWrite(lampLatchSel[0], HIGH);
        digitalWrite(lampLatchSel[1], HIGH);
        digitalWrite(lampLatchSel[2], LOW);
        break;
      case 4: //5th latch addressed (4)
        digitalWrite(lampLatchSel[0], LOW);
        digitalWrite(lampLatchSel[1], LOW);
        digitalWrite(lampLatchSel[2], HIGH);
        break;
      case 5: //6th latch addressed (5)
        digitalWrite(lampLatchSel[0], HIGH);
        digitalWrite(lampLatchSel[1], LOW);
        digitalWrite(lampLatchSel[2], HIGH);
        break;

      default:
        Serial.println("this lamp latch not defined:");
        Serial.println(cd);
        break;
    }
  
    //Start setting bits in latches
    digitalWrite(lampLatchEnable, LOW); //Active LOW //set bit 0
    delay(200); //wait a bit here
    digitalWrite(lampLatchEnable, HIGH); 
    digitalWrite(lampBitSel[0], HIGH);
    delay(100);
    digitalWrite(lampLatchEnable, LOW); //set bit 1
    delay(200); //wait a bit here
    digitalWrite(lampLatchEnable, HIGH); 
    digitalWrite(lampBitSel[0], LOW);
    digitalWrite(lampBitSel[1], HIGH);
    delay(100);
    digitalWrite(lampLatchEnable, LOW); //set bit 2
    delay(200); //wait a bit here
    digitalWrite(lampLatchEnable, HIGH);
    digitalWrite(lampBitSel[0], HIGH);
    digitalWrite(lampBitSel[1], HIGH);
    delay(100);
    digitalWrite(lampLatchEnable, LOW);  //set bit 3
    delay(200); //wait a bit here
    digitalWrite(lampLatchEnable, HIGH); 
    digitalWrite(lampBitSel[0], LOW);
    digitalWrite(lampBitSel[1], LOW);
    digitalWrite(lampBitSel[2], HIGH);
    delay(100);
    digitalWrite(lampLatchEnable, LOW);  //set bit 4
    delay(200); //wait a bit here
    digitalWrite(lampLatchEnable, HIGH); 
    digitalWrite(lampBitSel[0], HIGH);
    digitalWrite(lampBitSel[1], LOW);
    digitalWrite(lampBitSel[2], HIGH);
    delay(100);
    digitalWrite(lampLatchEnable, LOW); //set bit 5
    delay(200); //wait a bit here
    digitalWrite(lampLatchEnable, HIGH); 
    digitalWrite(lampBitSel[0], LOW);
    digitalWrite(lampBitSel[1], HIGH);
    digitalWrite(lampBitSel[2], HIGH);
    delay(100);
    digitalWrite(lampLatchEnable, LOW); //set bit 6
    delay(200); //wait a bit here
    digitalWrite(lampLatchEnable, HIGH);
    digitalWrite(lampBitSel[0], HIGH);
    digitalWrite(lampBitSel[1], HIGH);
    digitalWrite(lampBitSel[2], HIGH);
    digitalWrite(lampLatchEnable, LOW);  //set bit 7
    delay(200); //wait a bit here
    digitalWrite(lampLatchEnable, HIGH); //latches are de-selected
  }
}

void setup() { //setup code, run once:
  for (int io = 0; io < output1Cnt; io++) {
    pinMode(outputs1[io], OUTPUT);
  }
  for (int oi = 0; oi < output2Cnt; oi++) {
    pinMode(outputs2[oi], OUTPUT);
  }

  clearSystem();
  
  for (int ii = 0; ii < inputCnt; ii++) {
    pinDebouncer.addPin(switchIN[ii], HIGH, INPUT_PULLUP); //Inputs will default to HIGH, and avoid floating. Switches close to gnd.
  }
  pinDebouncer.begin();
  
  Serial.begin(9600); //set serial monitor to same speed 
}

void loop() {
  pinDebouncer.update();  //run switch inputs here

  if (inPlay == false) { //attract mode here
    //set Game relay off (flippers + music off)
    //The "Game Relay" can stay on forever (not a solenoid)
    digitalWrite(gameRelayPin, LOW); 

    //Show high score on score display
    digitalWrite(HScoreLmp, HIGH); 

    //Flash the other PF lamps
    AttractLamps();
  }

  if (inPlay == true) {
    //Game playing here

    //set Game relay on (flippers powered+music plays)
    //Update display with balls left and current player score
    //Check for game over conditions and set inPlay = false if met, 0 balls left, etc
  }
}



void onPinActivated(int pinNumber){
  // do something according to the _pinNR that is closed
  switch (pinNumber) {
    case 22: //Coin sw
      Serial.println("Coin sw closed");
      credits = credits + 1; //coin inserted, so we have one more credit
      //Should also pulse coin meter relay
      digitalWrite(coinMtrPin, HIGH);
      delay(500);
      digitalWrite(coinMtrPin, LOW);
      break;
    case 23: //Start sw
      Serial.println("Start btn closed"); 
      if (credits > 0) { //start a game if we have credits
        inPlay = true; 
        credits = credits - 1;
        if (score > highScore) { //Maybe move this to some GameOver() function later
          highScore = score;
        }
        score = 0;
        ClearLamps();
        //reset drop targets
        digitalWrite(trgetBnkRstPin, HIGH);
        delay(750);
        digitalWrite(trgetBnkRstPin, LOW);
        ballCnt = 3; //reset ball count
        topLane1 = false;
        topLane2 = false;
        topLane3 = false;
        topLane4 = false;
        topLane5 = false;
        topLane6 = false;
        leftTgtCnt = 0;
        leftTgtCnt = 0;
      }
      break;
    case 24: //tough SW
      Serial.println("trough sw closed");
      ballInTrough = true;
      break;
    case 25: //Hole SW
      Serial.println("Hole sw closed");
      delay(500);
      //play sound effect
      delay(500);
      digitalWrite(HoleSolPin, HIGH);
      delay(750);
      digitalWrite(HoleSolPin, LOW);
      break;
    case 26: //outlanes
      Serial.println("Outlanes sw closed");
      //Ball drained. Game over or balls left? Extra ball won?               
      break;
    case 27: //inlanes
      Serial.println("Inlanes sw closed");
      //Give points and?
      break;
    case 28: //Left st. tgts(43-47)
      Serial.println("Left st. tgts(43-47) sw closed");
      break;
      case 29: //Right st. tgts(36-32)
      Serial.println("Right st. tgts(36-32)  sw closed");
      break;          
    case 30: //D.T. all down(27-24)
      Serial.println("D.T. all down(27-24) sw closed");
      break;                                      
    case 31: //Shooter lane(20)
      Serial.println("Shooter lane(20) sw closed");
      //Dont do anything till ball is launched
      break;        
    case 32: //Single st. target(29)
      Serial.println("Single st. target(29) sw closed");
      //Gives points according to lit values
      break;
    case 33: //Top lane 6(42)
      Serial.println("Top lane 6(42)  sw closed");
      //Turn on assoc. lamp
      topLane6 = true;
      break;
    case 34: //Top lane 5
      Serial.println("Top lane 5  sw closed");
      topLane5 = true;        
      //Turn on assoc. lamp        
      break;
    case 35: //Top lane 4
      Serial.println("Top lane 4  sw closed");
      topLane4 = true;        
      //Turn on assoc. lamp          
      break;
    case 36: //Top lane 3
      Serial.println("Top lane 3  sw closed");
      topLane3 = true;        
      //Turn on assoc. lamp 
      break;
    case 37: //Top lane 2          
      Serial.println("Top lane 2 sw closed");
      topLane2 = true;        
      //Turn on assoc. lamp 
      break;
    case 38: //Top lane 1(37)
      Serial.println("Top lane 1(37)  sw closed");
      topLane1 = true;        
      //Turn on assoc. lamp         
      break;
    case 39: //Left lane btns(11-9)
      Serial.println("Left lane btns(11-9) sw closed");
      break;
    case 40: //Rubber switches(22)
      Serial.println("Rubber switches(22) closed");
      score = score + 10;
      break;

        default:
          Serial.println("...");
          Serial.println("this sw not defined yet:");
          Serial.println(pinNumber);
          Serial.println("...");
          break;
    }
}

void onPinDeactivated(int pinNumber){
  // do something according to the _pinNR that is released
  switch (pinNumber) {
    case 22: //Shooter lane sw
      Serial.println("Shooter lane sw released");
      //play some sound effect here
      break;
    default:
        Serial.println("...");
        Serial.println("this sw doesnt do anything when released:");
        Serial.println(pinNumber);
        Serial.println("...");
        break;
  }
}

void AttractLamps() {  //Attract mode lamp show 
  int lmpLatchOffset = 0; 
  //lampDataLn = 14;
  //lampBitSel[] = {15,16,17};
  ///lampLatchSel[] = {19,20, 21};
  //lampLatchEnable = 18;
  digitalWrite(lampDataLn, LOW); //Lamp relays trigger on gnd
  digitalWrite(lampLatchEnable, HIGH); //Active LOW

  if (attractLmpCnt > 31) {
    attractLmpCnt = 0;
    ClearLamps();
  }

  //Select the correct latch
  if (attractLmpCnt < 8) { 
    digitalWrite(lampLatchSel[0], LOW);
    digitalWrite(lampLatchSel[1], LOW);
    digitalWrite(lampLatchSel[2], LOW);
    //After selecting the appropriate latch here, we just need to know the bit pos. on the selected latch when switching the current lamp on
    lmpLatchOffset = attractLmpCnt; 
  }
  else if (attractLmpCnt < 16) {
    digitalWrite(lampLatchSel[0], HIGH);
    digitalWrite(lampLatchSel[1], LOW);
    digitalWrite(lampLatchSel[2], LOW);
    lmpLatchOffset = attractLmpCnt - 8;
  }
  else if (attractLmpCnt < 24) {
    digitalWrite(lampLatchSel[0], LOW);
    digitalWrite(lampLatchSel[1], HIGH);
    digitalWrite(lampLatchSel[2], LOW);
    lmpLatchOffset = attractLmpCnt - 16;
  }
  else {
    digitalWrite(lampLatchSel[0], HIGH);
    digitalWrite(lampLatchSel[1], HIGH);
    digitalWrite(lampLatchSel[2], LOW);
    lmpLatchOffset = attractLmpCnt - 24;
  }
  switch (lmpLatchOffset) {
    case 0:
      digitalWrite(lampBitSel[0], LOW);
      digitalWrite(lampBitSel[1], LOW);
      digitalWrite(lampBitSel[2], LOW);
      delay(stdLatchDelay);
      digitalWrite(lampLatchEnable, LOW); //set lamp on
      delay(stdLatchDelay); //wait a bit here
      digitalWrite(lampLatchEnable, HIGH);
      break;
    case 1:
      digitalWrite(lampBitSel[0], HIGH);
      digitalWrite(lampBitSel[1], LOW);
      digitalWrite(lampBitSel[2], LOW);
      delay(stdLatchDelay);
      digitalWrite(lampLatchEnable, LOW); //set lamp on
      delay(stdLatchDelay); //wait a bit here
      digitalWrite(lampLatchEnable, HIGH);
      break;
    case 2:
      digitalWrite(lampBitSel[0], LOW);
      digitalWrite(lampBitSel[1], HIGH);
      digitalWrite(lampBitSel[1], LOW);
      delay(stdLatchDelay);
      digitalWrite(lampLatchEnable, LOW); //set lamp on
      delay(stdLatchDelay); //wait a bit here
      digitalWrite(lampLatchEnable, HIGH);
      break;
    case 3:
      digitalWrite(lampBitSel[0], HIGH);
      digitalWrite(lampBitSel[1], HIGH);
      digitalWrite(lampBitSel[1], LOW);
      delay(stdLatchDelay);
      digitalWrite(lampLatchEnable, LOW); //set lamp on
      delay(stdLatchDelay); //wait a bit here
      digitalWrite(lampLatchEnable, HIGH);
      break;
    case 4:
      digitalWrite(lampBitSel[0], LOW);
      digitalWrite(lampBitSel[1], LOW);
      digitalWrite(lampBitSel[1], HIGH);
      delay(100);
      digitalWrite(lampLatchEnable, LOW); //set lamp on
      delay(150); //wait a bit here
      digitalWrite(lampLatchEnable, HIGH);
      break;
    case 5:
      digitalWrite(lampBitSel[0], HIGH);
      digitalWrite(lampBitSel[1], LOW);
      digitalWrite(lampBitSel[1], HIGH);
      delay(100);
      digitalWrite(lampLatchEnable, LOW); //set lamp on
      delay(150); //wait a bit here
      digitalWrite(lampLatchEnable, HIGH);
      break;
    case 6:
      digitalWrite(lampBitSel[0], LOW);
      digitalWrite(lampBitSel[1], HIGH);
      digitalWrite(lampBitSel[1], HIGH);
      delay(100);
      digitalWrite(lampLatchEnable, LOW); //set lamp on
      delay(150); //wait a bit here
      digitalWrite(lampLatchEnable, HIGH);
      break;
    case 7:
      digitalWrite(lampBitSel[0], HIGH);
      digitalWrite(lampBitSel[1], HIGH);
      digitalWrite(lampBitSel[1], HIGH);
      delay(100);
      digitalWrite(lampLatchEnable, LOW); //set lamp on
      delay(150); //wait a bit here
      digitalWrite(lampLatchEnable, HIGH);
      break;

    default:
      Serial.println("this lmpLatchOffset not defined:");
      Serial.println(lmpLatchOffset);
      break;
  }
  attractLmpCnt = attractLmpCnt+1;
  
}