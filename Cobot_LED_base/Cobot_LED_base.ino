#include <Adafruit_NeoPixel.h>
#define PIN 4

double clock_speed = 16.0; //(in MHz) : value for Arduino Uno
//******************LED strip related**********************
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//variables for LED strip control
const int start_pixel = 0;
const int skip_step = 1;

//*************************LED strip related*************************


//variables for Serial communication
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int stringSize = 0;
String param = "";               // what parameter is received?
char value[10] = {'0','0','0','0','0','0','0','0','0','0'};

//variables for animation control
int wv = -1;
double D = 0.5;
int T = 1000;
int rmin = 0;
int gmin = 0;
int bmin = 0;
int rmax = 0;
int gmax = 255;
int bmax = 255;
int nSub = 2;
int wv1 = 1;
int wv2 = 1;
int wv3 = 1;
int wv4 = 1;
int wv5 = 1;
double D1 = 0.5;
double D2 = 0.5;
double D3 = 0.5;
double D4 = 0.5;
double D5 = 0.5;
int T1 = 500;
int T2 = 500;
int T3 = 500;
int T4 = 500;
int T5 = 500;
int nSeg = 2;
int r1 = 0;
int g1 = 255;
int b1 = 255;
int r2 = 0;
int g2 = 255;
int b2 = 255;
int r3 = 0;
int g3 = 255;
int b3 = 255;
double frac1 = 0.5;
double frac2 = 0.5;
double frac3 = 0;


//******Scenario specific**********************

//variables for state information
boolean path_blocked=false;
String task_type="none";
boolean waiting=false;

//variables for data exchange
int remaining_time=10;
int remaining_time_old=10;
int total_time=10;

//******End of scenario specific***************

void setup() {
  
  strip.begin();
  Serial.begin(9600);
  strip.show(); // Initialize all pixels to 'off'
  char command[7]={'0','0','0','0','0','0','0'};
  /*while(strcmp(command,"lights?")){
    Serial.print("lights");
    Serial.flush();
    if(Serial.available()){
      char c = Serial.read();
      for(int i = 1; i<7; i++)
        command[i-1] = command[i];
      command[6] = c;
      Serial.print(command);
      //Serial.print("lightslightslights");
      //Serial.flush();
    }
      
  }*/
  
  for (int i =0; i<500; i++){
    delay(1);
    Serial.print("ledbase");
  }
  
}

