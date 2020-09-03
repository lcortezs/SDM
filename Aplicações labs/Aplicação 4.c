/*
* 4A_EXP_PWM_SDM.c
*
* Created: 05/06/2019 18:13:45
* Author : Ana Watanabe
*/

#define F_CPU 16000000UL /*define a frequência do microcontrolador 16MHz
(necessário para usar as rotinas de atraso)*/
#include <avr/io.h> //definições do componente especificado
//#include <C:\Users\Ana Watanabe\Documents\Atmel
Studio\7.0\GccApplication2\GccApplication2\Header\gpio.h>
#include <util/delay.h> //para incluir rotina _delay_ms()

int main(void)
{
unsigned char i;
// led saida PB3
DDRB = 0b00001000; // configura o bit 3 => PB3
 // buzzer saida PD3
DDRD = 0b00001000; // configura o bit 3 => PD3
 // saida PB3 para OC2A página 203
// saida PD3 para OC2B
TCCR2A = 0b11100001; //MODO 1 para PWM fase corrigida, saida
OC2A(INVERTIDA) e OC2B
// NÃO invertida TOP = 0xFF, OC2A e OC2B habilitados
TCCR2B = 0b00000001; // liga TC2, prescaler = 1
OCR2B = 0; //controle do ciclo ativo do PWM (0%) no pino OC2B => desligar
o buzzer
 OCR2A = 0; //controle do ciclo ativo do PWM (0%) no pino OC2A => desligar o
led
 while (1)
{
for(i = 255; i > 0; i =i-5) // inicia com 100% - led 100% aceso
{
OCR2A = i;
OCR2B = i;
_delay_ms(100);
}
}
}
// Este exemplo é com saida invertida, caso fosse saida não invertida, teríamos o
contrário.
// COMxy1 COMxy0
// 1 0 saída não invertida => 0
// 1 1 saída invertida => 1
