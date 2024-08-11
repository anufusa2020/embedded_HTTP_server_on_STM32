int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
 
  while (1)
  {
      // Read PORT A, PIN 9 // can be changes later based on Requirement
      if(HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_9))
      {
          // Send the data to server in JSON format using  HTTP POST
          postHTTP_JSON_DataToServer();
          // Valid data available to be sent
          // Turn on the Blue LED
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
          // Later add code to turn on Green LED for Every message successfuly posted to the server
      }
      else
      {
          // No Valid Data to send, Turn on the Orange LED to Indicate waiting
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
      }
  }
}

void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
 
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
 
  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;         
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


void postHTTP_JSON_DataToServer() 
{
  Serial.println("Posting HTTP JSON data to server...");
  // Wait until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) 
  {
    HTTPClient http;   
    http.begin("https://sample.org/sample");  
    http.addHeader("Content-Type", "application/json");         
     
    StaticJsonDocument<400> doc;
    // Add values in the document
    //
    doc["sensor"] = stm_gpio;
    doc["time"] = 110820241230;
   
    // Add an array.
    //
    JsonArray data = doc.createNestedArray("data");
    data.add(48.756080);
    data.add(2.302038);
     
    String requestBody;
    serializeJson(doc, requestBody);
     
    int httpResponseCode = http.POST(requestBody);
    
    // handle HTP response code
    if(httpResponseCode>0)
    {
       
      String response = http.getString();                       
       
      Serial.println(httpResponseCode);   
      Serial.println(response);
     
    }
    else 
    {
     
      Serial.printf("Oops !!! Error while sending HTTP POST: %s\n", httpClient.errorToString(statusCode).c_str());
       
    }    
  }
} // HTTP JSON post API ends here

