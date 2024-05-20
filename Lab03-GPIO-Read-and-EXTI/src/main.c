/*Task 2.3 Build a circuit with two push buttons and EXTI. Count the first push-button presses and
store it in X variable. If you press two times second push button - blink a LED X many times.*/

/* USER CODE BEGIN PV */
uint8_t X = 0;            // to store the count of first button presses
uint8_t BTN2_Presses = 0; // to count the presses of the second button
bool isBlinking = false;  // flag to indicate blinking
/* USER CODE END PV */

/* Main loop of the program */
while (1)
{
    // Check if the second button has been pressed twice
    if (BTN2_Presses == 2)
    {
        BTN2_Presses = 0;  // Reset the second button counter
        isBlinking = true; // Set the blinking flag

        // Loop to blink the LED 'X' number of times
        for (uint16_t i = 0; i < X; i++)
        {
            HAL_GPIO_TogglePin(RED_GPIO_Port, GPIO_PIN_8);
            HAL_Delay(500);
            HAL_GPIO_TogglePin(RED_GPIO_Port, GPIO_PIN_8);
            HAL_Delay(500);
        }
        X = 0;
        isBlinking = false; // Reset flags
    }
}

/* Interrupt handler for rising edge signal on buttons */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    // Check if the LED is currently blinking
    if (!isBlinking)
    {
        // Increase count of X if first button is pressed
        if (GPIO_Pin == BTN1_Pin)
        {
            X++;
        }
        // Increase the press count for the second button
        else if (GPIO_Pin == BTN2_Pin)
        {
            BTN2_Presses++;
        }
    }
}

/*Task 2.4 Write the code to change the brightness of the two LED’s using two push buttons. One
button press should increase the brightness of the first LED and decrease the brightness of
the next and another button should do the same in opposite with changing brightness in
bugger steps with each press.*/

/* USER CODE BEGIN PV */
uint8_t delay_on = 1;
uint8_t delay_off = 5;
uint8_t delay_on2 = 1;
uint8_t delay_off2 = 5;
/* USER CODE END PV */

/* Main loop of the program */
while (1)
{
    // Control for the first LED
    HAL_GPIO_TogglePin(RED_GPIO_Port, GPIO_PIN_8);
    HAL_Delay(delay_on);
    HAL_GPIO_TogglePin(RED_GPIO_Port, GPIO_PIN_8);
    HAL_Delay(delay_off);

    // Control for the second LED
    HAL_GPIO_TogglePin(YELLOW_GPIO_Port, GPIO_PIN_9);
    HAL_Delay(delay_on2);
    HAL_GPIO_TogglePin(YELLOW_GPIO_Port, GPIO_PIN_9);
    HAL_Delay(delay_off2);
}

/* Interrupt handler for rising edge signal on buttons */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BTN1_Pin)
    {
        delay_on += 1;
        delay_off += 1; // Increase delays for the first led
        delay_on2 -= 1;
        delay_off2 -= 1; // Decrease delays for the second LED
    }
    else if (GPIO_Pin == BTN2_Pin)
    {
        delay_on -= 1;
        delay_off -= 1; // Decrease delays for the first led
        delay_on2 += 1;
        delay_off2 += 1; // Increase delays for the second LED
    }
    // Ensure delay times remain within functional boundaries
    delay_on = (delay_on < 1) ? 1 : delay_on;
    delay_off = (delay_off > 5) ? 5 : delay_off;
    delay_on2 = (delay_on2 < 1) ? 1 : delay_on2;
    delay_off2 = (delay_off2 > 5) ? 5 : delay_off2;
}

/*Task 2.5 Use EXTI to Write the code for toggling the LED if the push button was shortly pressed. If
the button is pressed and held for 5 seconds, the LED should start blinking*/

/* USER CODE BEGIN PD */
#define DEBOUNCE_TIME_MS 50 // Time for button debounce
/* USER CODE END PD */

/* USER CODE BEGIN PV */
uint8_t lastDebounceTime = 0;
uint8_t counterBTN = 0;
uint8_t blinkingFlag = 0;
/* USER CODE END PV */

while (1)
{
    // Check if button is pressed and held for at least 5 seconds
    if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET && (HAL_GetTick() - timerBTN >= 5000))
    {
        blinkingFlag = 1; // Set the blinking flag
    }
    if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_RESET && blinkingFlag)
    {
        blinkingFlag = 0;
        timerBTN = 0; // Reset the counter and flag
    }
    if (blinkingFlag) // Start blinking
    {

        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        HAL_Delay(200);
    }
}
/* USER CODE END WHILE */

/* Interrupt handler for rising and falling edge signal on buttons */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == USER_Btn_Pin)
    {
        uint32_t currentTime = HAL_GetTick();
        if (currentTime - lastDebounceTime > DEBOUNCE_TIME_MS) // Check for debounce time
        {
            lastDebounceTime = currentTime; // Update the last debounce time
            if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET)
            {                             // Detecting rising edge
                timerBTN = HAL_GetTick(); // Update the button press timer with current time
            }
            else // Detecting falling edge
            {
                if (!blinkingFlag)
                {
                    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin); // Toggle LED state on short press
                }
                blinkingFlag = 0;
                timerBTN = 0; // Reset flags
            }
        }
    }
}
