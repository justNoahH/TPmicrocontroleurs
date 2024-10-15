#include "mbed.h"
#include "LM75B.h"
#include "C12832.h"

// Déclaration des objets
AnalogIn pot1(p19);
AnalogIn pot2(p20);
LM75B thermometer(p28, p27);
C12832 lcd(D11, D13, D12, D7, D10);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
PwmOut red(p21);
PwmOut green(p22);
PwmOut blue(p23);

// Tableau des patterns LED
int patterns[5] = {0, 1, 3, 7, 15};

// Fonctions déjà définies
void ResetLeds() {
    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
}

void DisplayIntInLeds(int i) {
    led1 = (i & 0x1) ? 1 : 0;
    led2 = (i & 0x2) ? 1 : 0;
    led3 = (i & 0x4) ? 1 : 0;
    led4 = (i & 0x8) ? 1 : 0;
}

int AnalogInIndex(float v, int n) {
    return (int)(v * n);
}

// Fonctions pour la LED RGB
void R(float v) {
    red.write(1.0 - v);
}

void G(float v) {
    green.write(1.0 - (1.0 - v));
}

void B(float v) {
    blue.write(1.0 - (1.0 - 0.5 * v));
}

int main() {
    while (true) {
        float po1 = pot1.read();
        float po2 = pot2.read();
        float temp = thermometer.read();
        
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Pot1: %.2f", po1);
        lcd.locate(0, 10);
        lcd.printf("Pot2: %.2f", po2);
        lcd.locate(0, 20);
        lcd.printf("Temp: %.2fC", temp);
        
        int index = AnalogInIndex(po1, 5);
        DisplayIntInLeds(patterns[index]);
        
        // Contrôle de la LED RGB avec la valeur de po2
        R(po2);
        G(po2);
        B(po2);
        
        wait(0.5);
    }
}
