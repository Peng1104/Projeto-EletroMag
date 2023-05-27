#include <Arduino.h>

int pinCorrente = A5;
int pinTensao = A0;

long numero_de_medicoes = 40000;
double conversaoLeituraArduino = 5.0 / 1023.0;
double conversaoCorrente = 0.2 / 4.75;
double conversaoTensao = 20 / 4.71;
int razaoTransformador = 2;

void setup()
{
  Serial.begin(9600);

  pinMode(pinCorrente, INPUT);
  pinMode(pinTensao, INPUT);

  Serial.println("Iniciando...");
}

void loop()
{
  double tensaoMedida = 0;
  double correnteMedida = 0;

  double tensaoMedia = 0;
  double correnteMedia = 0;

  double picoTensao = 0;
  double picoCorrente = 0;

  double P = 0;
  double S = 0;

  for (long l = 0; l < numero_de_medicoes; l++)
  {
    tensaoMedida = analogRead(pinTensao) * conversaoLeituraArduino;
    correnteMedida = analogRead(pinCorrente) * conversaoLeituraArduino;

    if (tensaoMedida > picoTensao)
    {
      picoTensao = tensaoMedida;
    }
    if (correnteMedida > picoCorrente)
    {
      picoCorrente = correnteMedida;
    }

    tensaoMedia += tensaoMedida;
    correnteMedia += correnteMedida;

    P += tensaoMedida * correnteMedida;
  }

  tensaoMedia /= numero_de_medicoes;
  correnteMedia /= numero_de_medicoes;

  picoTensao -= tensaoMedia;
  picoCorrente -= correnteMedia;

  picoTensao *= conversaoTensao * razaoTransformador;
  picoCorrente *= conversaoCorrente * razaoTransformador;

  P /= numero_de_medicoes;
  P *= conversaoTensao * conversaoCorrente;

  S = picoCorrente * picoTensao / 2;

  Serial.flush();
  Serial.print("Potencia real média: ");
  Serial.println(P);
  Serial.print("Fator de Potencia: ");
  Serial.println(P / S);
  Serial.print("Offset do Amp op da tensao: ");
  Serial.println(tensaoMedia);
  Serial.print("Offset do Amp op da corrente: ");
  Serial.println(correnteMedia);
  Serial.print("\n");

  delay(1000);
}