

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
TickType_t cs_wait = 250;
TickType_t med_wait = 5000;

//Globals
static SemaphoreHandle_t lock;

// Task Low
void doTaskL (void *parameters) {

  TickType_t timestamp;

  while (1) {
    Serial.println ("Task L trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    xSemaphoreTake(lock, portMAX_DELAY);

    Serial.print ("Task L got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println (" ms waiting for lock. Doing some work...");

    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ( (xTaskGetTickCount () * portTICK_PERIOD_MS) - timestamp < cs_wait);

    // Release lock
    Serial.println ("Task L releasing lock");
    xSemaphoreGive(lock);

    // Sleep
    vTaskDelay (500 / portTICK_PERIOD_MS);
    
  }
}

// Task M
void doTaskM(void *parameters) {

  TickType_t timestamp;

  while (1) {
    
    Serial.println ("Task M");
    timestamp = xTaskGetTickCount () * portTICK_PERIOD_MS;
    while ( (xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < med_wait);

    Serial.println ("Task M done");
    vTaskDelay (500 / portTICK_PERIOD_MS);
  }
}

// Task H
void doTaskH (void *parameters) {
  TickType_t timestamp;

  while (1) {
    
    Serial.println ("Task H trying to take lock...");
    timestamp = xTaskGetTickCount () *portTICK_PERIOD_MS;
    xSemaphoreTake (lock, portMAX_DELAY);

    Serial.print ("Task H got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println (" ms waiting for lock. Doing some work...");

    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount () * portTICK_PERIOD_MS) - timestamp < cs_wait);

    // Release lock
    Serial.println ("Task H releasing lock");
    xSemaphoreGive(lock);

    // Sleep
    vTaskDelay (500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin (112500);

  vTaskDelay (1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println ("---Priority Inversion---");

  lock = xSemaphoreCreateBinary();
  xSemaphoreGive (lock);

  xTaskCreatePinnedToCore(doTaskL,
                          "Task L",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  vTaskDelay (1 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(doTaskH,
                         "Task H",
                         1024,
                         NULL,
                         3,
                         NULL,
                         app_cpu);

  xTaskCreatePinnedToCore(doTaskM,
                         "Task M",
                         1024,
                         NULL,
                         2,
                         NULL,
                         app_cpu);

  vTaskDelete (NULL);

}

void loop() {
  // never
}
