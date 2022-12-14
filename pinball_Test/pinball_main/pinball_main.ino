#include "pinball_display.h"
#include "pinball_lamps.h"
#include "pinball_sounds.h"
#include "FTDebouncer.h"
FTDebouncer pinDebouncer(10);


int switchIN[] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40};  
int inputCnt = 19; 

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
int topLn1SW = 33;
int topLn2SW = 34;
int topLn3SW = 35;
int topLn4SW = 36;
int topLn5SW = 37;
int topLn6SW = 38;
int leftLneSW = 39;
int rubber22SW = 40;


int outputs1[] = {2,3,4,5,6,7, 8,9,10,11,12, 13, 14, 15, 16,17,18,19,20, 21};
int output1Cnt = 20;

int outputs2[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 65, 69};
int output2Cnt = 11;

//Bank1 + Disp. digit 7 direct control
int outputs3[] = {42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53}; 
int output3Cnt = 12;

// Bank1 is active HIGH
int coilPins[] = {54, 55, 56, 57, 58, 59, 60, 61}; //Only 54-57 used
int coilCnt = 8;
int gameRelayPin = 54; 
int troughSolPin = 55;
int holeSolPin = 56;
int trgetBnkRstPin = 57;

// Bank1 is active HIGH
int relays5VPins[] = {41, 48, 2, 3, 4, 5, 6, 7}; int relays5Vcnt = 8;
int coinMtrPin = 2; //arduino pin# for relay control, 5V feed on this relay


bool inPlay = false; //Game running or in attract mode?
bool ballInTrough = false; //Is ball ready when starting game?

int digitscroll = 0;
long attractTimer = 0L;
int stdLatchDelay = 1; //Std. delay for latch ops in millisecs
int stdSolPulseTime = 250; //standard time to leave sols energized in millisecs 
//int dispDataBit = 0; //default value for init cycle
//int lampDataBit = 0; //default value for init cycle

int credits = 0;
int ballCnt = 0; //Game only contains one physical ball
bool shootAgain = false;
bool ballSaved = false;

bool topLane1 = false;
bool topLane2 = false;
bool topLane3 = false;
bool topLane4 = false;
bool topLane5 = false;
bool topLane6 = false;
bool special  = false;
int specialScroll = 0; //for anim Special lights

int rightLaneTgtValue = 0; //steps right lane target value

int bonusStep = 0;
int bonusMult = 0;

//For multiple scores
int score100 = 0;
int score1k = 0;
int score10k = 0;


void ClearSystem() { 
  //clear relay bank 1, coil lines, game relay
  for (int cc = 0; cc < coilCnt; cc++) {
    digitalWrite(coilPins[cc], LOW);
    digitalWrite(relays5VPins[cc], LOW);
  }
  ClearLamps();
  ClearDispAll();
  ClearSoundPins();
  
  PulseSol(holeSolPin); //Just in case ball was left here
}

void setup() { //setup code, run once on power-on
  for (int io = 0; io < output1Cnt; io++) {
    pinMode(outputs1[io], OUTPUT);
  }
  for (int oi = 0; oi < output2Cnt; oi++) {
    pinMode(outputs2[oi], OUTPUT);
  }
  for (int iii = 0; iii < output3Cnt; iii++) {
    pinMode(outputs3[iii], OUTPUT);
  }  
  
  for (int ii = 0; ii < inputCnt; ii++) {
    pinDebouncer.addPin(switchIN[ii], HIGH, INPUT_PULLUP); //Inputs will default to HIGH, and avoid floating. Switches close to gnd.
  }
  pinDebouncer.begin();
  
  Serial.begin(9600); //set serial monitor to same speed 
  ClearSystem();
}

