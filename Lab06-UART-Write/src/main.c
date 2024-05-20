/*Task 2.3 Write code for Send numbers from “1” to “10” into your terminal in decimal and hex
formats*/

/* USER CODE BEGIN 2 */
char msg[30]; // Buffer for UART message
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
    for (uint8_t i = 1; i <= 10; i++)
    {
        // Convert number to ASCII and send in decimal format
        sprintf(msg, "Decimal: %d\r\n", i);
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

        // Convert number to ASCII and send in hexadecimal format
        sprintf(msg, "Hexadecimal: %X\r\n", i);
        HAL_UART_Transmit(&huart3, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

        HAL_Delay(1000);
    }
}
/* USER CODE END 3 */

/*Task 2.4 Create variables for resistance and voltage in your code and write software wich calculates
current and send into UART this output:*/

/* USER CODE BEGIN PV */
volatile float resistance = 100.0; // Resistance in Ohms
volatile float voltage = 115.0;    // Voltage in Volts
volatile float current;
char uartTxBuffer[50];
/* USER CODE END PV */

/* USER CODE BEGIN WHILE */
while (1)
{
    current = voltage / resistance;
    int length = snprintf(uartTxBuffer, sizeof(uartTxBuffer), "Current: %.2f A\r\n", current); // Format the output
    HAL_UART_Transmit(&huart3, (uint8_t *)uartTxBuffer, length, 1000);                         // Send the output
    HAL_Delay(1000);
}
/* USER CODE END WHILE */
