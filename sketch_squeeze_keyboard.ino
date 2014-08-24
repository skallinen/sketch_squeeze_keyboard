//USE_GITHUB_USERNAME=skallinen

#include <Stepper.h>
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>

#define DIGITAL_SENSOR_THRESHOLD 15
#define DIGITAL_SENSOR_MAXIMUM 500

#define ANALOG_SENSOR_THRESHOLD 90
#define ANALOG_HARD_PRESS_THRESHOLD 550

#define DIGITAL_SENSOR_COUNT 3
#define ANALOG_SENSOR_COUNT 6

SoftwareSerial BT = SoftwareSerial(4, 3);

String keypressPattern;
String keypressPatternPrevious;
String keyboardStateCurrent;
int modeToggleKey = 0;
int mode = 0;



int keypressPatternCycleCount;
int measurmentCycles;
int inputTime;
int inputTimeCounter;


int fsrPinD[DIGITAL_SENSOR_COUNT] = {
  2,6,7};     // the FSR and cap are connected to pin2
int fsrReadingDigital[DIGITAL_SENSOR_COUNT];     // the digital reading

int fsrPinA[ANALOG_SENSOR_COUNT] = {
  0,1,2,3,4,5};     // the FSR and cap are connected to pin2

int fsrReadingAnalog[ANALOG_SENSOR_COUNT];     // the digital reading

int fsrReadingAnalogPrev[ANALOG_SENSOR_COUNT];     // reading  from preveious cycle
int fsrReadingDigitalPrev[DIGITAL_SENSOR_COUNT];     // reading  from preveious cycle

int fsrReadingAnalogDiff[ANALOG_SENSOR_COUNT];     // reading  from preveious cycle
int fsrReadingDigitalDiff[DIGITAL_SENSOR_COUNT];     // reading  from preveious cycle

int fsrReadingDigitalPressed[DIGITAL_SENSOR_COUNT];     // Key pressed
int fsrReadingAnalogPressed[ANALOG_SENSOR_COUNT];     // Key pressed




int ledPins[3] = {
  13,12,11};    // you can just use the 'built in' LED

