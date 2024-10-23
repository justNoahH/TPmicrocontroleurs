#include "mbed.h"
#include "C12832.h"

Serial pc(USBTX, USBRX); // tx, rx
 

#define DO  262.0
#define DOd 277.0
#define RE  294.0
#define REd 311.0
#define MI 330.0
#define FA  349.0
#define FAd 370.0
#define SOL  392.0
#define SOLd 415.0
#define LA  440.0
#define LAd 466.0
#define SI  494.0

PwmOut speaker(p26);
AnalogIn pot1(p19);
AnalogIn pot2(p20);
C12832 lcd(p5, p7, p6, p8, p11);
float frequence = 0;
int gm = 0;



void PlayNote(float frequency, float volume){

    speaker.period(1.0f /frequency);
    speaker.write(volume); 
}

float frequency(float v , int n, float frequence){
    int val=0;
    if (v<0.0) {
        val= 0;
    } else if (v >= 1) {
        val= n-1;
    } else {
        val= (int)(v*n);
    }
    for (int i=0; i<val ; i++){
      frequence*=2;
      
  }
  
  return frequence; 
}

int gamme(float v , int n, float frequence) {
    if (v<0.0) {
        return 0;
    } else if (v >= 1) {
        return n-1;
    } else {
        return (int)(v*n);
    }
    
      
  }
  


void afficher (float frequence){


    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Volume : %2f", pot1.read());
    //lcd.locate(0,10);
    //lcd.printf("Gamme : %2d", gamme(pot2.read(), 5, frequence));
    lcd.locate(0,20);
    //lcd.printf("Note : %2f",frequency(pot2.read(),5, frequence) );
    lcd.printf("Note : %2f",frequence );
}

int Chiffre() {
    char chiffrec = pc.getc();
    int chiffre = chiffrec - '0';
    int cara[7]={DO, RE, MI, FA, SOL, LA, SI};
    if (chiffre<=7 && chiffre>0) {
        return cara[chiffre-1];
    }
    return 0;
}

int Lettre(char chiffrec) {
    
    char cara[26]={'a','z','e','r','t','y','u','i','o','p','q','s','d','f','g','h','j','k','l','m','w','x','c','v','b','n'};
    int carachif[26]={DO, DOd, RE, REd, MI, FA, FAd, SOL, SOLd, LA, LAd, SI, 2*DO, 2*DOd, 2*RE, 2*REd, 2*MI, 2*FA, 2*FAd, 2*SOL, 2*SOLd, 2*LA, 2*LAd, 2*SI, 4*DO, 4*DOd};
    int note = 0;
    for(int i=0;i<26;i++){
        if( chiffrec==cara[i]){
            return carachif[i];
        }
    }     
    }
 
int gamme(char chiffrec) {
    char cara[26]={'a','z','e','r','t','y','u','i','o','p','q','s','d','f','g','h','j','k','l','m','w','x','c','v','b','n'};
    int carachif[26]={DO, DOd, RE, REd, MI, FA, FAd, SOL, SOLd, LA, LAd, SI, 2*DO, 2*DOd, 2*RE, 2*REd, 2*MI, 2*FA, 2*FAd, 2*SOL, 2*SOLd, 2*LA, 2*LAd, 2*SI, 4*DO, 4*DOd};
    for(int i=0;i<26;i++){
        if( chiffrec==cara[i]){
            return i/12+1;
        }
    }   

}

void afficheNote(char chiffrec, float v, float frequence, float gm) {
    if (chiffrec==13) {
      pc.printf("Volume : %2f", v);
      pc.printf("Gamme : %2f", gm);
      pc.printf("Note : %2f",  frequence);
    }
}

int main()
{
  
  while(1){
  
  float v= pot1.read();
  
  float v_gamme= pot2.read();

  char chiffrec = pc.getc();

  afficheNote(chiffrec, v, frequence, gm);
  gm = gamme(chiffrec);
  frequence= Lettre(chiffrec); //Chiffre();



  

 // PlayNote(frequency(pot2.read(), 5, frequence),v);
 PlayNote(frequence,v);
 

  afficher (frequence);
  wait(0.5);
  

  }

}     


