# Библиотека для работы с интеллектуальными сервоприводами STS3032

Перед началом работы иницализировать переменную-структуру шины.
```c
Sts3032connect_t sts_bus;
```
После этого объявить функции передачи и приема данных по UART:
```c
uint8_t TransmitUart6(uint8_t* Data, uint8_t len) { 
  HAL_HalfDuplex_EnableTransmitter(&huart6);
  HAL_UART_Transmit(&huart6, Data, len, 300);
}
uint8_t ReceiveUart6(uint8_t* Data, uint8_t len) {
  HAL_HalfDuplex_EnableReceiver(&huart6);
  HAL_UART_Receive(&huart6, Data, len, 10);
}
```
Затем в теле основной программы задать поля структуры sts_bus:
```c
  sts_bus.transmit = TransmitUart6;
  sts_bus.receive = ReceiveUart6;
```
Библиотека готова к работе!