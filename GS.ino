#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// Inicializa o LCD com o endereço e tamanho especificados
Adafruit_LiquidCrystal lcd(0);

// Define os pinos para cada sensor ultrassônico
const int PINO_TRIGGER_LESTE_PIN = 6;
const int PINO_ECHO_LESTE_PIN = 5;
const int PINO_TRIGGER_NORTE_PIN = 8;
const int PINO_ECHO_NORTE_PIN = 7;
const int PINO_TRIGGER_OESTE_PIN = 4;
const int PINO_ECHO_OESTE_PIN = 3;
const int PINO_TRIGGER_SUL_PIN = 2;
const int PINO_ECHO_SUL_PIN = 9;

const int MAX_DISTANCE = 320;

// Define os pinos
const int PINO_TEMPERATURA = A0;
const int PINO_SALINIDADE = A1;  
const int PINO_LDR = A2;  
const int PINO_PH = A3;  
const int PINO_ESPECTRO = A5;

void setup() {
  lcd.begin(16, 2);  // Inicializa o LCD

  // Configura os pinos dos sensores ultrassônicos como saída ou entrada
  pinMode(PINO_TRIGGER_LESTE_PIN, OUTPUT);
  pinMode(PINO_ECHO_LESTE_PIN, INPUT);
  pinMode(PINO_TRIGGER_NORTE_PIN, OUTPUT);
  pinMode(PINO_ECHO_NORTE_PIN, INPUT);
  pinMode(PINO_TRIGGER_OESTE_PIN, OUTPUT);
  pinMode(PINO_ECHO_OESTE_PIN, INPUT);
  pinMode(PINO_TRIGGER_SUL_PIN, OUTPUT);
  pinMode(PINO_ECHO_SUL_PIN, INPUT);
}

void loop() {
  // Mede a distância de cada PINO ultrassônico à boia
  unsigned int distances[4] = {
    medirDistancia(PINO_TRIGGER_LESTE_PIN, PINO_ECHO_LESTE_PIN),
    medirDistancia(PINO_TRIGGER_NORTE_PIN, PINO_ECHO_NORTE_PIN),
    medirDistancia(PINO_TRIGGER_OESTE_PIN, PINO_ECHO_OESTE_PIN),
    medirDistancia(PINO_TRIGGER_SUL_PIN, PINO_ECHO_SUL_PIN)
  };

  int minIndex = -1;
  unsigned int minValue = MAX_DISTANCE - 1;
  int countMin = 0;

  // Identifica a menor distância e conta quantas vezes ela aparece
  for (int i = 0; i < 4; i++) {
    if (distances[i] < minValue) {
      minValue = distances[i];
      minIndex = i;
      countMin = 1;
    } else if (distances[i] == minValue) {
      countMin++;
    }
  }

  // Exibe a temperatura com TMP36
  // TMP36 tem uma saída de 500mV a 0°C e aumenta 10mV/°C acima disso.
  // Fórmula: (tensão lida - 500mV) * 100 = Temperatura em °C
  // Deve-se adicionar 50 para compensar o offset de -50°C a 0V.
  float temperatura = (analogRead(PINO_TEMPERATURA) * (5.0 / 1023.0) - 0.5) * 100;

  // Exibe a salinidade usando um potenciômetro de 250k como um divisor de tensão.
  // A leitura é feita em milivolts para uma comparação relativa.
  float salinidade = analogRead(PINO_SALINIDADE) * (5.0 / 1023.0) * 1000;

  // Exibe a intensidade da luz
  int valorLDR = analogRead(PINO_LDR);
  float intensidadeLuz = valorLDR * (5.0 / 1023.0) * 100;

  // Exibe o pH
  float pH = analogRead(PINO_PH) * 14.0 / 1023.0;  
  
  // Exibe o espectro luminoso
  float espectro = mapFloat((analogRead(PINO_ESPECTRO) * (5.0 / 1023.0)), 0.0, 5.0, 380.0, 750.0);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Corrente: ");
  lcd.setCursor(0, 1);
  switch(minIndex) {
    case 0: lcd.print("Leste"); break;
    case 1: lcd.print("Norte"); break;
    case 2: lcd.print("Oeste"); break;
    case 3: lcd.print("Sul"); break;
  }
      
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura: ");
  lcd.setCursor(0, 1);
  lcd.print(temperatura, 1);
  lcd.print(" C");
  
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Salinidade: ");
  lcd.setCursor(0, 1);
  lcd.print(salinidade, 1);
  lcd.print(" ppm");  
  
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iluminacao: ");
  lcd.setCursor(0, 1);
  lcd.print(intensidadeLuz, 0);
  lcd.print(" lux");  

  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("pH: ");
  lcd.setCursor(0, 1);
  lcd.print(pH, 2); 

  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Espectro lum...: ");
  lcd.setCursor(0, 1);
  lcd.print(espectro, 2); 
  lcd.print(" nm");  

  delay(5000);
}

unsigned int medirDistancia(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  unsigned int duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

// Função para mapear valores float
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}