const char keymap_001[] PROGMEM ="9999999999asciiee";
const char keymap_002[] PROGMEM ="8888888888asciitt";
const char keymap_003[] PROGMEM ="0000001000raw0x65"; // KEY_e
const char keymap_004[] PROGMEM ="0000000100asciitt";
const char keymap_005[] PROGMEM ="0000000010asciiaa";
const char keymap_006[] PROGMEM ="0000000001asciioo";
const char keymap_007[] PROGMEM ="0000001100asciiii";
const char keymap_008[] PROGMEM ="0000000110asciinn";
const char keymap_009[] PROGMEM ="0000000011asciiss";
const char keymap_010[] PROGMEM ="0000001010asciirr";
const char keymap_011[] PROGMEM ="0000000101asciihh";
const char keymap_012[] PROGMEM ="0000001110asciill";
const char keymap_013[] PROGMEM ="0000000111asciidd";
const char keymap_014[] PROGMEM ="0000001111asciicc";
const char keymap_015[] PROGMEM ="0001001000asciiuu";
const char keymap_016[] PROGMEM ="0001000100asciimm";
const char keymap_017[] PROGMEM ="0001000010asciiff";
const char keymap_018[] PROGMEM ="0001000001asciigg";
const char keymap_019[] PROGMEM ="0001001100asciipp";
const char keymap_020[] PROGMEM ="0001000110asciiyy";
const char keymap_021[] PROGMEM ="0001000011asciiww";
const char keymap_022[] PROGMEM ="0001001010asciibb";
const char keymap_023[] PROGMEM ="0001000101asciivv";
const char keymap_024[] PROGMEM ="0001001110asciikk";
const char keymap_025[] PROGMEM ="0001000111asciixx";
const char keymap_026[] PROGMEM ="0001001111asciiqq";
const char keymap_027[] PROGMEM ="0001001001asciizz";
const char keymap_028[] PROGMEM ="1000001000asciiEe";
const char keymap_029[] PROGMEM ="1000000100asciiTt";
const char keymap_030[] PROGMEM ="1000000010asciiAa";
const char keymap_031[] PROGMEM ="1000000001asciiOo";
const char keymap_032[] PROGMEM ="1000001100asciiIi";
const char keymap_033[] PROGMEM ="1000000110asciiNn";
const char keymap_034[] PROGMEM ="1000000011asciiSs";
const char keymap_035[] PROGMEM ="1000001010asciiRr";
const char keymap_036[] PROGMEM ="1000000101asciiHh";
const char keymap_037[] PROGMEM ="1000001110asciiLl";
const char keymap_038[] PROGMEM ="1000000111asciiDd";
const char keymap_039[] PROGMEM ="1000001111asciiCc";
const char keymap_040[] PROGMEM ="1001001000asciiUu";
const char keymap_041[] PROGMEM ="1001000100asciiMm";
const char keymap_042[] PROGMEM ="1001000010asciiFf";
const char keymap_043[] PROGMEM ="1001000001asciiGg";
const char keymap_044[] PROGMEM ="1001001100asciiPp";
const char keymap_045[] PROGMEM ="1001000110asciiYy";
const char keymap_046[] PROGMEM ="1001000011asciiWw";
const char keymap_047[] PROGMEM ="1001001010asciiBb";
const char keymap_048[] PROGMEM ="1001000101asciiVv";
const char keymap_049[] PROGMEM ="1001001110asciiKk";
const char keymap_050[] PROGMEM ="1001000111asciiXx";
const char keymap_051[] PROGMEM ="1001001111asciiQq";
const char keymap_052[] PROGMEM ="1001001001asciiZz";
const char keymap_053[] PROGMEM ="0000100000raw0x2CSPACE";
const char keymap_054[] PROGMEM ="0000010000raw0x2ABACKSPACE";
const char keymap_055[] PROGMEM ="0000110000raw0x28ENTER";
const char keymap_056[] PROGMEM ="0010001000ascii,e";
const char keymap_057[] PROGMEM ="1010001000ascii.e";
const char keymap_058[] PROGMEM ="0010000100ascii-t"; // -
const char keymap_059[] PROGMEM ="1010000100ascii_t"; // _
const char keymap_060[] PROGMEM ="0010000010ascii\"a";
const char keymap_061[] PROGMEM ="1010000010ascii\'a";
const char keymap_062[] PROGMEM ="0010000001ascii(o";
const char keymap_063[] PROGMEM ="1010000001ascii)o";
const char keymap_064[] PROGMEM ="0010001100ascii;i";
const char keymap_065[] PROGMEM ="1010001100ascii:i";
const char keymap_066[] PROGMEM ="0010000110ascii/n";
const char keymap_067[] PROGMEM ="1010000110ascii\\n";
const char keymap_068[] PROGMEM ="0010000011ascii=s";
const char keymap_069[] PROGMEM ="1010000011ascii*s";
const char keymap_070[] PROGMEM ="0010001010ascii!r";
const char keymap_071[] PROGMEM ="1010001010ascii?r";
const char keymap_072[] PROGMEM ="0010000101ascii$h";
const char keymap_073[] PROGMEM ="1010000101ascii€h";
const char keymap_074[] PROGMEM ="0010001110ascii[l";
const char keymap_075[] PROGMEM ="1010001110ascii]l";
const char keymap_076[] PROGMEM ="0010000111ascii<d";
const char keymap_077[] PROGMEM ="1010000111ascii>d";
const char keymap_078[] PROGMEM ="0010001111ascii{c";
const char keymap_079[] PROGMEM ="1010001111ascii}c";
const char keymap_080[] PROGMEM ="0011001000ascii+u";
const char keymap_081[] PROGMEM ="1011001000ascii-u";
const char keymap_082[] PROGMEM ="0011000100ascii|m";
const char keymap_083[] PROGMEM ="1011000100ascii&m";
const char keymap_084[] PROGMEM ="0011000010ascii%f";
const char keymap_085[] PROGMEM ="1011000010ascii^f";
const char keymap_086[] PROGMEM ="0011000001ascii`g";
const char keymap_087[] PROGMEM ="1011000001ascii~g";
const char keymap_088[] PROGMEM ="0011001100ascii@p";
const char keymap_089[] PROGMEM ="1011001100ascii#p";
const char keymap_090[] PROGMEM ="0011000110asciiyy";
const char keymap_091[] PROGMEM ="1011000110asciiYy";
const char keymap_092[] PROGMEM ="0011000011asciiww";
const char keymap_093[] PROGMEM ="1011000011asciiWw";
const char keymap_094[] PROGMEM ="0011001010ascii1b";
const char keymap_095[] PROGMEM ="1011001010ascii2b";
const char keymap_096[] PROGMEM ="0011000101ascii3v";
const char keymap_097[] PROGMEM ="1011000101ascii4v";
const char keymap_098[] PROGMEM ="0011001110ascii5k";
const char keymap_099[] PROGMEM ="1011001110ascii6k";
const char keymap_100[] PROGMEM ="0011000111ascii7x";
const char keymap_101[] PROGMEM ="1011000111ascii8x";
const char keymap_102[] PROGMEM ="0011001111ascii9q";
const char keymap_103[] PROGMEM ="1011001111ascii0q";
const char keymap_104[] PROGMEM ="0011001001asciizz";
const char keymap_105[] PROGMEM ="1011001001asciiZz";
const char keymap_106[] PROGMEM ="1000100000raw0x2CSPACE";
const char keymap_107[] PROGMEM ="1000010000raw0x2ABACKSPACE";
const char keymap_108[] PROGMEM ="1000110000raw0x28ENTER";
const char keymap_109[] PROGMEM ="0010100000raw0x2CSPACE";
const char keymap_110[] PROGMEM ="0010010000raw0x2ABACKSPACE";
const char keymap_111[] PROGMEM ="0010110000raw0x28ENTER";
const char keymap_112[] PROGMEM ="1010100000raw0x2CSPACE";
const char keymap_113[] PROGMEM ="1010010000raw0x2ABACKSPACE";
const char keymap_114[] PROGMEM ="1010110000raw0x28ENTER";

