#include <Arduino.h>

int pinCorrente = A5;
int pinTensao = A0;

long numero_de_medicoes = 40000;
double conversaoLeituraArduino = 5.0 / 1023.0;
double conversaoCorrente = 0.2 / 4.86;
double conversaoTensao = 20 / 4.84;

void setup()
{
  Serial.begin(9600);

  pinMode(pinCorrente, INPUT);
  pinMode(pinTensao, INPUT);

  Serial.println("Iniciando...");
}

void loop()
{
  double correnteMedia = 0;
  double picoCorrente = 0;
  double menorCorrente = 1000;
  double tensaoMedia = 0;
  double picoTensao = 0;
  double menorTensao = 1000;
  double P = 0;

  for (long l = 0; l < numero_de_medicoes; l++) // le 60 amostras
  {
    double correnteMedida = analogRead(pinCorrente) * conversaoLeituraArduino;
    double tensaoMedida = analogRead(pinTensao) * conversaoLeituraArduino;

    if (correnteMedida > picoCorrente)
    {
      picoCorrente = correnteMedida;
    }
    if (tensaoMedida > picoTensao)
    {
      picoTensao = tensaoMedida;
    }
    if (correnteMedida < menorCorrente)
    {
      menorCorrente = correnteMedida;
    }
    if (tensaoMedida < menorTensao)
    {
      menorTensao = tensaoMedida;
    }

    P += correnteMedida * tensaoMedida;

    correnteMedia += correnteMedida;
    tensaoMedia += tensaoMedida;
  }
  /*
  for (long l = 0; l < numero_de_medicoes; l++)
  {
    P += correnteMedia * tensaoMedia;
  } */
  P /= numero_de_medicoes;
  correnteMedia /= numero_de_medicoes;
  tensaoMedia /= numero_de_medicoes;

  //picoTensao -= tensaoMedia + menorTensao;
  //picoCorrente -= correnteMedia + menorCorrente;

  double S = picoCorrente * picoTensao / 2;

  Serial.flush();
  Serial.print("Potencia: ");
  Serial.println(P);
  Serial.print("Potencia Aparente: ");
  Serial.println(S);
  Serial.print("Fator de Potencia: ");
  Serial.println(P / S);
  Serial.print("Tensão média medida no arduino: ");
  Serial.println(tensaoMedia);
  Serial.print("Corrente média medida no arduino: ");
  Serial.println(correnteMedia);
  Serial.print("Tensão média no circuito: ");
  Serial.println(tensaoMedia * conversaoTensao);
  Serial.print("Corrente média no circuito: ");
  Serial.println(correnteMedia * conversaoCorrente);
  Serial.println("Pico de tensao");
  Serial.println(picoTensao);
  Serial.println("Pico de corrente");
  Serial.println(picoCorrente);
  Serial.println("Menor tensao");
  Serial.println(menorTensao);
  Serial.println("Menor corrente");
  Serial.println(menorCorrente);

  delay(1000);
}