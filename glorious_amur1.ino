#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o LCD com o endereço e tamanho especificados
LiquidCrystal_I2C lcd(0x20, 16, 2);  // Endereço I2C ajustado para 0x20

// Define os pinos para cada sensor ultrassônico
const int SENSOR_TRIGGER_LESTE_PIN = 6;
const int SENSOR_ECHO_LESTE_PIN = 5;
const int SENSOR_TRIGGER_NORTE_PIN = 8;
const int SENSOR_ECHO_NORTE_PIN = 7;
const int SENSOR_TRIGGER_OESTE_PIN = 4;
const int SENSOR_ECHO_OESTE_PIN = 3;
const int SENSOR_TRIGGER_SUL_PIN = 2;
const int SENSOR_ECHO_SUL_PIN = 9;

const int MAX_DISTANCE = 320;

// Define os pinos onde estão conectados os sensores de temperatura e salinidade
const int SENSOR_TEMPERATURA = A0;
const int SENSOR_SALINIDADE = A1;  
const int PINO_LDR = A2;  // Pino para o sensor LDR

void setup() {
  lcd.init();  // Inicializa o LCD
  lcd.backlight();  // Liga o backlight do LCD
  lcd.display();

  // Configura os pinos dos sensores ultrassônicos como saída ou entrada
  pinMode(SENSOR_TRIGGER_LESTE_PIN, OUTPUT);
  pinMode(SENSOR_ECHO_LESTE_PIN, INPUT);
  pinMode(SENSOR_TRIGGER_NORTE_PIN, OUTPUT);
  pinMode(SENSOR_ECHO_NORTE_PIN, INPUT);
  pinMode(SENSOR_TRIGGER_OESTE_PIN, OUTPUT);
  pinMode(SENSOR_ECHO_OESTE_PIN, INPUT);
  pinMode(SENSOR_TRIGGER_SUL_PIN, OUTPUT);
  pinMode(SENSOR_ECHO_SUL_PIN, INPUT); 
  
  pinMode(PINO_LDR, INPUT);
}

void loop() {
  // Mede a distância de cada sensor ultrassônico à boia
  unsigned int distances[4] = {
    medirDistancia(SENSOR_TRIGGER_LESTE_PIN, SENSOR_ECHO_LESTE_PIN),
    medirDistancia(SENSOR_TRIGGER_NORTE_PIN, SENSOR_ECHO_NORTE_PIN),
    medirDistancia(SENSOR_TRIGGER_OESTE_PIN, SENSOR_ECHO_OESTE_PIN),
    medirDistancia(SENSOR_TRIGGER_SUL_PIN, SENSOR_ECHO_SUL_PIN)
  };

  int minIndex = -1;
  unsigned int minValue = MAX_DISTANCE + 1;
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

  // Exibe a temperatura
  float temperature = (analogRead(SENSOR_TEMPERATURA) * (5.0 / 1023.0) - 0.5) * 100;
  // Exibe a salinidade
  float salinity = analogRead(SENSOR_SALINIDADE) * (5.0 / 1023.0) * 1000;
  // Exibe a intensidade da luz
  int valorLDR = analogRead(PINO_LDR);
  float intensidadeLuz = valorLDR * (5.0 / 1023.0) * 100;

  lcd.clear();
  lcd.setCursor(0, 0);

  if (minValue == 0 && countMin > 1) {
    lcd.print("Sentido: Parada");
  } else {
    lcd.print("Sentido: ");
    switch(minIndex) {
      case 0: lcd.print("Leste"); break;
      case 1: lcd.print("Norte"); break;
      case 2: lcd.print("Oeste"); break;
      case 3: lcd.print("Sul"); break;
      default: lcd.print("Indefinido"); break;
    }
  }
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Sal: ");
  lcd.print(salinity, 1);
  lcd.print(" ppm");  
  
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luz: ");
  lcd.print(intensidadeLuz, 0);

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
