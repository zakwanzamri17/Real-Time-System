void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  xTaskCreate(
    FlameSensor
    , "FlameSensor"
    , 200
    , NULL
    , 3
    , NULL  );

   xTaskCreate(
    FireSuppression
    , "FireSupression"
    , 200
    , NULL
    , 2
    , NULL );

   xTakCreate(
    Standby
    , "FireSupressed"
    , 200
    , NULL
    , 0
    , NULL );

   vSemaphoreCreateBinary(BinarySemaphore1);
   vSemaphoreCreateBinary(BinarySemaphore2);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Supressing  Fire");
  delay(500);
}

void Standby (void *pvParameters) {
  pinMode(led_pin, OUTPUT);
  for(;;) {
    if (xSemaphoreTake(BinarySemaphore2,999999999)) {
      for (int i = 0; i < 9; i++) {
        digitalWrite(led_pin, LOW);
      }
    }
  }
}

void FireSupression (void *pvParameters) {
  pinMode(led_pin, OUTPUT);
  pinMode(alarm_pin, OUTPUT);
  pinMode(pump, OUTPUT);
  for(;;) {
    if (xSemaphoreTake(BinarySemaphore1, 9999999999)) {
      digitalWrite(alarm_pin, HIGH);
      digitalWrite(led_pin, HIGH);
      digitalWrite(pump, HIGH);
    }
  }
}

void FlameSensor (void *pvParameters) {
  for(;;) {
    pinMode(flame_sensor_pin, INPUT);
    int p = digitalRead(flame_sensor_pin);
    if (p == HIGH) {
      Serial.println("Fire!");
      xSemaphoreGliveFromISR(BinarySemaphore1, NULL);
      vTaskDelay(200);
    }
    else {
      digitalWrite(led_pin, LOW);
      digitalWrite(alarm_pin, LOW);
      digitalWrite(pump, LOW);
    }
  }
}