//               ***MAIN LOOP***
void loop() {
  pinDebouncer.update();  //run switch inputs here

  if (inPlay == false) { //attract mode here
    digitalWrite(gameRelayPin, LOW); //set Game relay off (flippers + music off)
                                     //The "Game Relay" can stay on forever (not a solenoid)

    if(credits > 0) { //start+credit lamp on if we have credits
    //  SetLamp(0, creditStartLmp); 
    } else {
      //SetLamp(1, creditStartLmp); 
    }
    
    //Flash the PF lamps in attract mode, once every second or something similar
    if (attractTimer > 35000L) {
      AttractLamps();
      AttractDisp();
      AttractSounds();      
      //Score1(); //to test displays in attract mode
      //SetDispDigit(digitscroll, digitscroll);      
      Serial.println("AttractTimer");
      //SetLamp(1,20);
      attractTimer = 0L;
    } else {
      attractTimer++;      
    }
  }

  if (inPlay == true) { // ***Game playing here***
    digitalWrite(gameRelayPin, HIGH);  //set Game relay on (flippers + music on)
                                       //The "Game Relay" can stay on forever (not a solenoid)   

    //We use this method for scoring to prevent blocking the system
    //For instance getting 500 points would block for a moment, since Score100() would be called  times in a row without returning to main loop
    //But now Score100() will only be called once per iteration of main loop, 
    //and other switch inputs can be responded to. Scoring additional points would just increment the integer counters, and the resulting scores would be added to the main score counter over the following iterations
    if (score100 > 0) {
      Score100();
      score100--;
    }
    if (score1k > 0) {
      Score1k();
      score1k--;
    }    
    if (score10k > 0) {
      Score10k();
      score10k--;
    }

    //when special is active, scroll the special lamps
    if (special == true && attractTimer > 3000L) {
      attractTimer = 0L;
      SpecialLmpsScroll();
    } else {
      attractTimer++;
    }

    CheckGOver();
    
  }
}



