int sndSel[] = {8, 9, 10, 11};
int sndPlay = 12;
int stdLatchSndDelay = 20;
int attractSndCnt = 0;

void ClearSoundPins() {
  digitalWrite(sndPlay, HIGH); //Enable is active LOW
  digitalWrite(sndSel[0], LOW);
  digitalWrite(sndSel[1], LOW);
  digitalWrite(sndSel[2], LOW);
  digitalWrite(sndSel[3], LOW);
}

void PlaySound(int sound) { //Can play 10 sounds, so sound is 0-9
  digitalWrite(sndPlay, HIGH); //Enable is active LOW
  switch (sound) {
    case 0: //play the time machine is waiting
      digitalWrite(sndSel[0], LOW);
      digitalWrite(sndSel[1], LOW);
      digitalWrite(sndSel[2], LOW);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchSndDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 1: //now we start
      digitalWrite(sndSel[0], HIGH);
      digitalWrite(sndSel[1], LOW);
      digitalWrite(sndSel[2], LOW);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchSndDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 2: //you ruined the experiment
      digitalWrite(sndSel[0], LOW);
      digitalWrite(sndSel[1], HIGH);
      digitalWrite(sndSel[2], LOW);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchSndDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 3: //Go into the future 
      digitalWrite(sndSel[0], HIGH);
      digitalWrite(sndSel[1], HIGH);
      digitalWrite(sndSel[2], LOW);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchSndDelay);
      digitalWrite(sndPlay, HIGH); //stops repeat playing 
      break;
    case 4: //Another coin please
      digitalWrite(sndSel[0], LOW);
      digitalWrite(sndSel[1], LOW);
      digitalWrite(sndSel[2], HIGH);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchSndDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 5: //Drained sfx
      digitalWrite(sndSel[0], HIGH);
      digitalWrite(sndSel[1], LOW);
      digitalWrite(sndSel[2], HIGH);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchSndDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 6: //You have the Special
      digitalWrite(sndSel[0], LOW);
      digitalWrite(sndSel[1], HIGH);
      digitalWrite(sndSel[2], HIGH);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchSndDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 7: //SFX
      digitalWrite(sndSel[0], HIGH);
      digitalWrite(sndSel[1], HIGH);
      digitalWrite(sndSel[2], HIGH);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchSndDelay);
      digitalWrite(sndPlay, HIGH);
      break;      
    
    default:
      Serial.println(" ");
      Serial.println("this sound not defined yet:");
      Serial.println(sound);
      Serial.println(" ");
      break;
  }
}

void AttractSounds() {
  Serial.println("AttractSounds, attractSndCnt:");     
  Serial.println(attractSndCnt);
  //Serial.println("AttractLamps now, attractLmpCnt:  ");
  //Serial.println(attractLmpCnt);
  if (attractSndCnt > 10) {
    attractSndCnt = 0;
    //ClearSoundPins();
    //digitalWrite(53, HIGH);  
    //digitalWrite(68, HIGH);  
    Serial.println("AttractSnd reset+PlaySound()"); 
    //SetDispDigit(0, attractDispCnt);   
    PlaySound(0);              
  } else {
    attractSndCnt++; 
    // digitalWrite(sndPlay, HIGH); //Enable is active LOW
    // digitalWrite(sndSel[0], HIGH);
    // digitalWrite(sndSel[1], HIGH);
    // digitalWrite(sndSel[2], LOW);
    // digitalWrite(sndSel[3], LOW);
    // digitalWrite(sndPlay, LOW); 
    // delay(20);
    // digitalWrite(sndPlay, HIGH);
    
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
}

