#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = LED_BUILTIN;

static SemaphoreHandle_t bin_sem;

void blinkLED(void *parameters) {

  int num = *(int *)parameters;

  xSemaphoreGive(bin_sem);

  Serial.print("Received: ");
  Serial.println(num);

  pinMode(led_pin, OUTPUT);

  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:

  long int delay_arg;

  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---Mutex Solution---");
  Serial.println("Enter number for delay (miliseconds)");

  while (Serial.available() <= 0);

  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);

  bin_sem = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(blinkLED,
                          "Blink LED",
                          1024,
                          (void *)&delay_arg,
                          1,
                          NULL,
                          app_cpu);

    xSemaphoreTake(bin_sem, portMAX_DELAY);
    
    Serial.println("Done");

}

void loop() {
  // put your main code here, to run repeatedly:

}
