//Reading buttons through a binary encoder circuit
//2016 Michael Niemi
//MIT License
//

#define NUM_STRINGS 4
#define NUM_FRETS 8
#define VELOCITY 100
#define noteON 145
#define noteOFF 129 
#define BASENOTE 28
#define SEQUENCE 0
#define KEYPAD 1

#define NUM_BUTTONS 12


int mapping[] {0,1,0,2,0,3,10,4,9,5,11,6,12,7,8,0};
//int mapping[] {0,1,0,2,0,3,10,4,9,5,12,6,13,7,8,0};
int buttonState[NUM_BUTTONS];
int currentNote = 0;
int mode = SEQUENCE;

int loopcount = 0;
int prevbutton = 0;

bool stepStatus[NUM_BUTTONS];

unsigned long previousMillis = 0;
const long interval = 200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  pinMode(2 , INPUT_PULLUP);
  pinMode(3 , INPUT_PULLUP);
  pinMode(4 , INPUT_PULLUP);
  pinMode(5 , INPUT_PULLUP);
}

void loop() {

  //Serial.println(readSequential());
  //Serial.println(15-readDirect());//
//  Serial.println(readDirect());

if (mode == KEYPAD){ 
  int buttonIn=mapping[readDirect()];
//  Serial.println(buttonIn);

  if( buttonIn != currentNote ){
    if( buttonIn == 0){
      MIDImessage(noteOFF, currentNote+BASENOTE, VELOCITY);
    }
    if( currentNote == 0){
      MIDImessage(noteON, buttonIn+BASENOTE, VELOCITY); 
    }
    currentNote = buttonIn;
  }
  delay(1);
  }

  if (mode == SEQUENCE){
    ////Check Input, update status in stepStatus
    updateButtons();

   unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
      previousMillis = currentMillis;
     //Send midi message for active buttons
      if (stepStatus[loopcount]){
          MIDImessage(noteON, BASENOTE+10, VELOCITY); 
          MIDImessage(noteOFF, BASENOTE+10, VELOCITY);
        }
        
      loopcount++;
      if(loopcount == 12){loopcount=0;}
    }
  }
}


void updateButtons(){
  int buttonIn=mapping[readDirect()];
  if (buttonIn != prevbutton){
    if(buttonIn != 0){
      stepStatus[buttonIn-1]=!(stepStatus[buttonIn-1]);
//      Serial.println("buttonchanged");
//      Serial.println(stepStatus[buttonIn-1]);
    }
    prevbutton=buttonIn;
  }
}


byte readSequential(){
  byte buttonId;
  for(int i=2; i<6; i++){
    if( digitalRead(i) == 1){
      //We will "add" the value of our pin to the byte. The << shifts it to the right bit, indexed by out loop
      buttonId = buttonId | (1 << (i-2));
    }
  }

  return buttonId;
}

byte readDirect(){
  
  byte buttonId =  PIND >> 2;
  return buttonId;
  
}

void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}

