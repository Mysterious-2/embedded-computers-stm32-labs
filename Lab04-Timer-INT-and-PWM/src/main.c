/*Task 3.4 Write code for toggle 3 LEDs. Toggle one LED with 1Hz frequency inside of the while(1)
loop and two other LEDs with 10 Hz and 20Hz using two different Timer’s interrupts.*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim3)
    {
        HAL_GPIO_TogglePin(RED2_GPIO_Port, RED2_Pin);
    }
    else if (htim == &htim4)
    {

        HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
    }
}

/* USER CODE BEGIN 2 */
HAL_TIM_Base_Start_IT(&htim3);
HAL_TIM_Base_Start_IT(&htim4);
/* USER CODE END 2 */

/* Infinite loop */
/* USER CODE BEGIN WHILE */

while (1)
{
    HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
    HAL_Delay(500);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
}
/* USER CODE END 3 */

/*Task 3.5 Write code for the check User button. If the button is pressed one-time LED is ON for
about 10 sek and then OFF. If the button is pressed like “double click” LED must blink 5 seconds and after must be OFF. For time counting – please use Timer interrupt. You
should use HAL_TIM_Base_Start_IT function.*/

/* USER CODE BEGIN PV */
volatile uint8_t led_task_active = 0; // 0: no task active, 1: task active
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint32_t blink_count = 0;

    if (htim == &htim1)
    {
        // Turn off the LED after 10 seconds
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        HAL_TIM_Base_Stop_IT(htim);
        led_task_active = 0; // Clear task active flag
    }
    else if (htim == &htim3)
    {
        // Toggle LED for blinking
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

        // Increment the blink counter every toggle
        blink_count++;

        // If 10 toggles have occurred, stop the timer and reset the LED
        if (blink_count >= 10)
        {                                                              // 5 seconds have passed (10 * 0.5 sec)
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); // Ensure LED is off
            HAL_TIM_Base_Stop_IT(htim);
            blink_count = 0;
            led_task_active = 0; // Clear task active flag and counter
        }
    }
}
/* USER CODE END 0 */

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint32_t lastTime = 0;
    static uint8_t btnCounter = 0;
    uint32_t currentTime = HAL_GetTick();

    if (GPIO_Pin == USER_Btn_Pin && !led_task_active)
    { // Check if no task is active
        if (currentTime - lastTime < 300)
        {
            btnCounter++;
        }
        else
        {
            btnCounter = 1;
        }
        lastTime = currentTime;

        if (btnCounter == 1)
        {
            led_task_active = 1; // Set task active flag
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
            HAL_TIM_Base_Start_IT(&htim1);
        }
        else if (btnCounter == 2)
        {
            btnCounter = 0;      // Reset counter
            led_task_active = 1; // Set task active flag
            HAL_TIM_Base_Start_IT(&htim3);
        }
    }
}
/* USER CODE END 4 */

/*Task 3.6 Write code for changing two LED brightness using PWM (frequency 1 kHz and duty change
every 20 mS, up from 20% to 100% and down).*/

/* USER CODE BEGIN PV */
uint8_t duty_cycle = 20;   // Start at 20%
int8_t duty_direction = 1; // Used to increment or decrement
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ // Update duty cycle every 20ms
    if (htim == &htim1)
    {

        if (duty_cycle >= 100 || duty_cycle <= 20)
        {
            duty_direction *= -1; // Change direction at bounds
        }
        duty_cycle += duty_direction * 10; // Change duty cycle

        // Update PWM duty cycle
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (htim1.Init.Period * duty_cycle) / 100);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (htim1.Init.Period * duty_cycle) / 100);
    }
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
// Start base timer with interrupt
HAL_TIM_Base_Start_IT(&htim1);
/* USER CODE END 2 */
