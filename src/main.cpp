  #include <OneWire.h>
  #include <DallasTemperature.h>
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include "GravityTDS.h"

  #define pinOxigeno A1 // Pin analógico para el sensor de oxígeno
  #define pH_PIN A0  // Pin analógico para el sensor de pH
  #define ONE_WIRE_BUS A3  // Pin analógico para el sensor de temperatura
  #define TURBIDITY_PIN A2 // Pin analógico para el sensor de turbidez
  #define IR_SENSOR A4 // Pin analógico para el sensor infrarrojo
  #define TdsSensorPin 6 // Pin digital para el sensor de TDS

  // Variables globales
  float valorOxigeno = 0;
  float mgL = 0;
  float temperature = 25, tdsValue = 0;
  float pHValue = 0; // Variable para almacenar el valor del pH

  // Variables de ajuste para el sensor de pH
  const float VREF = 5.0; // Voltaje de referencia del Arduino
  const float PH_SENSITIVITY = 0.18; // Sensibilidad del sensor (mV/pH)
  const float PH_NEUTRAL_VOLTAGE = 2.5; // Voltaje para pH 7.00 - ajusta según la documentación

  OneWire oneWire(ONE_WIRE_BUS);
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  DallasTemperature sensors(&oneWire);
  GravityTDS gravityTds;

  void setup() {
    sensors.begin();
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.backlight();
    pinMode(pinOxigeno, INPUT);
    pinMode(TURBIDITY_PIN, INPUT);
    pinMode(IR_SENSOR, INPUT);
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  // Voltaje de referencia en el ADC
    gravityTds.setAdcRange(1024);  // 1024 para ADC de 10 bits
    gravityTds.begin();  // Inicialización del sensor TDS

  }

  void mostrarOxigeno();
  void mostrarTemperatura();
  void mostrarPH();
  void mostrarTurbidez();
  void mostrarNivelAgua();
  void mostrarTDS();

  void loop() {
    mostrarOxigeno();
    mostrarTemperatura();
    mostrarPH();
    mostrarTurbidez();
    mostrarNivelAgua();
    mostrarTDS();
  }

  void mostrarOxigeno() {
    valorOxigeno = analogRead(pinOxigeno);
    mgL = valorOxigeno * (5.0 / 1023.0); // Ajusta según la calibración del sensor

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Oxigeno:");
    lcd.setCursor(0, 1);
    lcd.print(mgL, 2);
    lcd.print(" mg/L");

    Serial.print("Oxigeno: ");
    Serial.print(mgL);
    Serial.println(" mg/L");

    delay(3000);
  }

void mostrarTemperatura() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);  // Actualiza la temperatura global
  float tempF = (temperature * 9.0) / 5.0 + 32.0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.setCursor(7, 1);
  lcd.print("C");
  lcd.setCursor(9, 1);
  lcd.print(tempF);
  lcd.setCursor(15, 1);
  lcd.print("F");

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" C / ");
  Serial.print(tempF);
  Serial.println(" F");

  delay(3000);
}

void mostrarTDS() {
  gravityTds.setTemperature(temperature);  // Usa la temperatura actualizada
  gravityTds.update();
  tdsValue = gravityTds.getTdsValue();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TDS:");
  lcd.setCursor(0, 1);
  lcd.print(tdsValue, 0);
  lcd.print(" ppm");

  Serial.print("TDS (ppm): ");
  Serial.println(tdsValue);

  delay(3000);
}


  void mostrarPH() {
    int pHReading = analogRead(pH_PIN);
    float voltage = (pHReading / 1023.0) * VREF;
    pHValue = (voltage - PH_NEUTRAL_VOLTAGE) / PH_SENSITIVITY + 7.0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("pH:");
    lcd.setCursor(0, 1);
    lcd.print(pHValue, 2);

    Serial.print("pH: ");
    Serial.println(pHValue);

    delay(3000);
  }

  void mostrarTurbidez() {
    int turbidityValue = analogRead(TURBIDITY_PIN);
    float ntuValue = turbidityValue * 0.5;  // Suponiendo que cada unidad de lectura es 0.5 NTU

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turbidez:");
    lcd.setCursor(0, 1);
    lcd.print(ntuValue);
    lcd.print(" NTU");

    Serial.print("Turbidez (NTU): ");
    Serial.println(ntuValue);

    delay(3000);
  }

  void mostrarNivelAgua() {
    int irValue = analogRead(IR_SENSOR);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nivel de agua:");
    lcd.setCursor(0, 1);
    lcd.print(irValue < 800 ? "Alta        " : "Baja        ");

    Serial.print("Nivel de agua: ");
    Serial.println(irValue < 800 ? "Alta" : "Baja");

    delay(3000);
  }

