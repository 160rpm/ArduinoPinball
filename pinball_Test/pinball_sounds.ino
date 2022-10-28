int sndSel[] = {8, 9, 10, 11, 12};
int sndPlay = 13;

void ClearSoundPins() {
  digitalWrite(sndPlay, HIGH); //Enable is active LOW
  for (int cs = 0; cs < 5; cs++) {
    digitalWrite(sndSel[cs], LOW);
  }
}