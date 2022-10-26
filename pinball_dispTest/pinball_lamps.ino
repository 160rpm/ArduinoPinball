
int lampLatchDelay = 1;
int lampDataLn = 14;
int lampBitSel[] = {15,16,17};
int lampLatchEnable = 18;
int lampLatchSel[] = {21,20,19}; //Driver board has 6 latches for 3 banks, but currently only two banks used, so only actually need 4 latches -> 2 select lines
char GetHL[2][5] = {"LOW", "HIGH"};

int lamp1 = 0;
int lamp2 = 1;

int lampStates[] = {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  //Bank 1
                    0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0}; //Bank 2

//First 3 digits are latch adr, next 3 are bit adr. Treat as binary values, 0 or 1 only.
int lampMap[32][6] =  { {0, 0, 0, 0, 0, 0}, {0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 0}, {0, 0, 0, 1, 1, 0},    //lamps 0-3
                        {0, 0, 0, 0, 0, 1}, {0, 0, 0, 1, 0, 1}, {0, 0, 0, 0, 1, 1}, {0, 0, 0 , 1, 1, 1},   //lamps 4-7
                        {1, 0, 0, 0, 0, 0}, {1, 0, 0, 1, 0, 0}, {1, 0, 0, 0, 1, 0}, {1, 0, 0, 1, 1, 0},    //lamps 8-11
                        {1, 0, 0, 0, 0, 1}, {1, 0, 0, 1, 0, 1}, {1, 0, 0, 0, 1, 1}, {1, 0, 0 , 1, 1, 1}    //lamps 12-15
                        {0, 1, 0, 0, 0, 0}, {0, 1, 0, 1, 0, 0}, {0, 1, 0, 0, 1, 0}, {0, 1, 0, 1, 1, 0},    //lamps 16-19
                        {0, 1, 0, 0, 0, 1}, {0, 1, 0, 1, 0, 1}, {0, 1, 0, 0, 1, 1}, {0, 1, 0 , 1, 1, 1},   //lamps 20-23
                        {1, 1, 0, 0, 0, 0}, {1, 1, 0, 1, 0, 0}, {1, 1, 0, 0, 1, 0}, {1, 1, 0, 1, 1, 0},    //lamps 24-27
                        {1, 1, 0, 0, 0, 1}, {1, 1, 0, 1, 0, 1}, {1, 1, 0, 0, 1, 1}, {1, 1, 0 , 1, 1, 1} }; //lamps 28-31

// do something like ToggleLamp(int state, int lampMap[lamp1]){} to turn individual lamps on or off. 
SetLamp(int state, int lampNo) {
    digitalWrite(lampLatchEnable, HIGH); //always deselect latches before setting addresses  
    digitalWrite(lampLatchSel[0], GetHL[lampMap[lampNo][0]]);
    digitalWrite(lampLatchSel[1], GetHL[lampMap[lampNo][1]]);
    digitalWrite(lampLatchSel[2], GetHL[lampMap[lampNo][2]]);

    digitalWrite(lampBitSel[0], GetHL[lampMap[lampNo][3]]);
    digitalWrite(lampBitSel[1], GetHL[lampMap[lampNo][4]]);
    digitalWrite(lampBitSel[2], GetHL[lampMap[lampNo][5]]);

    digitalWrite(lampDataLn, GetHL[state]); //Lamp relays trigger on LOW, so 'state = 0' -> lamp on
    delay(lampLatchDelay);
    digitalWrite(lampLatchEnable, LOW); //Sets state of lamp as configured above 
    delay(lampLatchDelay);
    digitalWrite(lampLatchEnable, HIGH);
    lampStates[lampNo] = state; //Update map with the state we just set this lamp to
}

