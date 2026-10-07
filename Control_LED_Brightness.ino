// Using Queues to Control LED Brightness

#define POT_PIN 15
#define LED_PIN 2
#define PWM_FREQ 5000
#define PWM_RESOLUTION 12
#define Queue_Size 5

QueueHandle_t potQueue = NULL;

void potTask(void *parameter ){

  for(;;){
    uint16_t potValue = analogRead(POT_PIN);
    xQueueSend(potQueue,&potValue,portMAX_DELAY);
    Serial.printf("potTask: send pod value %u");
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

void LEDBrightnessTask(void *parameter){
  for(;;){
    uint16_t potValue;
    if(xQueueReceive(potQueue,&potValue,portMAX_DELAY)){
      uint16_t brightness = potValue;
      ledcWrite(LED_PIN,brightness);
    Serial.printf("LEDBrightness :setbrightness %u/n",brightness);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Setup PWM for LED
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);

  //create queue
  potQueue = xQueueCreate(Queue_Size, sizeof(uint16_t));
  if (potQueue == NULL) {
    Serial.println("Failed to create queue!");
    while (1);
  }

  // Creat task
  xTaskCreatePinnedToCore(potTask, "potTask",3000,NULL,1,NULL,1);

  xTaskCreatePinnedToCore(LEDBrightnessTask,"LEDBrightnessTask",3000,NULL,1,NULL,1);
}

void loop() {
  // Empty
}