const char * const  string_table[] PROGMEM = 
{
  keymap_001,
  keymap_002,
  keymap_003,
  keymap_004,
  keymap_005,
  keymap_006,
  keymap_007,
  keymap_008,
  keymap_009,
  keymap_010,
  keymap_011,
  keymap_012,
  keymap_013,
  keymap_014,
  keymap_015,
  keymap_016,
  keymap_017,
  keymap_018,
  keymap_019,
  keymap_020,
  keymap_021,
  keymap_022,
  keymap_023,
  keymap_024,
  keymap_025,
  keymap_026,
  keymap_027,
  keymap_028,
  keymap_029,
  keymap_030,
  keymap_031,
  keymap_032,
  keymap_033,
  keymap_034,
  keymap_035,
  keymap_036,
  keymap_037,
  keymap_038,
  keymap_039,
  keymap_040,
  keymap_041,
  keymap_042,
  keymap_043,
  keymap_044,
  keymap_045,
  keymap_046,
  keymap_047,
  keymap_048,
  keymap_049,
  keymap_050,
  keymap_051,
  keymap_052,
  keymap_053,
  keymap_054,
  keymap_055,
  keymap_056,
  keymap_057,
  keymap_058,
  keymap_059,
  keymap_060,
  keymap_061,
  keymap_062,
  keymap_063,
  keymap_064,
  keymap_065,
  keymap_066,
  keymap_067,
  keymap_068,
  keymap_069,
  keymap_070,
  keymap_071,
  keymap_072,
  keymap_073,
  keymap_074,
  keymap_075,
  keymap_076,
  keymap_077,
  keymap_078,
  keymap_079,
  keymap_080,
  keymap_081,
  keymap_082,
  keymap_083,
  keymap_084,
  keymap_085,
  keymap_086,
  keymap_087,
  keymap_088,
  keymap_089,
  keymap_090,
  keymap_091,
  keymap_092,
  keymap_093,
  keymap_094,
  keymap_095,
  keymap_096,
  keymap_097,
  keymap_098,
  keymap_099,
  keymap_100,
  keymap_101,
  keymap_102,
  keymap_103,
  keymap_104,
  keymap_105,
  keymap_106,
  keymap_107,
  keymap_108,
  keymap_109,
  keymap_110,
  keymap_111,
  keymap_112,
  keymap_113,
  keymap_114
};



