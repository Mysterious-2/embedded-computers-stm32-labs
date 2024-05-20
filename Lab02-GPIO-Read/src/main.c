/*Task3.2 Write the code for turning LED ON using the USER push button and turning the LED OFF by
an external push button*/
while (1)
{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 1)
    {
        HAL_Delay(100);
        if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 1)
        {
            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
        }
    }
    if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == 0)
    {
        HAL_Delay(100);
        if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == 0)
        {
            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
        }
    }
}

/*Task 3.3 Write the code to start blinking the LED after X(as an example 5 times) presses a push
button. Continue counting the number of presses and turn LED off when the number
reaches X+Y time (as an example 5+5=10). Store the number of presses into a separate
global variable.*/

/* USER CODE BEGIN PD */
#define X 5
#define Y 5
/* USER CODE END PD */

/* USER CODE BEGIN PV */
int BTN_counter = 0;
int Total_BTN_Presses;
/* USER CODE END PV */

while (1)
{
    if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 1)
    {
        HAL_Delay(100); // Debouncing delay
        if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 1)
        {
            Total_BTN_Presses++;
            BTN_counter++;
            if (BTN_counter == X)
            {
                while (BTN_counter <= X + Y)
                {
                    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                    HAL_Delay(100);
                    if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 1)
                    {
                        if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 1)
                        {
                            BTN_counter++;
                            Total_BTN_Presses++;
                        }
                    }
                }
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
                BTN_counter = 0; // Reset BTN_counter after blinking
            }
        }
    }
}

/*Task 3.4 Write the code for detecting how many times (until 7) the push-button was pressed and
indicate this number of presses in binary on 3 LED’s, like:
If 1 press – 001
If 2 presses – 010*/

/* USER CODE BEGIN PV */
int BTN_counter = 0;
/* USER CODE END PV */

while (1)
{
    if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_SET)
    {
        HAL_Delay(100); // Debounce delay

        if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_SET)
        {
            // Wait until button is released
            while (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_SET)
                ;

            BTN_counter++;

            if (BTN_counter > 7)
            {
                BTN_counter = 1; // Reset counter to 1 if it exceeds 7
            }

            // Update LEDs based on BTN_counter in binary
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, (BTN_counter & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (BTN_counter & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, (BTN_counter & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }
}

/*Task 3.5 Write the code for toggling the LED if the push button was shortly pressed. If the button is
pressed and held for 5 seconds, the LED should start blinking*/

/* USER CODE BEGIN PV */
int press_duration = 0;
/* USER CODE END PV */

while (1)
{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_SET)
    {
        HAL_Delay(100); // Debounce delay

        if (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_SET)
        {
            // Wait until button is released or held for 5 seconds
            press_duration = 0;
            while (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_SET)
            {
                HAL_Delay(100);
                press_duration++;
                if (press_duration >= 50) // 5 seconds (50 * 100 ms)
                {
                    // Button held for 5 seconds, start blinking
                    while (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_SET)
                    {
                        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                        HAL_Delay(150);
                    }
                    press_duration = 0; // Reset press duration
                }
            }

            if (press_duration < 50) // Button pressed shortly
            {
                HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                press_duration = 0; // Reset press duration
            }
        }
    }
}
