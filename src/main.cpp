#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6o2XgdXA1"
#define BLYNK_TEMPLATE_NAME "anti theft"

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define pir_input 32
#define buzzer 13
#define reset_butt_1 26
#define reset_butt_2 27

char auth[] = "S05r4DRr2Yj4nDNgOYfWYx4HhNqstJxd";
char ssid[] = "VIETTEL_Y2aACxAx";
char pass[] = "Nb7YuCuc";

long buz_time_now, buz_last_time;
long alert_time_now, alert_last_time;
bool horn_sw = 0;
bool alert_sw;

void buz_active(){
 if(buz_time_now - buz_last_time >= 100){
    buz_last_time = buz_time_now;
    uint8_t pwm = (ledcRead(buzzer) == 170) ? 255 : 170;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    ledcWrite(buzzer, pwm);
 }
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin();
  ledcAttach(buzzer, 1000, 8);  
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V0, 0);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(reset_butt_1, OUTPUT);
  pinMode(reset_butt_2, INPUT_PULLUP);
  pinMode(pir_input, INPUT);
  ledcWrite(buzzer, 255); delay(100);
  ledcWrite(buzzer, 0);
}

void loop()
{
  Blynk.run();
  buz_time_now = millis();
  alert_time_now = millis();
  if(digitalRead(pir_input) == 1) horn_sw = 1;

  if(digitalRead(reset_butt_2) == 0)  horn_sw = 0;

  if(alert_sw == 1 && horn_sw == 1){
    if(alert_time_now - alert_last_time >= 8000){
      alert_last_time = alert_time_now;
      Blynk.logEvent("notify", "Cảnh báo, có người vào trong nhà");
      Serial.println("Có người vào trong nhà");
    }
    buz_active();
  }
  else ledcWrite(buzzer, 0);
}
BLYNK_WRITE(V0){
  alert_sw = param.asInt();
  horn_sw = 0;
}
 