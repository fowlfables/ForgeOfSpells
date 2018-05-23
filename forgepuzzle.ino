//https://github.com/adafruit/Adafruit_MPR121/blob/master/Adafruit_MPR121.h

#include <Wire.h>
#include "Adafruit_MPR121.h"

 
Adafruit_MPR121 cap = Adafruit_MPR121();
 
int dragon = 0;              //correct inner rune detected
int friendw = 0;              //correct outer rune detected
#define booksensor 12          //magnet sensor*/
unsigned long prevMillis;
unsigned long prevColorMillis;
unsigned long currentMillis;
unsigned long flamebrightness = 100;
int flametime = 100;         //millis at current color/brightness
int color = 0;               //rainbow color*/
int red = 11;	 //red LED pin
int green = 10;               //green LED pin
int blue = 9;	 //blue LED pin
int rvalue = 0;
int gvalue = 0;
int bvalue = 0;
int colorTime = 750; //amount of time for each color
 
void setup(){
	cap.begin(0x5A);              // Start the MPR121 chip
	pinMode(red, OUTPUT);
	pinMode(green, OUTPUT);
	pinMode(blue, OUTPUT);
	pinMode(booksensor, INPUT);
	digitalWrite(red, HIGH);
	digitalWrite(green, HIGH);
	digitalWrite(blue, HIGH);
}
 
void loop(){
	flicker();
	for (int i=0; i<12; i++) {	 // Go through each of the 12 sensors
		if (cap.touched() & _BV(i)) {    // Check if the sensor is touched
			if(i==0)
			{
				dragon=1;
				blueflame();
				delay(50);
			}
			else if(i==1)
			{
				friendw=1;
				blueflame();
				delay(50);
			}
			else if(i<=6)
			{
				dragon=0;
				blueflame();
				delay(50);
			}
			else
			{
				friendw=0;
				blueflame();
				delay(50);
			}
		}
	}
 
	if (dragon && friendw)
	{
		if (digitalRead(booksensor)==LOW)
		{
			rainbowflame();
		}else{
			blueflame();
		}
	}else{
		greenflame();
	}
}
 
void flicker(){
	currentMillis = millis();
	if (currentMillis - prevMillis >= flametime)
	{
		prevMillis = currentMillis;
		flamebrightness = random(0,150);
		flametime = random(100,200);
		//color = random(0,5);
	}
}
 
void blueflame(){
	analogWrite(red,255);
	analogWrite(green,255);
	analogWrite(blue,flamebrightness);
}
 
void greenflame(){
	analogWrite(red,255);
	analogWrite(green,flamebrightness);
	analogWrite(blue,255);
}
 
void rainbowflame(){
	//first color section
	if((currentMillis - prevColorMillis) <= colorTime){    
		gvalue = map((currentMillis - prevColorMillis),0,colorTime,0,255);
		rvalue = map((prevColorMillis - currentMillis),0,-colorTime,255,0);
	//second color section
	} else if((currentMillis - prevColorMillis) <= (2 * colorTime)){
		bvalue = map((currentMillis - prevColorMillis),colorTime,(2*colorTime),0,255);
		gvalue = map((prevColorMillis - currentMillis),-colorTime,-(2*colorTime),255,0);
	//third color section
	} else if((currentMillis - prevColorMillis) <= (3 * colorTime)){
		rvalue = map((currentMillis - prevColorMillis),(2*colorTime),(3*colorTime),0,255);
		bvalue = map((prevColorMillis - currentMillis),-(2*colorTime),-(3*colorTime),255,0);
	} else {		    //start over
		prevColorMillis = currentMillis;           //reset counter to 0
	}

	analogWrite(red,rvalue);
	analogWrite(green,gvalue);
	analogWrite(blue,bvalue);
}
