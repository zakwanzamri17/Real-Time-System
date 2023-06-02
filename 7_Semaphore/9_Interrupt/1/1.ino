#if CONFIG_FREERTOS_UNICORE
 static const BaseType_t app_cpu = 0;
#else
 static const BaseType_t app_cpu = 1;
#endif

static const uint16_t timer_divider = 80;
static const uint64_t timer_max_count = 1000000;

static const int led_pin = LED_BUILTIN;

static hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {

  int pin_state = digitalRead(led_pin);
  digitalWrite(led_pin, !pin_state);
}

void setup() {

  pinMode(led_pin, OUTPUT);

  timer = timerBegin(0, timer_divider, true);

  timerAttachInterrupt(timer, &onTimer, true);

  timerAlarmWrite(timer, timer_max_count, true);

  timerAlarmEnable(timer);

}

void loop() {
  // put your main code here, to run repeatedly:

}
