#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu =0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
enum { NUM_TASKS = 5 };
enum { TASK_STACK_SIZE = 2048 };

// Globals
static SemaphoreHandle_t bin_sem;
static SemaphoreHandle_t done_sem;
static SemaphoreHandle_t chopstick[NUM_TASKS];

// Tasks
void eat(void *parameters) {

  int num;
  char buf[50];

  num = *(int *)parameters;
  xSemaphoreGive(bin_sem);

  //Take left chopstick
  xSemaphoreTake (chopstick[num], portMAX_DELAY);
  sprintf(buf, "Philosopher %i took chopstick %i", num, num);
  Serial.println(buf);

  // delay
  vTaskDelay (1 / portTICK_PERIOD_MS);

  // Take right chopstick
  xSemaphoreTake (chopstick[(num+1)%NUM_TASKS], portMAX_DELAY);
  sprintf(buf, "Philosopher %i took chopstick %i", num, (num+1)%NUM_TASKS);
  Serial.println(buf);

  // Eat
  sprintf (buf, "Philosopher %i is eating", num);
  Serial.println (buf);
  vTaskDelay (10 / portTICK_PERIOD_MS);

  // Put down right chopstick
  xSemaphoreGive (chopstick[(num+1)%NUM_TASKS]);
  sprintf (buf, "Philosopher %i returned chopstick %i", num, (num+1)%NUM_TASKS);
  Serial.println (buf);

  // Put down left chopstick
  xSemaphoreGive (chopstick[num]);
  sprintf (buf, "Philosopher %i returned chopstick %i", num, num);
  Serial.println(buf);
  vTaskDelete (NULL);
}

void setup() {

  char task_name[20];

  Serial.begin(112500);

  vTaskDelay (1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---Deadlock and Starvation---");

  // kernel objects
  bin_sem = xSemaphoreCreateBinary();
  done_sem = xSemaphoreCreateCounting (NUM_TASKS, 0);
  for (int i = 0; i < NUM_TASKS; i++) {
    chopstick[i] = xSemaphoreCreateMutex();
  }

  // Start eating
  for (int i = 0; i < NUM_TASKS; i++) {
    sprintf (task_name, "Philosopher %i", i);
    xTaskCreatePinnedToCore(eat,
                            task_name,
                            TASK_STACK_SIZE,
                            (void *)&i,
                            1,
                            NULL,
                            app_cpu);
    xSemaphoreTake (bin_sem, portMAX_DELAY);
  }

  // Wait all done
  for (int i =0; i < NUM_TASKS; i++) {
    xSemaphoreTake (done_sem, portMAX_DELAY);
  }

  // Done
  Serial.println ("Done! No deadlock occured");
}

void loop () {
  // Do nothing
}
