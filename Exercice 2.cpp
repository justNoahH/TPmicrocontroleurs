#include "mbed.h"
#include "LM75B.h"
#include "C12832.h"



DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);
AnalogIn pot1(p19);
AnalogIn pot2(p20);
LM75B thermo(p28, p27);
C12832 lcd(p5, p7, p6, p8, p11);
PwmOut rouge(p23);
PwmOut vert(p24);
PwmOut bleu(p25);
int patterns[5]= {0,1,3,7,15};


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

void afficher (){


    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Po1 : %2f", pot1.read());
    lcd.locate(0,10);
    lcd.printf("Po2 : %2f", pot2.read());
    lcd.locate(0,20);
    lcd.printf("Temp : %2f", thermo.read());
}

int AnalogIn2Index(float v , int n ){
    if (v<0.0) {
        return 0;
    } else if (v >= 1) {
        return n-1;
    } else {
        return (int)(v*n);
    }
}

void R(float v){
        rouge.write(1/(0.5-v));
}

void B(float v){
    bleu.write(1/(-0.5+v));
}


void V(float v){
    if (v<0.5) {
        vert.write(1/v);
    }
    else {
        vert.write(1/(0.5-(v-0.5)));
    }
}

int main() {
    ResetLeds();
    while(1){
        afficher();
        int index = AnalogIn2Index(pot1.read(), 5);
        DisplayIntLeds(patterns[index]);
        wait(1); 

        R(pot2.read());
        B(pot2.read());
        V(pot2.read());

    }
}





