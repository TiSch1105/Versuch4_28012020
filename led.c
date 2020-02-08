//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include "led.h"
#include "main.h"
#define TRUE 1
#define FALSE 0
typedef uint8_t bool;

// ----------------------------------------------------------------------------
//6.3.1.1 Initialisierung und Start
void init_adc(bool single_shot)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;                            // GPIOA clock an
	GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1;      // Analog Mode

	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;                             //clock ADC3 an
	ADC->CCR  |= ADC_CCR_ADCPRE_0;                                                  // Clock auf Teiler vier
	ADC3->CR2 |= ADC_CR2_ADON;                                                      // ADC an, dauert bis aktiv.
	ADC3->CR1 |= ADC_CR1_RES_1;                                                     // 0b10 -> 8 Bit Aufloesung
	//ADC3->CR2 |= ADC_CR2_ALIGN;                                                   // Linksbuendig abspeichern

	if(single_shot){
		ADC3->CR2 |= ADC_CR2_CONT; 		// Staendig wandeln
	}
	else{
		ADC3->CR2|=ADC_CR2_DDS;			//Single shot mode
	}

}

void start_adc(){
        ADC3->CR2 |= ADC_CR2_SWSTART;		// Start der Wandlung(en)
        }

//6.3.1.2 Umrechnung in mV
uint16_t adc_get(){                         //mit dieser funktion wird der aktuelle wert des adc in einen integer geschrieben
        int adress = (ADC3->DR&0xff);
        return adress;
}

int adc_in_millivolt(uint16_t adc_value) 	//nutzt die adc_get um dann den wert in mV auszugeben
{
        uint16_t value=3300*adc_value/255;	// adc_value wird mit 3300 (3,3V = 3300mV multipliziert und durch 255 geteilt
        return value;
}

void init_Timer2() {
 //Enable Timer 5
 RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Timer Clock an
 TIM2->PSC = 10000; // Vorteiler an
 TIM2->ARR = 50000; // Obere Grenze 		//
 TIM2->CR1 |= TIM_CR1_CEN; 		//Einschalten vom Timer
} //bei Verwendung von Interrupts muss der Flag deaktiviert werden - kompliziert - Zwischenspeichern von Werten im Cache verursacht minimale zeitliche Verzögerungen
GPIO_TypeDef *get_gpio_address(int gpioNumber)// durch funktionsaufruf wird die Adresse von den jeweiligen GPIO ausgegeben
{
return ((GPIO_TypeDef*)(AHB1PERIPH_BASE+gpioNumber*0x400U));
}
