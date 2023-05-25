#include <Arduino.h>

int pinCorrente = A5;
int pinTensao = A0;

long numero_de_medicoes = 40000;
double conversaoLeituraArduino = 5.0 / 1023.0;
double conversaoCorrente = 4.25 / 19.23;
double conversaoTensao = 4.20 / 0.2;

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
  double tensaoMedia = 0;
  double picoTensao = 0;
  double P = 0;

  for (long l = 0; l < numero_de_medicoes; l++) // le 60 amostras
  {
    double correnteMedida = analogRead(pinCorrente) * conversaoLeituraArduino;
    double tensaoMedia = analogRead(pinTensao) * conversaoLeituraArduino;

    if (correnteMedida > picoCorrente)
    {
      picoCorrente = correnteMedida;
    }
    if (tensaoMedia > picoTensao)
    {
      picoTensao = tensaoMedia;
    }
    P += correnteMedida * tensaoMedia;

    correnteMedia += correnteMedida;
    tensaoMedia += tensaoMedia;
  }
  for (long l = 0; l < numero_de_medicoes; l++) {
    P += correnteMedida * tensaoMedia;
  }
  P /= numero_de_medicoes;
  correnteMedia /= numero_de_medicoes;
  tensaoMedia /= numero_de_medicoes;

  picoTensao -= tensaoMedia;
  picoCorrente -= correnteMedia;

  double S = picoCorrente * picoTensao / 2;

  Serial.flush();
  Serial.print("Potencia: ");
  Serial.println(P);
  Serial.print("Potencia Aparente: ");
  Serial.println(S);
  Serial.print("Fator de Potencia: ");
  Serial.println(P / S);
  Serial.print("Tensão média medida: ");
  Serial.println(tensaoMedia);
  Serial.print("Corrente média medida: ");
  Serial.println(correnteMedia);
  Serial.print("Tensão no circuito: ");
  Serial.println(tensaoMedia * conversaoTensao);
  Serial.print("Corrente no circuito: ");
  Serial.println(correnteMedia * conversaoCorrente);

  delay(1000);
}