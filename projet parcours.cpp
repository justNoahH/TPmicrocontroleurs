#include "mbed.h"
#include "C12832.h"
#include "FileSystemLike.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

C12832 lcd(p5, p7, p6, p8, p11);
DigitalIn startButton(p14);
DigitalIn R(p15);
DigitalIn L(p12);

struct Brique
{
    int x;
    int y;
    bool affiche;
};

void affiche_ecran(int x, int y, int w, int h, int billex, int billey, Brique allbrick[12])
{
    lcd.cls();
    for(int i = 0; i < 12; i += 1)
    {
        if(allbrick[i].affiche)
        {
            lcd.fillrect(allbrick[i].x, allbrick[i].y, allbrick[i].x + 3, allbrick[i].y + 10, -1);
        }
    }
    lcd.fillrect(x, y, x + w, y + h, -1);
    lcd.fillcircle(billex, billey, 2, -1);
}

void deplace_bille(int depl[2], int &billex, int &billey, int x, int y, bool &jouer)
{
    if(billex <= 3)
    {
        depl[0] *= -1;
    }
    if(billex >= lcd.width() - 3)
    {
        jouer = false;
    }
    if(billey >= y - 5 and billey <= y + 4 and billex >= x - 1 and billex <= x + 1)
    {
        depl[0] *= -1;
    }
    if(billey <= 3 or billey >= lcd.height() - 3)
    {
        depl[1] *= -1;
    }
    billex += depl[0];
    billey += depl[1];
}

void deplace_joueur(int &y)
{
    if(R.read() and y >= 2)
    {
        y -= 1;
    }
    if(L.read() and y <= lcd.height() - 8)
    {
        y += 1;
    }
}

void createbrick(Brique allbrick[12])
{
    int point = 0;
    for(int bx = 5; bx <= 20; bx += 5)
    {
        for(int by = 1; by <= lcd.height() - 10; by += 10)
        {
            allbrick[point].x = bx;
            allbrick[point].y = by;
            allbrick[point].affiche = true;
            point += 1;
        }
    }
}

void collide_brick(int billex, int billey, int depl[2], Brique allbrick[12], int &gagne)
{
    for(int i = 0; i < 12; i += 1)
    {
        if(billex == allbrick[i].x + 5 and billey >= allbrick[i].y and billey <= allbrick[i].y + 10 and allbrick[i].affiche)
        {
            allbrick[i].affiche = false;
            depl[0] *= -1;
            gagne += 1;
        }
        if(billey == allbrick[i].y and billex <= allbrick[i].x + 5 and billex >= allbrick[i].x and allbrick[i].affiche)
        {
            allbrick[i].affiche = false;
            depl[1] *= -1;
            gagne += 1;
        }
        if(billey == allbrick[i].y + 10 and billex <= allbrick[i].x + 5 and billex >= allbrick[i].x and allbrick[i].affiche)
        {
            allbrick[i].affiche = false;
            depl[1] *= -1;
            gagne += 1;
        }
    }
}

void update(int x, int &y, int w, int h, int &billex, int &billey, int depl[2], bool &jouer, Brique allbrick[12], int &gagne)
{
    deplace_joueur(y);
    deplace_bille(depl, billex, billey, x, y, jouer);
    collide_brick(billex, billey, depl, allbrick, gagne);
    affiche_ecran(x, y, w, h, billex, billey, allbrick);
}

void sauvegarder_score(int score)
{
    FILE* file = fopen("/local/SCORE.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "Score du joueur : %d/12\r\n", score);
        fclose(file);
    }
    else
    {
        lcd.cls();
        lcd.locate(0, 0);
        lcd.printf("Erreur sauvegarde");
    }
}

int lire_meilleur_score()
{
    FILE* file = fopen("/local/SCORE.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    int meilleur_score = 0;
    char buffer[50];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        int score;
        if (sscanf(buffer, "Score du joueur : %d/12", &score) == 1)
        {
            if (score > meilleur_score)
            {
                meilleur_score = score;
            }
        }
    }
    fclose(file);
    return meilleur_score;
}

void afficher_menu(int meilleur_score)
{
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Menu Principal");
    lcd.locate(0, 10);
    lcd.printf("1. Center pour lancer une partie");
    lcd.locate(0, 20);
    lcd.printf("2. Meilleur score: %d/12", meilleur_score);
    //lcd.locate(0, 30);
    //lcd.printf("Appuyez sur start");
}

int lancer_partie()
{
    int x = lcd.width() - 9, y = lcd.height() / 2, w = 1, h = 5;
    int depl[2] = {-1, 1}, billex = lcd.width() - 12, billey = lcd.height() / 2, gagne = 0;
    bool jouer = true;
    Brique allbrick[12];
    createbrick(allbrick);

    while (jouer)
    {
        update(x, y, w, h, billex, billey, depl, jouer, allbrick, gagne);
        if (gagne == 12)
        {
            lcd.cls();
            lcd.locate(0, 0);
            lcd.printf("Bien joue");
            sauvegarder_score(gagne);
            return 1;
        }
        wait(0.01);
    }

    sauvegarder_score(gagne);
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Game Over: Score %d", gagne);
    wait(3);
}

int main()
{
    LocalFileSystem local("local");
    while (true)
    {
        int meilleur_score = lire_meilleur_score();
        afficher_menu(meilleur_score);

        while (not startButton.read())
        {
            wait(0.1);
        }

        lancer_partie();
    }
}