void loop() {  
  
  //Serial.println("debug");
  //*************************Updating animation control variables from received command*************************************
  if (stringComplete) {
    
    //Serial.println("String complete.");
    if(inputString == "lights?"){
      Serial.write("lights");  
    }
    //parse input string:
    
    
    //Serial.print("Complete string:");
    //Serial.println(inputString);
    //Serial.print("StringSize:");
    //Serial.println(stringSize);
    
    //get params
    int i=-1;
    boolean done = 0;

    //set parameters to default values before updating some of them
    wv = -1;
    D = 0.5;
    T = 1000;
    rmin = 0;
    gmin = 0;
    bmin = 0;
    rmax = 0;
    gmax = 255;
    bmax = 255;
    nSub = 2;
    wv1 = 1;
    wv2 = 1;
    wv3 = 1;
    wv4 = 1;
    wv5 = 1;
    D1 = 0.5;
    D2 = 0.5;
    D3 = 0.5;
    D4 = 0.5;
    D5 = 0.5;
    T1 = 500;
    T2 = 500;
    T3 = 500;
    T4 = 500;
    T5 = 500;
    nSeg = 2;
    r1 = 0;
    g1 = 255;
    b1 = 255;
    r2 = 0;
    g2 = 255;
    b2 = 255;
    r3 = 0;
    g3 = 255;
    b3 = 255;
    frac1 = 0.5;
    frac2 = 0.5;
    frac3 = 0;




    while(done ==0){
      
      i++;
      param = "";
      
      while(inputString[i]!='=' && inputString[i]!='\n'){
        param += inputString[i];
        i++;
      }
      
      //Serial.print(param);
      //Serial.print(":");
      i++;
      
      //get value
      int j=0;
      while(i<stringSize && inputString[i]!=' ' && inputString[i]!='\n'){
        value[j] = inputString[i];
        i++;
        j++;
      }
      value[j] = '\0';    
      if(inputString[i] == '\n') done = 1;
      
      //Serial.println(atol(value));    
      
      //end of parsing
      
      
      //set corresponding parameters to received values (the rest already set to default values)      
      
      if(param == "wv") wv = atol(value); 
      else if(param == "D") D = atof(value);
      else if(param == "T") T = atol(value);
      else if(param == "R") {//Serial.print("Updating r from "); Serial.print(r); Serial.print(" to"); Serial.println(atol(value)); 
        rmax = atol(value);}
      else if(param == "G") {//Serial.print("Updating g from "); Serial.print(g); Serial.print(" to"); Serial.println(atol(value)); 
        gmax = atol(value);}
      else if(param == "B") {//Serial.print("Updating b from "); Serial.print(b); Serial.print(" to"); Serial.println(atol(value)); 
        bmax = atol(value);}
      else if(param == "nSub") nSub = atol(value);
      else if(param == "wv1") wv1 = atol(value);
      else if(param == "wv2") wv2 = atol(value);
      else if(param == "wv3") wv3 = atol(value);
      else if(param == "wv4") wv4 = atol(value);
      else if(param == "wv5") wv5 = atol(value);
      else if(param == "D1") D1 = atof(value);
      else if(param == "D2") D2 = atof(value);
      else if(param == "D3") D3 = atof(value);
      else if(param == "D4") D4 = atof(value);
      else if(param == "D5") D5 = atof(value);
      else if(param == "T1") T1 = atol(value);
      else if(param == "T2") T2 = atol(value);
      else if(param == "T3") T3 = atol(value);
      else if(param == "T4") T4 = atol(value);
      else if(param == "T5") T5 = atol(value);
      else if(param == "r1") r1 = atol(value);
      else if(param == "g1") g1 = atol(value);
      else if(param == "b1") b1 = atol(value);
      else if(param == "r2") r2 = atol(value);
      else if(param == "b2") b2 = atol(value);
      else if(param == "g2") g2 = atol(value);
      else if(param == "r3") r3 = atol(value);
      else if(param == "g3") g3 = atol(value);
      else if(param == "b3") b3 = atol(value);
      else if(param == "frac1") frac1 = atof(value);
      else if(param == "frac2") frac2 = atof(value);
      else if(param == "frac3") frac3 = atof(value);
      //Serial.println("loop");
      
      
    }
       
    inputString = "";
    stringSize = 0;
    //param = ""; ATTENTION: modified by Kim
    //value = {'0','0','0','0','0'};
    stringComplete = false;
  }
  
  
  /*Serial.print("task_type:");
  Serial.println(task_type);
  Serial.print("waiting:");
  Serial.println(waiting);
  Serial.print("path_blocked:");
  Serial.println(path_blocked);
  Serial.print("total_time:");
  Serial.println(total_time);
  Serial.print("remaining_time:");
  Serial.println(remaining_time);*/
    
//******************End of updating animation control variables from received command*************************************  
  
  
//******************Run animations based on command***********************************************************************  
  if(param == "left_turn") {
    for(int i=(strip.numPixels()+start_pixel)/2; i<strip.numPixels(); i=i+skip_step){
       strip.setPixelColor(i,strip.Color(0,20,50));
    }
    strip.show();
    delay(1000);
    for(int i=(strip.numPixels()+start_pixel)/2; i<strip.numPixels(); i=i+skip_step){
       strip.setPixelColor(i,strip.Color(0,15,0));
    }
    strip.show();
    delay(1000);
  }

  else if(param == "right_turn") {
    for(int i=start_pixel; i<(strip.numPixels()+start_pixel)/2; i=i+skip_step){
       strip.setPixelColor(i,strip.Color(0,20,50));
    }
    strip.show();
    delay(1000);
    for(int i=start_pixel; i<(strip.numPixels()+start_pixel)/2; i=i+skip_step){
       strip.setPixelColor(i,strip.Color(0,15,0));
    }
    strip.show();
    delay(1000);    
  }

  else if(param == "straight") {
    for(int i=start_pixel; i<strip.numPixels(); i=i+skip_step){
       strip.setPixelColor(i,strip.Color(0,15,0));
    }
    strip.show();
  
  }

  else{    
    //protocol-based control    
    
    switch(wv) {
    
    case 1: //square wave
      square(D,T,rmin,gmin,bmin,rmax,gmax,bmax);
      break;
    
    case 2: //triangular wave
      triangular(D,T,rmin,gmin,bmin,rmax,gmax,bmax);
      break;

    default: //
      for(int i=start_pixel;i<strip.numPixels();i=i+skip_step){
        strip.setPixelColor(i,strip.Color(0,15,0)); 
      }
      strip.show();
      Serial.flush();
      break;
     }
   
  }
  


//******************End of run animations based on state***************************************


//At the end of every main loop iteration, poll ROS node for incoming message(s)
  readSerial();

}



