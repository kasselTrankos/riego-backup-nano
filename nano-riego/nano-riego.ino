#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "RTClib.h"
// RTC_DS1307 rtc;
RTC_DS3231 rtc;
String irrigation = "";
String next ="";
String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String monthsNames[12] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre" };

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Inicia el LCD en la dirección 0x27, con 16 caracteres y 2 líneas

void setup()
{
  Serial.begin(9600);
   pinMode(A0, OUTPUT);
   if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
   }
   // Si se ha perdido la corriente, fijar fecha y hora
   /*if (rtc.lostPower()) {
      // Fijar a fecha y hora de compilacion
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      
      // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
      // rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
   }*/
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   lcd.begin();                      
   lcd.backlight();
   
 }
void irrigate(DateTime date) {
  
  if(date.hour() == 9 && date.minute() == 6 && date.second() >=0 && date.second()<= 55) {
    next= " ->15:12";
    irrigation = " 40s";
    digitalWrite(A0, HIGH);
  } else if(date.hour() == 15 && date.minute() == 12 && date.second() >=0 && date.second()<=40) {
    next= " ->21:00";
    irrigation = " 55s";
    Serial.println("ON");
    digitalWrite(A0, HIGH);
  } else if(date.hour() == 21 && date.minute() == 0 && date.second() >=0 && date.second()<=55) {
    next= " ->09:06";
    irrigation = " 55s";
    Serial.println("ON");
    digitalWrite(A0, HIGH);
  } else {
    irrigation = "";
    digitalWrite(A0, LOW);
  }
}

void printDate(DateTime date)
{
  
   lcd.clear();
   lcd.setCursor(0, 0);
   String dd = String(date.year(), DEC) + "/" + String(date.month(), DEC) +"/"+ String(date.day(), DEC) + irrigation;
   
   lcd.print(dd);
   String hh = String(date.hour(), DEC) + ":"+ String(date.minute(), DEC) + ":"+ String(date.second(), DEC) + next;
   
   lcd.setCursor(0, 1);
   lcd.print(hh);
}

void loop() {
   // Obtener fecha actual y mostrar por Serial
   DateTime now = rtc.now();
   irrigate(now);
   printDate(now);
   delay(1000);
}
