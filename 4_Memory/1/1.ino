#if CONFIG_FREERTOS_UNICORE
  static const BaseTpe_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

void testTask(void *parameter) {
  while (1) {
    int a = 1;
    int b[100];

    for (int i = 0; i < 100; i++) {
      b[i] = a + 1;
    }
    Serial.println(b[0]);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  vTaskDelay(1000 /portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS MEMORY Demo---");

  xTaskCreatePinnedToCore(testTask,
                          "Test Task",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