String parseKeysFromFlash(String needle){
  char buffer[30];
  String output = "";
  for (int i = 0; i < 114; i++)
  {
    strcpy_P(buffer, (char*)pgm_read_word(&(string_table[i])));
    //   Serial.println( buffer );
    //Serial.println( String(buffer).substring(0,10) );

    if (needle == String(buffer).substring(0,10)) output = String(buffer);
    //Serial.println(String(buffer).substring(15,16));
  }
  return output;
}

String Pattern(String keymapString) {
  return keymapString.substring(0,10);
}

String Type(String keymapString) {
 /* String response = "";
  if (keymapString.substring(10,13) == "raw"){
    response = "raw";
    Serial.println("if starts with condition!");
  } 
  else {
    response = keymapString.substring(10,15);
  }*/
  return keymapString.substring(10,13);
  //return response;
}
String AsciiCharacter(String keymapString) {
  return keymapString.substring(15,16);
}

String NonPrintableByteStr(String keymapString) {
    return keymapString.substring(13,17);
}



void mouseCommand(uint8_t buttons, uint8_t x, uint8_t y) {
  BT.write(0xFD);
  BT.write((byte)0x00);
  BT.write((byte)0x03);
  BT.write(buttons);
  BT.write(x);
  BT.write(y);
  BT.write((byte)0x00);
  BT.write((byte)0x00);
  BT.write((byte)0x00);
}

void keyCommand(uint8_t modifiers, uint8_t keycode1, uint8_t keycode2 = 0, uint8_t keycode3 = 0, 
uint8_t keycode4 = 0, uint8_t keycode5 = 0, uint8_t keycode6 = 0) {
  BT.write(0xFD);       // our command
  BT.write(modifiers);  // modifier!
  BT.write((byte)0x00); // 0x00  
  BT.write(keycode1);   // key code #1
  BT.write(keycode2); // key code #2
  BT.write(keycode3); // key code #3
  BT.write(keycode4); // key code #4
  BT.write(keycode5); // key code #5
  BT.write(keycode6); // key code #6
}





// Uses a digital pin to measure a resistor (like an FSR or photocell!)
// We do this by having the resistor feed current into a capacitor and
// counting how long it takes to get to Vcc/2 (for most arduinos, thats 2.5V)
int RCtime(int RCpin) {
  int reading = 0;  // start with 0

    // set the pin to an output and pull to LOW (ground)
  pinMode(RCpin, OUTPUT);
  digitalWrite(RCpin, LOW);

  // Now set the pin to an input and...
  pinMode(RCpin, INPUT);
  while (digitalRead(RCpin) == LOW) { // count how long it takes to rise up to HIGH
    reading++;      // increment to keep track of time 

    if (reading == DIGITAL_SENSOR_MAXIMUM) {
      // if we got this far, the resistance is so high
      // its likely that nothing is connected! 
      break;           // leave the loop
    }
  }
  // OK either we maxed out at 30000 or hopefully got a reading, return the count

  return reading;
}

String keysPressed(void) {
  measurmentCycles++;

  for(int i = 0; i < DIGITAL_SENSOR_COUNT; i++){
    if (fsrReadingDigital[i] < DIGITAL_SENSOR_THRESHOLD ) {
      // Serial.println("D1 reading = " + String(fsrReadingDigital1));
      fsrReadingDigitalPressed[i] = 1;
    } 
    else {
      fsrReadingDigitalPressed[i] = 0;
    }
  }

  for(int i = 0; i < ANALOG_SENSOR_COUNT; i++){
    if (fsrReadingAnalog[i] > ANALOG_SENSOR_THRESHOLD ) {
      // Serial.println("D1 reading = " + String(fsrReadingDigital1));
      fsrReadingAnalogPressed[i] = 1;
    } 
    else {
      fsrReadingAnalogPressed[i] = 0;
    }
  }

  keypressPatternPrevious = keypressPattern;
  //Serial.println(keypressPattern);
  String ForcePress = ( averagePressForce() > ANALOG_HARD_PRESS_THRESHOLD ) ? "1" : "0";
  keypressPattern =  ForcePress +
    String(fsrReadingDigitalPressed[0]) + 
    String(fsrReadingDigitalPressed[1]) +
    String(fsrReadingDigitalPressed[2]) +
    String(fsrReadingAnalogPressed[0]) + 
    String(fsrReadingAnalogPressed[1]) + 
    String(fsrReadingAnalogPressed[2]) + 
    String(fsrReadingAnalogPressed[3]) + 
    String(fsrReadingAnalogPressed[4]) + 
    String(fsrReadingAnalogPressed[5]);
  //Serial.println(keypressPattern);

  return keypressPattern;


}

