#if CONFIG_FREERTOS_UNICORE
 static const BaseType_t app_cpu = 0;
#else
 static const BaseType_t app_cpu = 1;
#endif

static int shared_var = 0;
static SemaphoreHandle_t mutex;

void incTask(void *parameters) {

  int local_var;

  //Loop forever
  while (1) {

    if (xSemaphoreTake(mutex, 0) == pdTRUE){
    
    local_var = shared_var;
    local_var++;
    vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
    shared_var = local_var;

    xSemaphoreGive(mutex);

    Serial.println(shared_var);
  } else {
    
  }
 }
}

void setup() {
  // put your setup code here, to run once:

  randomSeed(analogRead(0));

  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Race---");

  mutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(incTask,
      "Increment Task 1",
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
