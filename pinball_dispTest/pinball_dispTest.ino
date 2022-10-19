#include "FTDebouncer.h"
FTDebouncer pinDebouncer(30);

int coils[] = {54, 55, 56, 57, 58, 59, 60, 61}; //Only 54-57 used

int dispDataLn = 62;
int dispBitSel[] = {63,64,65};
int dispLatchSel[] = {66,67}; //8 digits -> 4 latches
int dispLatchEnable = 68;

int sndSel[] = {8, 9, 10, 11};
int sndPlay = 12;

int CoinMtrRelay = 7;
int GOverLmp = 6;
int HScoreLmp = 5;
int TiltLmp = 4;

int lampDataLn = 14;
int lampBitSel[] = {15,16,17};
int lampLatchSel[] = {18,19,20};
int lampLatchEnable = 21;


int switchIN[] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40, 41,42, 43, 44, 45, 46, 47, 48, 49, 50,51,52,53};  
int inputCnt = 32; //Only 22-40 used atm.

int coinSW = 22;
int startSW = 23;
int troughSW = 24;
int holeSW = 25;
int outlanesSW = 26;
int inlanesSW = 27;
int leftTgts = 28;
int rightTgts = 29;
int DTdown = 30;
int Shooterln = 31;
int stndTarget = 32;
int topLn6 = 33;
int topLn5 = 34;
int topLn4 = 35;
int topLn3 = 36;
int topLn2 = 37;
int topLn1 = 38;
int leftLne = 39;
int sw22 = 40;

int outputs1[] = {0,1,2,3,4,5,6,7, 8,9,10,11,12, 13, 14, 15, 16,17,18,19,20, 21}
int output1Cnt = 22;

int outputs2[] = {54, 55, 56, 57, 58, 59, 60, 61, 62,63,64,65,66,67,68,69};
int output2Cnt = 16;

int coilPins[] = {54, 55, 56, 57, 58, 59, 60, 61};
int coilCnt = 8;

int relays12V[] = {};
int relays12Vcnt = 8;

int dispDataBit = 0; //default value for init cycle
int lampDataBit = 0; //default value for init cycle
int credits = 0;
bool inPlay = false; //Game running or in attract mode?
bool ballInTrough = false;
int highScore = 0;
int score = 0;

void clearSystem() {
  //clear coil lines, game relay first
  for (int cc = 0; cc < coilCnt; cc++) {
    digitalWrite(coilPins[cc], LOW);
  }
  
  //clear sound lines
   for (int cs = 0; cs < 4; cs++) {
    digitalWrite(sndSel[cs], LOW);
  }
  digitalWrite(sndPlay, HIGH); //Enable is active LOW
  
  //Clear display ram
  digitalWrite(dispDataLn, LOW);
  digitalWrite(dispLatchEnable, HIGH); //Active LOW?
  digitalWrite(63, LOW); //int dispBitSel[] = {63,64,65};
  digitalWrite(64, LOW);
  digitalWrite(65, LOW);
  
  digitalWrite(66, LOW); //dispLatchSel[] = {66,67}; //8 digits -> 4 latches
  digitalWrite(67, LOW);

  for (int cd = 0; cd < 4; cd++) { //Do for each of 4 latches (two digits per latch)
    switch (cd){
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

  
  //clear lamp ram
}

void setup() { //setup code, run once:
   for (int io = 0; io < outputCnt; io++) {
    pinMode(outputs[io], OUTPUT);
  }
  clearSystem();
  
  for (int ii = 0; ii < inputCnt; ii++) {
    pinDebouncer.addPin(switchIN[ii], HIGH, INPUT_PULLUP); //Inputs will default to HIGH, and avoid floating
  }
  pinDebouncer.begin();

  for (int io = 0; io < outputCnt; io++) {
    pinMode(outputs[io], OUTPUT);
  }
  
  Serial.begin(9600); //set serial monitor to same speed if needed
}

void loop() {
  pinDebouncer.update();  

  if (inPlay == false) {
    //attract mode here

    //set Game relay off (coils + sound off)
    //Show high score + HS light
    //Flash the other PF lamps
  }
}



void onPinActivated(int pinNumber){
  // do something according to the _pinNR that is triggered
  // for instance:
  //Serial.println(pinNumber);
  switch (pinNumber)
    {
      case 22: //Coin sw
        Serial.println("Coin sw closed");
        break;
      case 23: //Start sw
        Serial.println("Start btn closed"); 
        break;
      case 24: //tough SW
          Serial.println("trough sw closed");
          break;
        case 25: //Hole SW
          Serial.println("Hole sw closed");
          break;
        case 26: //inlanes sw
          Serial.println("inlane sw closed");
          break;
        case 27: //Hole sw
          Serial.println("Hole sw closed");
          break;
        default:
          Serial.println("this sw not defined yet:");
          Serial.println(pinNumber);
          break;
      }
}

void onPinDeactivated(int pinNumber){
  // do something according to the _pinNR that is triggered
  // for instance:
  Serial.println(pinNumber);
  //Through switch?
  //Shooter lane sw?
}
