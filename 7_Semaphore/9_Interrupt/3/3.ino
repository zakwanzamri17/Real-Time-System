#if CONFIG_FREERTOS_UNICORE
 static const BaseType_t app_cpu = 0;
#else
 static const BaseType_t app_cpu = 1;
#endif

static const uint16_t timer_divider = 8;
static const uint64_t timer_max_count = 1000000;

static const int adc_pin = A0;

static hw_timer_t *timer = NULL;
static volatile uint16_t val;
static SemaphoreHandle_t bin_sem = NULL;

void IRAM_ATTR onTimer() {

  BaseType_t task_woken = pdFALSE;

  val = analogRead(adc_pin);

  xSemaphoreGiveFromISR(bin_sem, &task_woken);

  if (task_woken) {
    portYIELD_FROM_ISR();
  }
}

void printValues(void *parameters) {
  while (1) {
    xSemaphoreTale(bin_sem, portMAX_DELAY);
    Serial.println(val);
    
  }
}

void setup() {

  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---ISR Buffer---");

  bin_sem = xSemaphoreCreateBinary();

  if (bin_sem == NULL) {
    Serial.println("Could not create semaphore");
    ESP.restart();
  }

  xTaskCreatePinnedToCore(printValues,
                          "Print values",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);


  timer = timerBegin(0, timer_divider, true);

  timerAttachInterrupt(timer, &onTimer, true);

  timerAlarmWrite(timer, timer_max_count, true);

  timerAlarmEnable(timer);

  vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
