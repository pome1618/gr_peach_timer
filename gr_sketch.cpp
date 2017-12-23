/* GR-PEACH Sketch Template V2.02 */
#include <Arduino.h>
#include <MsTimer2.h>

#define LONG_CLICK 1000000

typedef struct{
	int h;
	int m;
	int s;
} Timer;

typedef enum {
	Normal,
	Edit_h,
	Edit_m
} State;

Timer timer;
State state;

void timer_update(){
	int c;
	c=timer.s/60;
	timer.s=timer.s%60;
	timer.m=timer.m+c;
	c=timer.m/60;
	timer.m=timer.m%60;
	timer.h=(timer.h+c)%12;
}

void out_update(){
	digitalWrite(PIN_IO36  , !(timer.h&0x8));
	digitalWrite(PIN_IO37  , !(timer.h&0x4));
	digitalWrite(PIN_IO38  , !(timer.h&0x2));
	digitalWrite(PIN_IO39  , !(timer.h&0x1));
	digitalWrite(PIN_IO40  , !(timer.m&0x20));
	digitalWrite(PIN_IO41  , !(timer.m&0x10));
	digitalWrite(PIN_IO42  , !(timer.m&0x8));
	digitalWrite(PIN_IO43  , !(timer.m&0x4));
	digitalWrite(PIN_IO32  , !(timer.m&0x2));
	digitalWrite(PIN_IO33  , !(timer.m&0x1));
}

void sec_event(){
	if(state==Normal){
		timer.s=timer.s+1;
	}
	timer_update();
	out_update();
}
void setup()
{
	timer.h=0;
	timer.m=0;
	timer.s=0;
	state=Normal;
	pinMode(PIN_SW        , INPUT);
	pinMode(PIN_LED_RED   , OUTPUT);
	pinMode(PIN_LED_GREEN , OUTPUT);
	pinMode(PIN_LED_BLUE  , OUTPUT);
	pinMode(PIN_LED_USER  , OUTPUT);
	pinMode(PIN_IO36      , OUTPUT);
	pinMode(PIN_IO37      , OUTPUT);
	pinMode(PIN_IO38      , OUTPUT);
	pinMode(PIN_IO39      , OUTPUT);
	pinMode(PIN_IO40      , OUTPUT);
	pinMode(PIN_IO41      , OUTPUT);
	pinMode(PIN_IO42      , OUTPUT);
	pinMode(PIN_IO43      , OUTPUT);
	pinMode(PIN_IO32      , OUTPUT);
	pinMode(PIN_IO33      , OUTPUT);
	MsTimer2::set(1000, sec_event);
	MsTimer2::start();
}

void loop()
{
	while(1){
		volatile long i;
		switch(state){
			case Normal :
		        digitalWrite(PIN_LED_RED, 1);
				if(digitalRead(PIN_SW) == 0){
					for(i=0;i<LONG_CLICK;i++){
						if(digitalRead(PIN_SW) == 1){
							break;
						}
					}
			        digitalWrite(PIN_LED_USER, 1);
					while(digitalRead(PIN_SW) == 0);
			        digitalWrite(PIN_LED_USER, 0);
					if(i == LONG_CLICK){
						state=Edit_h;
					}
				}
		        digitalWrite(PIN_LED_RED, 0);
				break;
			case Edit_h:
		        digitalWrite(PIN_LED_GREEN, 1);
				while(1){
					if(digitalRead(PIN_SW) == 0){
						for(i=0;i<LONG_CLICK;i++){
							if(digitalRead(PIN_SW) == 1){
								break;
							}
						}
				        digitalWrite(PIN_LED_USER, 1);
						while(digitalRead(PIN_SW) == 0);
				        digitalWrite(PIN_LED_USER, 0);
						if(i == LONG_CLICK){
							state=Edit_m;
							timer_update();
							out_update();
							break;
						}else{
							timer.h++;
						}
					}
				}
		        digitalWrite(PIN_LED_GREEN, 0);
				break;
			case Edit_m:
		        digitalWrite(PIN_LED_BLUE, 1);
				while(1){
					if(digitalRead(PIN_SW) == 0){
						for(i=0;i<LONG_CLICK;i++){
							if(digitalRead(PIN_SW) == 1){
								break;
							}
						}
				        digitalWrite(PIN_LED_USER, 1);
						while(digitalRead(PIN_SW) == 0);
				        digitalWrite(PIN_LED_USER, 0);
						if(i == LONG_CLICK){
							state=Normal;
							timer_update();
							out_update();
							break;
						}else{
							timer.m++;
						}
					}
				}
		        digitalWrite(PIN_LED_BLUE, 0);
				break;
			default:
				state=Normal;
				break;
		}
	}
}