void readSensors(void) {
  //save last cycles readings
  for(int i = 0; i < DIGITAL_SENSOR_COUNT; i++){
    fsrReadingDigitalPrev[i] = fsrReadingDigital[i];
    fsrReadingDigital[i] = RCtime(fsrPinD[i]);
  }

  for(int i = 0; i < ANALOG_SENSOR_COUNT; i++){
    fsrReadingAnalogPrev[i] = fsrReadingAnalog[i];
    fsrReadingAnalog[i] = analogRead(fsrPinA[i]);
  }

}

int averagePressForce(void){
  //    Serial.println("yay");

  int total = 0;
  int CycleCount = 0;
  for(int i = 0; i < ANALOG_SENSOR_COUNT; i++){
    if (fsrReadingAnalog[i] > ANALOG_SENSOR_THRESHOLD) {
      total += fsrReadingAnalog[i];
      CycleCount++;
    }
  }
  return (CycleCount != 0 ) ? total/CycleCount : 0;
}

String keyboardState(void){
  String result;
  keypressPattern = keysPressed();
  //Serial.println(keypressPattern);



  if (keypressPattern == keypressPatternPrevious || keypressPattern ==  flipForcePressBit(keypressPatternPrevious)){
    keypressPatternCycleCount++; 
  } 
  else {
    //result = "CHANGED\n ------------------------->" + keypressPattern;
    keypressPatternCycleCount = 0;

  }

  if(keypressPattern=="0000000000"){ // no keys are pressed
    digitalWrite(ledPins[0], LOW);
    result = "IDLE";
  } 
  else {
    result = "WAITING";
    if ( keypressPatternCycleCount == 3 || keypressPatternCycleCount > 60) {
      digitalWrite(ledPins[0], HIGH);
      //Serial.println(keypressPatternCycleCount);

      result = keypressPattern;
      //result = "result------------------------->" + ;
    }
  }
  return result;
}

String flipForcePressBit(String input) {
  if (input.charAt(0) == '0') {
    input.setCharAt(0, '1');
  }
  else if (input.charAt(0) == '1') {
    input.setCharAt(0, '0');
  }
  return input;
}


unsigned int hexToInt(String hex) {
  int intValue = (int)strtol(&(hex[0]), NULL, 16);
  return intValue;
}

void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(ledPins[0], OUTPUT);  // have an LED for output 
  pinMode(ledPins[1], OUTPUT);  // have an LED for output 
  pinMode(ledPins[2], OUTPUT);  // have an LED for output 


}



void loop(void) {
  readSensors();
  keyboardStateCurrent = keyboardState();
  String type = "";
  String command = "";
  int commandcode;
  char modeIndicator;




  delay(1);

  if(keyboardStateCurrent == "0010000000") {
    if(modeToggleKey < 2) {
      modeToggleKey++;
    }
    else{
      modeToggleKey=0;
    }
  }
  switch (modeToggleKey) {
  case 1:
    modeIndicator = '1';
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[2], HIGH);

    break;
  case 2:
    modeIndicator = '2';
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], LOW);
    break;
  default: 
    modeIndicator = '0';
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[2], LOW);

  }

  Serial.println(keyboardStateCurrent);
    keyboardStateCurrent.setCharAt(2,modeIndicator);
   Serial.println(keyboardStateCurrent);
   String keymapstring = "";
    keymapstring = parseKeysFromFlash(keyboardStateCurrent);
    type = Type(keymapstring);
    if (type == "asc") {
      command = AsciiCharacter(keymapstring);
    } 
    else {
      command = NonPrintableByteStr(keymapstring);

    }
    if (command != "") Serial.println(command);
    if (type != "") Serial.println(type);
    if (type == "asc") BT.print(command);
    if (type == "raw") {
      commandcode = hexToInt(command);
      //   Serial.println(commancode);
      keyCommand(0,commandcode);
      delay(10);
      keyCommand(0,0);
    }



}