//****************************ANIMATIONS*****************************************************************

//lower to upper
void progressBar1(int total_time, int remaining_time, uint32_t color1, uint32_t color2){
  for(int count = start_pixel; count<strip.numPixels(); count=count+skip_step){
     //delayMicroseconds(10);
     strip.setPixelColor(count,color1);
  }
  Serial.println("progressing");
  //strip.show();
  //Serial.print("Total:");
  //Serial.println(total_time);
  //Serial.print("Remaining:");
  //Serial.println(remaining_time);
  int numPixelsToBeOn = (total_time - remaining_time + 0.0)/total_time*(strip.numPixels()-start_pixel)/skip_step;
  //Serial.print("Pixels to be on:");
  //Serial.println(numPixelsToBeOn);
  
  
  for(int count = strip.numPixels(); count >= strip.numPixels() - numPixelsToBeOn*skip_step;  count=count-skip_step){
    //delayMicroseconds(10);
    strip.setPixelColor(count,color2);
  }
  strip.show();
}
/*
//upper to lower
void progressBar2(int total_time, int remaining_time){
  for(int count = start_pixel; count<strip.numPixels(); count=count+skip_step){yt
     //delayMicroseconds(10);
     strip.setPixelColor(count,strip.Color(0,20,0));
  }
  //strip.show();
  
  //Serial.print("Total:");
  //Serial.println(total_time);
  //Serial.print("Remaining:");
  //Serial.println(remaining_time);
  int numPixelsToBeOn = (total_time - remaining_time + 0.0)/total_time*(strip.numPixels()-start_pixel)/skip_step;
  //Serial.print("Pixels to be on:");
  //Serial.println(numPixelsToBeOn);
  
  
  for(int count = start_pixel; count<start_pixel + numPixelsToBeOn*skip_step; count=count+skip_step){
    //delayMicroseconds(10);
    strip.setPixelColor(count,strip.Color(0,0,100));
  }
  strip.show();
}

//Blinking
void progressBar3(int total_time, int remaining_time, double d){
  
  progressBar1(total_time,remaining_time);
  delay(d);
  progressBar1(total_time,total_time);
  delay(d);
}


void progressBar4(int total_time, int remaining_time, double d){
  
  progressBar2(total_time,remaining_time);
  delay(d);
  progressBar2(total_time,total_time);
  delay(d);
}

void progressBar5(int total_time, int remaining_time,double d){  
  //int g_st = int((remaining_time_old+0.0)/total_time * 100);
  ///int b_st = int((total_time - remaining_time_old+0.0)/total_time * 100);
  double g_end = (remaining_time+0.0)/total_time * 100;
  double b_end = (total_time + 0.0 - remaining_time)/total_time * 100; 
  /*
  for(int i=0; i<d; i=i++){
    delay(100);
    for(int j=start_pixel; j<strip.numPixels(); j=j+skip_step){
      strip.setPixelColor(j,strip.Color(0,g_st+(g_end-g_st)*i/d,b_st+(b_end-b_st)*i/d));
    }
    strip.show();
  }
  
  //normal_blink(0.5,500,500);
  
  for(int j=start_pixel; j<strip.numPixels(); j=j+skip_step){
      strip.setPixelColor(j,strip.Color(0,g_end,b_end));
  }
  strip.show();
  //delay(500);
  //normal_blink(0.5,500,500);
}

*/
void square(double on_duty_cycle, double period, int rmin, int gmin, int bmin, int rmax, int gmax, int bmax){
   for(int i=start_pixel; i<strip.numPixels(); i=i+skip_step){
      strip.setPixelColor(i,strip.Color(rmax,gmax,bmax));
   }
   strip.show();
   delay(on_duty_cycle*period);
   
   for(int i=start_pixel; i<strip.numPixels(); i=i+skip_step){
      strip.setPixelColor(i,strip.Color(rmin,gmin,bmin));
   }
   strip.show();
   delay((1-on_duty_cycle)*period);
}

