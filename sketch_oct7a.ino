      //FreeRTOS Queues – Three Tasks

      #include<Arduino.h>
      #include<freertos/FreeRTOS.h>

      #define POT_PIN 15
      #define LED_PIN 2

      QueueHandle_t q1 = NULL;
      QueueHandle_t q2 = NULL;

      void sensor(void *p){
        while(1){

          uint16_t v=analogRead(POT_PIN);
          xQueueSend(q1,&v,portMAX_DELAY);
          xQueueSend(q2,&v,portMAX_DELAY);
          vTaskDelay(300/portTICK_PERIOD_MS);
        }
      }

      void LEDTask(void *p){
        uint16_t l;

        while(1){
          xQueueReceive(q1,&l,portMAX_DELAY);
          ledcWrite(LED_PIN,l);
        }
      }

      void SerialTask(void *p){
        uint16_t s;
        while(1){
          xQueueReceive(q2,&s,portMAX_DELAY);
          Serial.print(s);

        }
      }

      void setup(){
        Serial.begin(115200);
        ledcAttach(LED_PIN,5000,12);
        q1=xQueueCreate(5,sizeof(uint16_t));
        q2=xQueueCreate(5,sizeof(uint16_t));

        xTaskCreatePinnedToCore(sensor,"sensor",3000,NULL,1,NULL,1); 
        xTaskCreatePinnedToCore(LEDTask,"LED",3000,NULL,1,NULL,1); 
        xTaskCreatePinnedToCore(SerialTask,"serial",3000,NULL,1,NULL,1);   
      }

      void loop(){

      }