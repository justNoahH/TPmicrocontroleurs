#include "mbed.h"
#include "C12832.h"


DigitalIn U(p15);
DigitalIn D(p12);
DigitalIn L(p13);
DigitalIn R(p16);
DigitalIn X(p14);
Timer t;


DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

C12832 lcd(p5, p7, p6, p8, p11);
DigitalOut leds[4]={led1, led2, led3, led4};
int sens = 0;


int main() {
    lcd.cls();

    while (true) {
        if (R) {
            sens = 0;  
        } else if (L) {
            sens = 1;  
        }

       
        lcd.locate(0, 0);
        if (sens == 0) {
            lcd.printf("R ");
        } else {
            lcd.printf("L ");
        }

        wait(0.1);
    }
}