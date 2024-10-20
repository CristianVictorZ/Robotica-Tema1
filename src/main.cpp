#include <Arduino.h>

#define BAUD 28800

//Pini pentru LED-ul RGB
#define pinLedRosu 5
#define pinLedVerde 6

//Pini pentru cele 2 butoane
#define pinButtonStart 2
#define pinButtonStop 3

//Pini pentru LED-urile de incarcare
#define pinLedAlb1 8
#define pinLedAlb2 9
#define pinLedAlb3 10
#define pinLedAlb4 11

//Vector care memoreaza pinii LED-urilor pentru incarcare; folosit pentru clipirea acestor LED-uri
int vPiniAlbi[4] = {pinLedAlb1, pinLedAlb2, pinLedAlb3, pinLedAlb4};

//Variabile necesare pentru citirea starii butoanelor
int read = 0;
int buttonState = 0;
int lastButtonState = 0;

//Variabile necesare pentru debouncing
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 

//Variabile necesare pentru verificarea starii programului
bool pornit = false;
bool citire = true;
bool oprireFortata = false;

//Variabile necesare pentru verificarea timpului de functionare si finalizare
unsigned long timpActivare = 0;
unsigned long durataFunctionare = 12000;
unsigned long timpFinalizare = 2500;

//Variabile necesare pentru clipirea LED-urilor
unsigned long timpBlink = 300;
unsigned long ultimulBlink = 0;

//Variabile necesare pentru oprirea fortata a sistemului
unsigned long timpOprire = 1000;
unsigned long timpIncepereOprire = 0;

//Variabile necesare pentru aprinderea si clipirea LED-urilor de incarcare
int numarBlinkFinal = 0;
bool stareLed = false;

//Functie pentru clipirea LED-urilor de incarcare
//ledCurent = led-ul care clipeste
//ledCurent < 4 => clipeste doar un singur LED
//ledCurent >= 4 => clipesc toate LED-urile de 3 ori
void blinkIncarcare(int ledCurent)
{
  if((millis() - ultimulBlink) > timpBlink)
  {
    ultimulBlink = millis();
    if(ledCurent < 4)
    {
      digitalWrite(vPiniAlbi[ledCurent],stareLed);
    }else if(numarBlinkFinal < 6)
    {
      for(int i = 0; i<4; i++)
        digitalWrite(vPiniAlbi[i],stareLed);
      numarBlinkFinal++;
    }
    stareLed = !stareLed;
  }
}

bool citesteButon(unsigned long timpDeApasare)
{
  //Verifica daca a fost apasat butonul de pornire (+debouncing)
  if (lastButtonState != read) 
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > timpDeApasare) 
  {
    if (read != buttonState) 
    {
      buttonState = read;
      if (buttonState == HIGH) 
        return true;
    }
  }

  return false;
}

int calculIncarcare()
{
  //Salveaza ca procent cat de mult din incarcare s-a realizat
  int procent = (millis() - timpActivare) * 100 / durataFunctionare;

  int ledCurent = floor(procent/25);  //LED-ul curent care clipeste

  //Daca nu sunt toate LED-urile aprinse le aprinde doar pe cele pana la LED-ul care clipeste
  if(ledCurent < 4)
  {
    for(int i = 0; i<min(ledCurent, 4); i++)
      digitalWrite(vPiniAlbi[i],HIGH);
  }

  return ledCurent;
}

//Functie pentru setarea culorii LED-ului RGB
void setRGB(int valRosu, int valVerde) 
{
  digitalWrite(pinLedRosu, valRosu);
  digitalWrite(pinLedVerde, valVerde);
}

void setup() 
{
  Serial.begin(BAUD);
  //LED-ul RGB
  pinMode(pinLedRosu, OUTPUT);              
  pinMode(pinLedVerde, OUTPUT);

  //Butoanele pentru pornire si oprire
  pinMode(pinButtonStart, INPUT);
  pinMode(pinButtonStop, INPUT);

  //LED-urile care corespund procentului de incarcare
  pinMode(pinLedAlb1, OUTPUT);
  pinMode(pinLedAlb2, OUTPUT);
  pinMode(pinLedAlb3, OUTPUT);
  pinMode(pinLedAlb4, OUTPUT);

  Serial.println("Setup complet");
}

void  loop() 
{

  //Verifica daca poate citi din nou (timpul de functionare si finalizare a trecut)
  if((millis() - timpActivare) > (durataFunctionare + timpFinalizare))
    citire = true;
  
  //Verifica daca citeste starea butonului de pornire
  if(citire)
  {
    pornit = false;
    oprireFortata = false;

    //LED-urile albe sunt stinse
    for(int i = 0; i<4; i++)
      digitalWrite(vPiniAlbi[i],LOW);
    
    //Citeste starea butonului de pornire
    read = digitalRead(pinButtonStart);

    if (citesteButon(debounceDelay)) {
      timpActivare = millis();
      pornit = true;
      citire = false;
      numarBlinkFinal = 0;  //Necesar pentru animatia de final
    }

    //Salveaza starea anterioara a butonului de pornire
    lastButtonState = read;
  }

  //Verifica daca statia e pornita
  if(pornit)
  {

    //Citeste starea butonului de oprire
    read = digitalRead(pinButtonStop);

    //Daca a fost apasat suficient de mult, statia incepe sa se opreasca
    if(citesteButon(timpOprire))
    {
      timpIncepereOprire = millis();
      oprireFortata = true;
      stareLed = false;  //Necesar pentru animatia de final
    }

    if(!oprireFortata)
    {
      //Memoreaza ultima stare a butonului de oprire
      lastButtonState = read;

      //Apelare functie pentru clipire
      blinkIncarcare(calculIncarcare());
    }else
    {
      //Daca a fost oprit de la buton, se apeleaza functia de cliprie cu valoarea 4 = toate LED-urile clipesc de 3 ori
      blinkIncarcare(4);

      //Daca timpul de la inceperea opririi este mai mare decat timpul de finalizare (expira timpul de finalizare),
      //incepe din nou sa citeasca starea butonului de pornire
      if((millis() - timpIncepereOprire) > timpFinalizare)
      {
        pornit = false;
        citire = true;
      }
    }

    //LED-ul RGB se face rosu
    setRGB(HIGH, LOW);
  }else
  {
    //Daca nu, LED-ul RGB se face verde
    setRGB(LOW, HIGH);
  }
}
