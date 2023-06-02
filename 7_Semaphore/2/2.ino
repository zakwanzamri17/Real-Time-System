#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static const int num_tasks = 5;

typedef struct Message {
  char body [20];
  uint8_t len;
} Message;

static SemaphoreHandle_t sem_params;

void myTask(void *parameters) {

  Message msg = *(Message *)parameters;

  xSemaphoreGive(sem_params);

  Serial.print("Received: ");
  Serial.print(msg.body);
  Serial.print(" | len: ");
  Serial.println(msg.len);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL);

}

void setup() {
  // put your setup code here, to run once:

  char task_name[12];
  Message msg;
  char text[20] = "All your base";

  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---Counting Semaphore---");

  sem_params = xSemaphoreCreateCounting(num_tasks, 0);

  strcpy(msg.body, text);
  msg.len = strlen(text);

  for (int i = 0; i < num_tasks; i++) {
    sprintf(task_name, "Task %i", i);

    xTaskCreatePinnedToCore(myTask,
                            task_name,
                            1024,
                            (void *)&msg,
                            1,
                            NULL,
                            app_cpu);
  }

  for (int i = 0; i < num_tasks; i++) {
    xSemaphoreTake(sem_params,portMAX_DELAY);
  }

  Serial.println("All tasks created");

}

void loop() {
  // put your main code here, to run repeatedly:

  vTaskDelay(1000 / portTICK_PERIOD_MS);

}
