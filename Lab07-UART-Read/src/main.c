/*Task 1. Write code for read ASCII numbers from UART from “1” to “9” and blink an LED so many
times.*/

/* USER CODE BEGIN PV */
uint8_t rxData[1]; // Buffer to store received data
volatile uint8_t blinkCount = 0; // Number of times to blink the LED
volatile uint8_t blinkCounter = 0; // Counter to keep track of the number of blinks
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_UART_Receive_IT(&huart3, rxData, 1); 
HAL_TIM_Base_Start_IT(&htim3);         
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart3)
    {
        if (rxData[0] >= '1' && rxData[0] <= '9')
        {
            blinkCount = rxData[0] - '0'; // Convert ASCII to integer
            blinkCounter = 0;             // Reset blink counter
        }

        HAL_UART_Receive_IT(&huart3, rxData, 1); // Receive next byte
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim3)
    {
        if (blinkCounter < blinkCount)  // Check if the required number of blinks is completed
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0); // Toggle LED
            blinkCounter++;
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // Ensure LED is off
            blinkCounter = 0;
            blinkCount = 0; // Reset flags after completing the required blinks
        }
    }
}
/* USER CODE END 4 */

/*Task 2. User 3 color LED’s. Write a code to read commands - sequence of bytes: „R11“, „G21“, „B31“
and blinks dedicated LED’s. If you will get „R10“, „G20“, „B30“ – the dedicated LED needs to
stop blinking. Please use two interrupts for UART and the Time of the blinking period. For
the parser, please use a switch case.*/

/* USER CODE BEGIN PV */
uint8_t rxData[3]; // Buffer to store received data
volatile uint8_t ledStateR = 0, ledStateG = 0, ledStateB = 0; // Flags to control LED states
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
HAL_UART_Receive_IT(&huart3, rxData, 3); 
HAL_TIM_Base_Start_IT(&htim3);
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart3) 
    {
        switch (rxData[0])  // Check the first byte of the received data
        {
        case 'R':  // Check if the command is for the Red LED
            ledStateR = (rxData[1] == '1' && rxData[2] == '1') ? 1 : ((rxData[1] == '1' && rxData[2] == '0') ? 0 : ledStateR); // Check the second and third byte of the received data
            break; 
 
        case 'G': // Check if the command is for the Green LED
            ledStateG = (rxData[1] == '2' && rxData[2] == '1') ? 1 : ((rxData[1] == '2' && rxData[2] == '0') ? 0 : ledStateG);
            break;

        case 'B': // Check if the command is for the Blue LED
            ledStateB = (rxData[1] == '3' && rxData[2] == '1') ? 1 : ((rxData[1] == '3' && rxData[2] == '0') ? 0 : ledStateB); 
            break;
        }

        HAL_UART_Receive_IT(&huart3, rxData, 3);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim3)
    {
        if (ledStateR)
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        }
        if (ledStateG)
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        }
        if (ledStateB)
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        }
    }
}
/* USER CODE END 4 */