void onPinActivated(int pinNumber) {
  // do something according to the _pinNR that is closed
  switch (pinNumber) {

    case coinSW:
      Serial.println("Coin sw closed");
      if (credits < 9) {
        credits = credits + 1; //coin inserted, so we have one more credit
        SetDispCredit(credits);
        SetLamp(0, creditStartLmp);

        PulseSol(coinMtrPin); //Pulse coin meter
      }      
      break;
    case 23: //Start sw
      Serial.println("Start btn closed"); 
      if (credits > 0) { //start a game if we have credits
        credits = credits - 1;
        SetDispCredit(credits);
        if (credits < 1) {
          SetLamp(1, creditStartLmp);
        }

        ScoreReset(); ClearDispScore(); ClearLamps();
        BonusReset(); SpecialReset(); LanesReset();
        
        PulseSol(trgetBnkRstPin);//reset drop targets
        
        ballCnt = 3; SetDispBall(ballCnt); //reset ball count on game start
        
        inPlay = true; PlaySound(1); //Now we start
        
      } else {
        Serial.println("Start btn closed, but no credits"); 
        PlaySound(4); //Play "another coin please"
      }
      break;
    
    case 24: //trough SW
      Serial.println("trough sw closed");
      ballInTrough = true;
      ballCnt--; SetDispBall(ballCnt);
      PlaySound(5);     //Ball drained, so play drained soundfx
      BonusCollect();   //On ball drain, collect bonus even if game not over. 
                        //G.Over state checked from main loop, G.Over if ball count reaches 0, after being decreased by one here
      break;
    
    case 25: //Hole SW
      Serial.println("Hole sw closed");
      
      if (special == true) { //Collect Special here and reset Special state
        PlaySound(7); //SFX
        ballCnt++;
        SpecialReset();
      }

      score1k = score1k + 5; //Always get 5k from the hole 
      PulseSol(holeSolPin);//Kicks ball back out
      break;

    case 26: //outlanes
      Serial.println("Outlanes sw closed"); //Ball drained
      PlaySound(5);
      if (ballSaved == true) { //Give ball back?
        ballCnt++; SetDispBall(ballCnt);
        ballSaved = false; SetLamp(1, outlaneLmps);
      }       
      break;

    case 27: //inlanes
      Serial.println("Inlanes sw closed");
      IncBonusStep();
      break;
    case 28: //Left st. tgts(43-47)
      Serial.println("Left st. tgts(43-47) sw closed");
      StepRightLaneVal();
      if (leftTgtLmpsState == 0) {
        leftTgtLmpsState = 1;
      } else {
        leftTgtLmpsState = 0;
      }
      SetLamp(leftTgtLmpsState, leftTgtsLmps);
      break;

      case 29: //Right st. tgts(36-32)
      Serial.println("Right st. tgts(36-32)  sw closed");
      StepRightLaneVal();
      if (rightTgtLmpsState == 0) {
        rightTgtLmpsState = 1;
      } else {
        rightTgtLmpsState = 0;
      }
      SetLamp(rightTgtLmpsState, rightTgtsLmps);
      break;  

    case 30: //D.T. all down(27-24)
      Serial.println("D.T. all down(27-24) sw closed");
      ballSaved = true; SetLamp(0, outlaneLmps);
      IncBonusStep();
      PlaySound(3); //Go to the future
      PulseSol(trgetBnkRstPin);//reset drop targets
      break;      

    case 31: //Shooter lane(20)
      Serial.println("Shooter lane(20) sw closed");
      //Dont do anything till ball is launched
      break;

    case 32: //Single st. target right lane(29)
      Serial.println("Single st. target(29) sw closed");
      switch (rightLaneTgtValue) { //We just get points here, value doesn't change
        case 0: 
          Score1();
          break;
        case 1:
          score1k = score1k + 3; 
          break;
        case 2:
          score1k = score1k + 5; 
          break;
      }
      PlaySound(7);
      break;

    case 33: //Top lane 1(42)
      Serial.println("Top lane 1(42)  sw closed");
      topLane1 = true; SetLamp(0, topRollovrLmp1);
      score100 = score100 + 5; 
      SpecialCheck();
      break;
    case 34: //Top lane 2
      Serial.println("Top lane 2 sw closed");
      topLane2 = true; SetLamp(0, topRollovrLmp2);             
      score100 = score100 + 5; 
      SpecialCheck();
      break;
    case 35: //Top lane 3
      Serial.println("Top lane 3  sw closed");
      topLane3 = true; SetLamp(0, topRollovrLmp3);                    
      score100 = score100 + 5; 
      SpecialCheck();
      break;
    case 36: //Top lane 4
      Serial.println("Top lane 4  sw closed");
      topLane4 = true; SetLamp(0, topRollovrLmp4);           
      score100 = score100 + 5; 
      SpecialCheck();
      break;
    case 37: //Top lane 5         
      Serial.println("Top lane 5 sw closed");
      topLane5 = true; SetLamp(0, topRollovrLmp5);           
      score100 = score100 + 5; 
      SpecialCheck();
      break;
    case 38: //Top lane 6(37)
      Serial.println("Top lane 6(37)  sw closed");
      topLane6 = true; digitalWrite(topLaneLmp6Pin, HIGH);
      score100 = score100 + 5; 
      SpecialCheck();

      break;
    case 39: //Left lane btns(11-9)
      Serial.println("Left lane btns(11-9) sw closed");
      SetLamp(0, lftRollovrLmps);
      IncBonusMult();
      Score1k();
      SetLamp(1, lftRollovrLmps);
      break;

    case 40: //Rubber switches(22)
      Serial.println("Rubber switches(22) closed");
      Score1();
      break;

    default:
      Serial.println(" ");
      Serial.println("this sw not defined yet:");
      Serial.println(pinNumber);
      Serial.println(" ");
      break;
  }
}

void onPinDeactivated(int pinNumber) {
  // do something according to the _pinNR that is released
  switch (pinNumber) {
    case 24: //trough SW
      Serial.println("trough sw released");
      ballInTrough = false;
      break;

    case 31: //Shooter lane sw
      Serial.println("Shooter lane sw released");
      PlaySound(7);
      break;

    default:
      Serial.println(" ");
      Serial.println("this sw doesnt do anything when released:");
      Serial.println(pinNumber);
      Serial.println(" ");
      break;
  }
}

//Check for game over conditions and set inPlay = false if met, 0 balls left, etc
void CheckGOver() { 
  if (ballCnt == 0 && ballInTrough == true) { //Game is over
    PlaySound(2); //you ruined the experiment (Game Over)
    inPlay = false; //Makes Main Loop switch to Attract Mode. Final score should remain on display, until new game started, right?
  } else { //If balls left, kick ball back out
    PulseSol(troughSolPin);
  }
}

