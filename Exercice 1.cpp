#include "mbed.h"



DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);


DigitalOut leds[4]= {myled1, myled2, myled3, myled4};

void ResetLeds() {
     
     for (int i =0; i <4;i++) {
         leds[i]=0;
     }

}

void DisplayIntLeds(int i){

    for (int j=0; j<4; j++){
        leds[3-j]=(i>>j)&1;
    }
}


int main() {
    ResetLeds();
  while(1) {
    for (int i=0; i<16; i++){
        DisplayIntLeds(i);
        wait(0.2);
    }
    
  }
}
