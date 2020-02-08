#include "main.h"
#include "led.h"

void SysTick_Handler(void)
{
  HAL_IncTick();
}
