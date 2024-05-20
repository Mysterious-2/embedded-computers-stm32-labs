/*Task 2.1 Write code for read voltage from ADC 1 channel and print out voltage into UART if it changes
(increased/decreased) more than 1 V.*/

/* USER CODE BEGIN PM */
uint16_t adc_value;
/* USER CODE END PM */

/* USER CODE BEGIN PV */
volatile float voltage = 0;
volatile float currentVlt = -1;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_ADC_Start_IT(&hadc1);
char buffer[50];
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
    if (fabs(currentVlt - voltage) >= 1.0) // Check if the voltage has changed by more than 1V
    {
        int len = sprintf(buffer, "Voltage: %.2f V\r\n", voltage);
        HAL_UART_Transmit(&huart3, (uint8_t *)buffer, len, 100);
        currentVlt = voltage; // Update currentVlt after sending
    }
    HAL_ADC_Start_IT(&hadc1);
    HAL_Delay(100);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_value = HAL_ADC_GetValue(hadc);
    voltage = ((float)adc_value * 3.3) / 4095.0;
    HAL_ADC_Stop_IT(&hadc1);
}
/* USER CODE END 4 */

/*Task 2.2 Write the code for read ADC value, turn on Green LED if value is less than 1V, turn on Yellow LED
if value is more than 1V and less than 2V, turn on the RED LED if value is more than 2V.*/

/* USER CODE BEGIN PV */
uint16_t adc_value;
volatile float voltage = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_ADC_Start_IT(&hadc1);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
    if (voltage < 1.0)
    {

        HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);   // Green LED ON
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // Blue LED OFF
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET); // Red LED OFF
    }
    else if (voltage >= 1.0 && voltage < 2.0)
    {
        HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET); // Green LED OFF
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);   // Blue LED ON
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET); // Red LED OFF
    }
    else if (voltage >= 2.0)
    {
        HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET); // Green LED OFF
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); // Blue LED OFF
        HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);   // Red LED ON
    }
    HAL_ADC_Start_IT(&hadc1);

    HAL_Delay(100);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_value = HAL_ADC_GetValue(hadc);
    voltage = ((float)adc_value * 3.3) / 4095.0;
    HAL_ADC_Stop_IT(&hadc1);
}
/* USER CODE END 4 */

/*Task 2.3 Write a code to read ADC 20 values and calculate mean value. Please use interrupt for this task*/

/* USER CODE BEGIN PD */
#define NUM_SAMPLES 20
/* USER CODE END PD */

/* USER CODE BEGIN PV */
char msg[100];
volatile uint16_t adc_buffer[NUM_SAMPLES];
volatile uint8_t adc_index = 0;
volatile uint8_t data_ready = 0;
volatile float average_voltage = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_TIM_Base_Start_IT(&htim3);
HAL_ADC_Start_IT(&hadc1);
/* USER CODE END 2 */

/* USER CODE BEGIN WHILE */
while (1)
{
    if (data_ready) // Check if the average voltage is ready to be sent
    {
        char uart_buffer[50];
        sprintf(uart_buffer, "Average Voltage: %.2fV\r\n", average_voltage);
        HAL_UART_Transmit(&huart3, (uint8_t *)uart_buffer, strlen(uart_buffer), 100);
        data_ready = 0; // Reset the flag

        HAL_ADC_Start_IT(&hadc1); // Restart ADC
    }
    /* USER CODE END WHILE */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    HAL_ADC_Start_IT(&hadc1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    adc_buffer[adc_index++] = HAL_ADC_GetValue(hadc);

    if (adc_index >= NUM_SAMPLES)
    {
        uint32_t sum = 0;
        for (uint8_t i = 0; i < NUM_SAMPLES; i++)
        {
            sum += adc_buffer[i];
        }
        average_voltage = ((float)sum / NUM_SAMPLES) * 3.3 / 4095.0;
        adc_index = 0;
        data_ready = 1; // Set the flag to send data
        HAL_ADC_Stop_IT(hadc);
    }
}
/* USER CODE END 4 */
