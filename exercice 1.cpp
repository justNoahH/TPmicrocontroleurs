#include "mbed.h"

// Déclaration des objets DigitalOut pour les 4 LEDs
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// Fonction de mise à 0 des LEDs
void ResetLeds() {
    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
}

// Fonction d'allumage des LEDs en fonction de l'entier i (0 à 15)
void DisplayIntInLeds(int i) {
    // Conversion de l'entier en binaire et affichage sur les LEDs
    led1 = (i & 0x1) ? 1 : 0;  // Bit 0
    led2 = (i & 0x2) ? 1 : 0;  // Bit 1
    led3 = (i & 0x4) ? 1 : 0;  // Bit 2
    led4 = (i & 0x8) ? 1 : 0;  // Bit 3
}

int main() {
    while (true) {
        // Boucle pour afficher les entiers de 0 à 15
        for (int i = 0; i < 16; i++) {
            DisplayIntInLeds(i); // Affichage de l'entier sur les LEDs
            wait(1); // Attente d'une seconde
            ResetLeds(); // Réinitialisation des LEDs
            wait(0.5); // Attente d'une demi-seconde avant le prochain affichage
        }
    }
}