void BonusReset() {
  bonusStep = 0; bonusMult = 0;
  SetLamp(1, bonus2xLmp); SetLamp(1, bonus3xLmp); SetLamp(1, bonus4xLmp); SetLamp(1, bonus5xLmp);
  SetLamp(1, bonus1kLmp); SetLamp(1, bonus2kLmp); SetLamp(1, bonus3KLmp); SetLamp(1, bonus4kLmp); 
  SetLamp(1, bonus5kLmp); SetLamp(1, bonus6kLmp); SetLamp(1, bonus7kLmp); SetLamp(1, bonus8kLmp); 
  SetLamp(1, bonus9kLmp); SetLamp(1, bonus10kLmp); SetLamp(1, bonus20kLmps);
  
}

void SpecialReset() {
  special = false;
  topLane1 = false; SetLamp(1, topRollovrLmp1);
  topLane2 = false; SetLamp(1, topRollovrLmp2);
  topLane3 = false; SetLamp(1, topRollovrLmp3);
  topLane4 = false; SetLamp(1, topRollovrLmp4);
  topLane5 = false; SetLamp(1, topRollovrLmp5);
  topLane6 = false; digitalWrite(topLaneLmp6Pin, LOW);
  SetLamp(0, specialOutrLmps); SetLamp(0, specialInnrLmps); 
  digitalWrite(specialCnterLmpPin, LOW); 
}

void LanesReset() {
  leftTgtLmpsState = 1;
  rightTgtLmpsState = 1;
  rightLaneTgtValue = 0;
  SetLamp(1, rightLne3kLmp);
  digitalWrite(rightLne5kLmpPin, LOW);
}

//Bonus score is collected on ball drain, and is always in multiples of 1000. 1 x 0 to 5 x 20k
void BonusCollect() {
  int bonusScore = 0;
  if (inPlay == true) { //Dont want stuff to start happening if the game isnt running

    if (bonusStep > 0) {
      if (bonusStep >= 11) {
        bonusScore = 20;
      } else {
        bonusScore = bonusStep;
      }

      if (bonusMult > 0) {
        if (bonusMult >= 4) {
          bonusScore = bonusScore * 5;
        } else {
          bonusScore = bonusScore * (bonusMult+1);
        }
      }
    }
  //Bonus will be added to Score by subsequent iterations of the Main Loop
  score1k = score1k + bonusScore; 
  BonusReset();
  }
}

//We don't get points here, just change states. Points given when ball drains
void IncBonusStep() { 
  switch (bonusStep) {
    case 0: 
      bonusStep++;
      SetLamp(1, bonus20kLmps);
      SetLamp(0, bonus1kLmp);
      break;
    case 1: 
      bonusStep++;
      SetLamp(1, bonus1kLmp);
      SetLamp(0, bonus2kLmp);
      break;
    case 2: 
      bonusStep++;
      SetLamp(1, bonus2kLmp);
      SetLamp(0, bonus3KLmp);
      break;
    case 3: 
      bonusStep++;
      SetLamp(1, bonus3KLmp);
      SetLamp(0, bonus4kLmp);
      break;
    case 4: 
      bonusStep++;
      SetLamp(1, bonus4kLmp);
      SetLamp(0, bonus5kLmp);
      break;
    case 5: 
      bonusStep++;
      SetLamp(1, bonus5kLmp);
      SetLamp(0, bonus6kLmp);
      break;
    case 6: 
      bonusStep++;
      SetLamp(1, bonus6kLmp);
      SetLamp(0, bonus7kLmp);
      break;
    case 7: 
      bonusStep++;
      SetLamp(1, bonus7kLmp);
      SetLamp(0, bonus8kLmp);
      break;
    case 8: 
      bonusStep++;
      SetLamp(1, bonus8kLmp);
      SetLamp(0, bonus9kLmp);
      break;
    case 9: 
      bonusStep++;
      SetLamp(1, bonus9kLmp);
      SetLamp(0, bonus10kLmp);
      break;
    case 10: 
      bonusStep++; //we need the last inc. when collecting the score
      SetLamp(1, bonus10kLmp);
      SetLamp(0, bonus20kLmps);
      break;
    
    default:
      Serial.println(" ");
      Serial.println("Bonus score already maxed out at step 10:");
      Serial.println(bonusStep);
      //Serial.println(" ");
      break;   
  }
}

