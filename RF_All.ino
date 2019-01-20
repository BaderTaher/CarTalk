#include <Adafruit_NeoPixel.h>

#ifdef AVR

#include <avr/power.h>

#endif

#define PIN 6

#define PINE 4

#define ldelay 10

Adafruit_NeoPixel stripr = Adafruit_NeoPixel(8, PINE, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel stripl = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket

#if defined (AVR_ATtiny85)

if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

// End of trinket special code

Serial.begin(115200);

stripr.begin();

stripr.show(); // Initialize all pixels to 'off'

stripl.begin();

stripl.show(); // Initialize all pixels to 'off'

}

int messagetype = 0;

int outputl = 0;

int outputr = 0;

int potentiometer = A0;

int carspeed;

int previousspeed=0;

int high = 128; //Max brightness

int low = 0; //Min brightness

uint32_t red = stripr.Color(high, low, low); //saves the white color full rgb value as on

uint32_t off = stripr.Color(low, low, low); //no rgb values so off

uint32_t red2 = stripl.Color(high, low, low); //saves the white color full rgb value as on

uint32_t off2 = stripl.Color(low, low, low); //no rgb values so off

int index = 1;

int divisible;

bool updateSpeed = LOW;

void SpeedConverter(int input)

{

if(input > 63)

{

for(int x = 0; x < 8; x++)
{
  stripr.setPixelColor(x, red);
}
stripr.show();
}

else if(abs(input - previousspeed) >=2)

{

previousspeed = input;

int divide;

for(int x=6; x >0; x-- )

{

divide = input%2;
    if (divide == 0){
      stripr.setPixelColor(x, off2);
    }
    else if (divide == 1){
      stripr.setPixelColor(x, red2);
    }
    input = input/2;
}

stripr.setPixelColor(0,red);

stripr.setPixelColor(7,red);

stripr.show();

}

}

void LeftUpdate(int input)

{

int i=0;

while (input>0){

divisible = input%2;

if (divisible == 0){

stripl.setPixelColor(i, off);
}

else if (divisible == 1){

stripl.setPixelColor(i, red);
}

input=input/2;
  i++;
}

stripl.show();

}

void RightUpdate(int input)

{

int divide;

for(int i =7; i >= 0; i--)

{

divide = input%2;
    if (divide == 0){
      stripr.setPixelColor(i, off2);
    }
    else if (divide == 1){
      stripr.setPixelColor(i, red2);
    }
    input = input/2;
}

stripr.show();

}

void loop() {

messagetype = Serial.read();

if(updateSpeed == HIGH)
  {
  carspeed = round(analogRead(potentiometer)/10.2);
  // Serial.println(carspeed);
  SpeedConverter(carspeed);

  delay(20);
  }
if(messagetype != -1 && messagetype != 10){

for(int n=0; n<8; n++){
  stripr.setPixelColor(n, off);
  stripl.setPixelColor(n, off);
  }

  Serial.print("Received: ");
  Serial.print(messagetype, DEC);
  Serial.print(" ");

  switch (messagetype){
  case 97: //only left
    Serial.println("General Message Available 10111101");
    outputl = 189;
    LeftUpdate(outputl);
    updateSpeed = LOW;
  break;
  case 98://both
    Serial.println("No Message Available 11111111");
    outputl = 255;
    outputr = 255;
    LeftUpdate(outputl);
    RightUpdate(outputr);
    updateSpeed = LOW;
  break;
  case 99://only left
    Serial.println("Status Message Available 10001111");
    outputl = 143;
    LeftUpdate(outputl);
    Serial.println("Speed");
    carspeed = analogRead(potentiometer)/10.2;
    //output = carspeed | B10000001;
    SpeedConverter(carspeed);
    Serial.println(carspeed);
    updateSpeed = HIGH;
  break;
  case 100://only left
    Serial.println("Warning Message Available 11110001");
    outputl = 241;
    LeftUpdate(outputl);
    updateSpeed = LOW;
  break;
  case 101://only right
    Serial.println("Turn Down High Beam 11011011");
    outputr = 219;
    RightUpdate(outputr);
    outputl = 241;
    LeftUpdate(outputl);
    updateSpeed = LOW;
  break;
  case 102://only right
  Serial.println("Emergency Vehicle - Move Aside 10001111");
  outputr = 143;
  RightUpdate(outputr);
  outputl = 241;
  LeftUpdate(outputl);
  updateSpeed = LOW;
  break;
  case 103://only right
  Serial.println("Attention: Vehicle Ahead is Slowing Down 11110001");
  outputr = 241;
  RightUpdate(outputr);
  outputl = 241;
  LeftUpdate(outputl);
  updateSpeed = LOW;
  break;
  case 104://only right
  Serial.println("Oncoming Vehicle 11110111");
  outputr = 247;
  RightUpdate(outputr);
  outputl = 241;
  LeftUpdate(outputl);
  updateSpeed = LOW;
  break;
  case 111://both
  Serial.println("OFF 00000000");
  outputl = 0;
  outputr = 0;
  LeftUpdate(outputl);
  RightUpdate(outputr);
  updateSpeed = LOW;
  break;
  default://right
    Serial.println("Speed");
    carspeed = round(analogRead(potentiometer)/10.2);
    //output = carspeed | B10000001;
    SpeedConverter(carspeed);
    Serial.println(carspeed);
    updateSpeed = HIGH;
    break;
  }
}

}
