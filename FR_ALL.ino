#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define ldelay 10

Adafruit_NeoPixel stripup = Adafruit_NeoPixel(8, 2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripleft = Adafruit_NeoPixel(8, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripright = Adafruit_NeoPixel(8, 13, NEO_GRB + NEO_KHZ800);


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  
  Serial.begin(115200);

  stripup.begin();
  stripup.show(); // Initialize all pixels to 'off'
  stripleft.begin();
  stripleft.show(); // Initialize all pixels to 'off'
  stripright.begin();
  stripright.show(); // Initialize all pixels to 'off'

}

int messagetype = 0;
int output = 0;

int high = 255; //Max brightness
int medium = 75; //Max brightness
int low = 0; //Min brightness


void loop() {


   messagetype = Serial.read();

   if(messagetype != -1 && messagetype != 10){
 
     Serial.print("Received: ");
     Serial.print(messagetype, DEC);
     Serial.print(" ");

     switch (messagetype){
       case 97:
         Serial.println("High beam");
         for(int n=0; n<8; n++){
           stripup.setPixelColor(n, low , low, high);
           stripleft.setPixelColor(n, low , low, high);
           stripright.setPixelColor(n, low , low, high);
         }
       break;
       case 98:
         Serial.println("Low beam");
         for(int n=0; n<8; n++){
           stripup.setPixelColor(n, low , low, low);
           stripleft.setPixelColor(n, low , low, high);
           stripright.setPixelColor(n, low , low, high);
         }
       break;
       case 99:
         Serial.println("Lower beam");
         for(int n=0; n<8; n++){
           stripup.setPixelColor(n, low , low, low);
           stripleft.setPixelColor(n, low, low, medium);
           stripright.setPixelColor(n, low, low, medium);
         }
       break;
       case 111:
         Serial.println("Off");
         for(int n=0; n<8; n++){
           stripup.setPixelColor(n, low , low, low);
           stripleft.setPixelColor(n, low , low, low);
           stripright.setPixelColor(n, low , low, low);
         }
       break;
       case 101:
         Serial.println("Oncoming Vehicle");
         output = 247;
         int i=7;
         while (output>0){
           int divisible = output%2;
           if (divisible == 0){
             stripup.setPixelColor(i, low , low, low);
             stripleft.setPixelColor(i, low , low, low);
             stripright.setPixelColor(i, low , low, low);
           }
           else if (divisible == 1){
             stripup.setPixelColor(i, low , low, high);
             stripleft.setPixelColor(i, low , low, high);
             stripright.setPixelColor(i, low , low, high);
           } 
           output=output/2;
           i--;
         }
       break;
       case 100:
         Serial.println("Emergency Vehicle");
         output = 143;
         int i=7;
         while (output>0){
           int divisible = output%2;
           if (divisible == 0){
             stripup.setPixelColor(i, low , low, low);
             stripleft.setPixelColor(i, low , low, low);
             stripright.setPixelColor(i, low , low, low);
           }
           else if (divisible == 1){
             stripup.setPixelColor(i, low , low, high);
             stripleft.setPixelColor(i, low , low, high);
             stripright.setPixelColor(i, low , low, high);
           } 
           output=output/2;
           i--;
         }
       break;
     }
    
   }
   stripup.show();
   stripleft.show();
   stripright.show();

}
