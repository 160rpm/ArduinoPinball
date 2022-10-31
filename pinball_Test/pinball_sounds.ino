int sndSel[] = {8, 9, 10, 11};
int sndPlay = 12;

void ClearSoundPins() {
  digitalWrite(sndPlay, HIGH); //Enable is active LOW
  for (int cs = 0; cs < 4; cs++) {
    digitalWrite(sndSel[cs], LOW);
  }
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
      delay(stdLatchDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 1: //now we start
      digitalWrite(sndSel[0], HIGH);
      digitalWrite(sndSel[1], LOW);
      digitalWrite(sndSel[2], LOW);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 2: //you ruined the experiment
      digitalWrite(sndSel[0], LOW);
      digitalWrite(sndSel[1], HIGH);
      digitalWrite(sndSel[2], LOW);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchDelay);
      digitalWrite(sndPlay, HIGH);
      break;
    case 3: //Go into the future 
      digitalWrite(sndSel[0], HIGH);
      digitalWrite(sndSel[1], HIGH);
      digitalWrite(sndSel[2], LOW);
      digitalWrite(sndSel[3], LOW);
      digitalWrite(sndPlay, LOW); //start playing
      delay(stdLatchDelay);
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
