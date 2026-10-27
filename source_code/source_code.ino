#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define DHT_PIN 7
#define DHT_TYPE DHT11
#define BUZZER 6
#define MOTOR_ENA 5
#define MOTOR_IN1 4
#define MOTOR_IN2 3
#define BLOWER_ENB 11
#define BLOWER_IN3 12
#define BLOWER_IN4 13

DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(MOTOR_ENA, OUTPUT);
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(BLOWER_ENB, OUTPUT);
    pinMode(BLOWER_IN3, OUTPUT);
    pinMode(BLOWER_IN4, OUTPUT);
    
    lcd.init();
    lcd.backlight();
    dht.begin();
    
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(BLOWER_IN3, LOW);
    digitalWrite(BLOWER_IN4, LOW);
    
    lcd.setCursor(0, 0);
    lcd.print("System Ready...");
    delay(1000);
    lcd.clear();
}

float measureDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}

void loop() {
    float distance = measureDistance();
    float humidity = dht.readHumidity();
    bool object_near1 = (distance <= 10);
    bool high_humidity = (humidity > 70);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm");
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity);
    lcd.print("%");
    delay(1000);

    if (object_near1) {
        analogWrite(MOTOR_ENA, 50);
        digitalWrite(BUZZER, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Object Near!");
        lcd.setCursor(0, 1);
        lcd.print("Speed Reduced!");
    }else {
        analogWrite(MOTOR_ENA, 100);
        digitalWrite(BUZZER, LOW);
    }

    if (high_humidity) {
        digitalWrite(BLOWER_IN3, HIGH);
        digitalWrite(BLOWER_IN4, LOW);
        analogWrite(BLOWER_ENB, 50);
        digitalWrite(BUZZER, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("High Humidity!");
        lcd.setCursor(0, 1);
        lcd.print("Blower On!");
    } else {
        digitalWrite(BLOWER_IN3, LOW);
        digitalWrite(BLOWER_IN4, LOW);
    }
    delay(1000);
}