void triangular(double duty_cycle, double period, int rmin, int gmin, int bmin, int rmax, int gmax, int bmax){
  double correction_factor=17;
  period = period/correction_factor;
  //fade in
  double first_cycle = duty_cycle*period;
  double second_cycle = period*(1-duty_cycle);
  for(int i=0; i<=first_cycle; i++){
    for(int j=start_pixel; j<strip.numPixels(); j=j+skip_step){
      strip.setPixelColor(j,strip.Color(rmin + (rmax-rmin)*(i+0.0)/first_cycle,gmin + (gmax-gmin)*(i+0.0)/first_cycle,bmin + (bmax-bmin)*(i+0.0)/first_cycle));
    }
    strip.show();
  }
  //fade out
  for(int i=0; i<=second_cycle; i++){
    for(int j=start_pixel; j<strip.numPixels(); j=j+skip_step){
      strip.setPixelColor(j,strip.Color((rmax-rmin)*(second_cycle-i)/second_cycle+rmin,(gmax-gmin)*(second_cycle-i)/second_cycle+gmin,(bmax-bmin)*(second_cycle-i)/second_cycle+bmin));
    }
    strip.show();
  }
    clearStrip();
}


void push(double period, uint32_t r, uint32_t g, uint32_t b){
  //fade in
  for(int i=0; i<period/2.0; i=i++){
    for(int j=start_pixel; j<strip.numPixels(); j=j+skip_step){
      strip.setPixelColor(j,strip.Color(r/4 + 3*r/4*i/period,g/4+3*g/4*i/period,b/4+3*b/4*i/period));
    }
  }

}


void colorWipe(uint32_t c, uint8_t wait) {
  clearStrip();
  for(uint16_t i=start_pixel; i<strip.numPixels(); i=i+skip_step) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
    
  for(uint16_t i=start_pixel; i<strip.numPixels(); i=i+skip_step) {
      strip.setPixelColor(i, 0);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<100; j++) {
    for(i=start_pixel; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color((j+i) * 3, 255 - (j+i) * 3,0));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=start_pixel; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<50; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=start_pixel; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=start_pixel; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=start_pixel; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=start_pixel; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void clearStrip(){
  
  for(int i=0;i<strip.numPixels();i++){
    strip.setPixelColor(i,strip.Color(0,0,0)); 
  }
  strip.show();
  
}


//****************************END OF ANIMATIONS*****************************************************************


void readSerial(){
  
    Serial.write("Ready to listen\n");
    delay(100);
    if(!Serial.available()) return;
    while (stringComplete == false) {
      //Serial.println("CurrentString:");
      //Serial.println(inputString);
      if(Serial.available()){
        // get the new byte:
        char inChar = (char)Serial.read(); 
        // add it to the inputString:
        inputString += inChar;
        stringSize++;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
          stringComplete = true;
          //Serial.write("Message received\n");
          //Serial.print("Received across this noble USB medium:");
          //Serial.println(inputString);
          //Serial.print("Size:");
          //Serial.println(stringSize);
        }
     } 
   }
}

