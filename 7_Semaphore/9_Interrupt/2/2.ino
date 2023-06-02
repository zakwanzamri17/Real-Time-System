#if CONFIG_FREERTOS_UNICORE
 static const BaseType_t app_cpu = 0;
#else
 static const BaseType_t app_cpu = 1;
#endif

static const uint16_t timer_divider = 8;
static const uint64_t timer_max_count = 1000000;
static const TickType_t task_delay = 2000 / portTICK_PERIOD_MS;

static hw_timer_t *timer = NULL;
static volatile int isr_counter;
static portMUX_TYPE spinlock = portMUX INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {

  portENTER_CRITICAL_ISR(&spinlock);
  isr_counter++;
  portExit_CRITICAL_ISR(&spinlock);
}

void printValues(void *parameters) {
  while (1) {
    while (isr_counter > 0) {
      Serial.println(isr_counter);

      portENTER_CRITICAL(&spinlock);
      isr_counter--;
      portEXIT_CRITICAL(&spinlock);
    }
    vTaskDelay(task_delay);
  }
}

void setup() {

  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---Interrupts---");

  xTaskCreatePinnedToCore(printValues,
                          "Print values",
                          1024,
                          NULL,
                          1,
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
