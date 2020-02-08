#include "main.h"
#include "led.h"
#include "usart.h"
#define TRUE 1
#define FALSE 0
typedef uint8_t bool;
#include "led.h"

void draw_rect_center() {

	uint16_t rectSize = 100;
	uint16_t centerX  = BSP_LCD_GetXSize()/2;
	uint16_t centerY  = BSP_LCD_GetYSize()/2;
	uint16_t rectX    = centerX-rectSize/2;
	uint16_t rectY    = centerY-rectSize/2;
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_FillRect(rectX, rectY, rectSize, rectSize); //fill with textcolor
}

void draw_text_center(char *text) {
	uint16_t centerX  = BSP_LCD_GetXSize()/2;
	uint16_t centerY  = BSP_LCD_GetYSize()/2;
	sFONT *textFont   = &Font16;
	uint16_t textH    = textFont->Height;
	uint16_t textW    = textFont->Width*strlen(text);
	BSP_LCD_SetFont(textFont);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
	BSP_LCD_DisplayStringAt(centerX-textW/2,centerY-textH/2, (uint8_t *)text, LEFT_MODE);
}

void init_ADC() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 						// GPIOA clock an
	GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1; 	// Analog Mode

	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN; 						//clock ADC3 an
	ADC->CCR  |= ADC_CCR_ADCPRE_0; 								// Clock auf Teiler vier
	ADC3->CR2 |= ADC_CR2_ADON;    								// ADC an, dauert bis aktiv.
	ADC3->CR1 |= ADC_CR1_RES_1;   								// 0b10 -> 8 Bit Aufloesung
	//ADC3->CR2 |= ADC_CR2_ALIGN;							    // Linksbuendig abspeichern
	ADC3->CR2 |= ADC_CR2_CONT;
	ADC3->CR2 |= ADC_CR2_SWSTART;

}

//6.3.1.1 Initialisierung und Start
//void init_adc(bool single_shot)
//{
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                            // GPIOA clock an
//	GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1;      // Analog Mode
//
//	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;                             //clock ADC3 an
//	ADC->CCR  |= ADC_CCR_ADCPRE_0;                                                  // Clock auf Teiler vier
//	ADC3->CR2 |= ADC_CR2_ADON;                                                      // ADC an, dauert bis aktiv.
//	ADC3->CR1 |= ADC_CR1_RES_1;                                                     // 0b10 -> 8 Bit Aufloesung
//	//ADC3->CR2 |= ADC_CR2_ALIGN;                                                   // Linksbuendig abspeichern
//
//	if(single_shot){
//		ADC3->CR2 |= ADC_CR2_CONT; 		// Staendig wandeln
//	}
//	else{
//		ADC3->CR2|=ADC_CR2_DDS;			//Single shot mode
//	}
//
//}

int main(void) {
	int counter = 0;

	LCD_LOG_DeInit();
	BSP_LCD_Init();
	BSP_LCD_Clear(LCD_COLOR_YELLOW);
	draw_rect_center();
	draw_text_center("Hallo MIT Labor!");
	init_ADC(FALSE);
	HAL_Delay(2000);
	BSP_LCD_Clear(LCD_COLOR_BLUE);
	init_Timer2();
	while (1)
	{
		while(!(TIM2->SR & TIM_SR_CC2IF));
		TIM2->SR &= ~TIM_SR_CC2IF;
		int adcres = (ADC3->DR&0xff);
		float result = 3.3f*adcres/255.f;
		double dresult = result;
		char buffer[20];
		//snprintf(buffer, sizeof(buffer), "%5d:ADC=%4.2f", counter++, result);
		draw_text_center(buffer);

		//--------------------------------------------------------------------
//6.3.1.2 Umrechnen in mV
		int time_ref = HAL_GetTick();
		//Funktion um eine Referenzzeit zu bilden die mit abgebildet wird

		int value = adc_in_millivolt(adc_get());
		// in value wird der aktuelle wert des adc gespeichert	durch aufruf der beiden funktionen
//

		printf("%5d:%2d.%02dV\n", HAL_GetTick()/1000, value/1000, (value%1000)/10);

//--------------------------------------------------------------------------------
//6.3.2 Interrupt

//		snprintf(buffer, sizeof(buffer),"%5d:%2d.%02dV\n", time_ref/1000, value/1000, (value%1000)/10);
//		//schreibt den aktuellen wert des adc in der form "tt 0.00V"
//
//		draw_text_center(buffer);
		//gibt den char auf das Display aus
/*//-----------------------------------------------------------------------------------------------------
		//es wird eine bitmaske generiert mithilfe sich eine "analoge Anzeige" darstellen lässt
		uint8_t Dioden=0xff<<adc_get()/32;
		//zuerst werden 8 bit zu 1, dadurch das der Integer unsigned ist werden nullen nachgezogen beim verschieben, es wird jeweils um die entsprechenden stellen verschoben
		virtual_Led(~Dioden);
		// nutzt die Funktion virtual_Led und negiert den uint8_t

*///--------------------------------------------------------------------

	}
}


