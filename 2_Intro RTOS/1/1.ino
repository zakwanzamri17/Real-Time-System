// Use only core 1 for demo
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Pins
static const int led_pin = LED_BUILTIN;

//Our task blink an LED
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Configure pin
  pinMode(led_pin, OUTPUT);

  xTaskCreatePinnedToCore(
              toggleLED,
              "Toggle LED",
              1024,
              NULL,
              1,
              NULL,
              app_cpu);


}

void loop() {
  // put your main code here, to run repeatedly:

}
