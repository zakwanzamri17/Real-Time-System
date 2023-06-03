#define pinSensor 34

static const int num_tasks = 5;
static SemaphoreHandle_t Binary_Sem;

void setup() {
  Serial.begin(9600);
  pinMode(pinSensor, INPUT);
  

}

void loop() {
  int fire = digitalRead(pinSensor);
  if(fire == 1) {
    Serial.println("Tiada api");
  } else {
    Serial.println("API!!!");
  }
  
  delay(1000); 

}

void Task_1 (void *pvParameters) {
  int fire = digitalRead(pinSensor);
  while (1) {
    Serial.println("Task 1 run");

    if(fire == 1) {
      xSemaphoreGive(Binary_Sem);
    } else {
      Serial.println("Task 1 end");
      delay(400);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}

void Task_2 (void *pvParameters) {
  int fire = digitalRead(pinSensor);
  Binary_Sem = xSemaphoreCreateCounting(num_tasks, 0);
  while (1) {
    Serial.println("Task 2 run");
    if (fire == 0) {
      xSemaphoreTake(Binary_Sem,portMAX_DELAY);
      Serial.println("Task 2 end");
      delay(500);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}