void IncBonusMult() { 
  switch (bonusMult) {
    case 0: 
      bonusMult++;
      SetLamp(1, bonus3xLmp);
      SetLamp(1, bonus4xLmp);
      SetLamp(1, bonus5xLmp);

      SetLamp(0, bonus2xLmp);
      break;
    case 1: 
      bonusMult++;
      SetLamp(1, bonus2xLmp);
      SetLamp(0, bonus3xLmp);
      break;
    case 2: 
      bonusMult++;
      SetLamp(1, bonus3xLmp);
      SetLamp(0, bonus4xLmp);
      break;
    case 3: 
      bonusMult++;
      SetLamp(1, bonus4xLmp);
      SetLamp(0, bonus5xLmp);
      break;
    case 4: //Maxed out here 
      SetLamp(1, bonus2xLmp);
      SetLamp(1, bonus3xLmp);
      SetLamp(1, bonus4xLmp);
      SetLamp(0, bonus5xLmp);
      break;
    
    default:
      Serial.println(" ");
      Serial.println("Bonus multiplier already maxed out:");
      Serial.println(bonusMult);
      Serial.println(" ");
      break;   
  }
}

//Called when hitting either of the top rollovers
void SpecialCheck() {
  if (topLane1 == true && topLane2 == true && topLane3 == true 
  && topLane4 == true && topLane5 == true && topLane6 == true) {
    special = true; //Special lamps are scrolled from game loop when true
    
    //reset special conds.
    topLane1 = false; SetLamp(1, topRollovrLmp1);
    topLane2 = false; SetLamp(1, topRollovrLmp2);
    topLane3 = false; SetLamp(1, topRollovrLmp3);
    topLane4 = false; SetLamp(1, topRollovrLmp4);
    topLane5 = false; SetLamp(1, topRollovrLmp5);
    topLane6 = false; digitalWrite(topLaneLmp6Pin, LOW);
    PlaySound(6); //You have the Special
  }
}

void StepRightLaneVal() { //We dont get points here, just set value of target in right lane
  switch (rightLaneTgtValue) { 
    case 0: 
      rightLaneTgtValue++;
      SetLamp(0, rightLne3kLmp);
      digitalWrite(rightLne5kLmpPin, LOW);
      break;
    case 1:
      rightLaneTgtValue++;
      SetLamp(1, rightLne3kLmp);
      digitalWrite(rightLne5kLmpPin, HIGH);
      break;
    case 2:
      rightLaneTgtValue = 0;
      SetLamp(1, rightLne3kLmp);
      digitalWrite(rightLne5kLmpPin, LOW);
      break;
  }
}

void SpecialLmpsScroll() {
  switch (specialScroll) { //We don't get points here, just scroll lamps
    case 0: 
      SetLamp(0, specialOutrLmps);
      SetLamp(1, specialInnrLmps);
      digitalWrite(specialCnterLmpPin, LOW);
      specialScroll++;
      break;
    case 1:
      SetLamp(1, specialOutrLmps);
      SetLamp(0, specialInnrLmps);
      digitalWrite(specialCnterLmpPin, LOW);
      specialScroll++;
      break;
    case 2:
      SetLamp(1, specialOutrLmps);
      SetLamp(1, specialInnrLmps);
      digitalWrite(specialCnterLmpPin, HIGH);
      specialScroll = 0;
      break;
  }
}

//Resets score counters
void ScoreReset() {
  score[0] = 0;
  score[1] = 0;
  score[2] = 0; score100 = 0;
  score[3] = 0; score1k = 0;
  score[4] = 0; score10k = 0;
  score[5] = 0;
}

void PulseSol(int solNo) {
  digitalWrite(solNo, HIGH);
  delay(stdSolPulseTime);
  digitalWrite(solNo, LOW);
}
