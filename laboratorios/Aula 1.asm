; acender led

.equ LED1 = PB5 ;led1 é substituido de PB5 na programação
.equ LED2 = PB4
.equ LED3 = PB3
.equ LED4 = PB2

.ORG 0x000 ; endereço de inicio de escrita do código

INICIO:
LDI R16,0xFF ; carrega R16 com valor 0xFF

OUT DDRB,R16 ; configura todos os pinos do PORTB como saída

PRINCIPAL:
CBI PORTB, LED1 ; coloca o pino PB5 em 0v, acende o LED1
CBI PORTB, LED2
SBI PORTB, LED3 ;coloca o pino PB3 em 5V, apaga o LED3
CBI PORTB, LED4
RJMP PRINCIPAL
