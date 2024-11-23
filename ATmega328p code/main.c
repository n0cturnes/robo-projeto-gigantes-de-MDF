#define F_CPU 16000000UL //define a frequência da CPU

#include <avr/io.h>
#include <avr/interrupt.h>

#define THRESHOLD 950  // Limiar de luz para o LDR (0 a 1023 para 10 bits)
#define LDR_PIN PC1    // Pino do LDR
#define TEMPO_INVULNERABILIDADE 5000 // Tempo de invulnerabilidade após ser atingido em milissegundos
#define BLINK_INTERVAL 1000      // Intervalo de piscada (em ms)

#define USART_BAUDRATE 9600 //baudrate desejado
#define BAUD_PRESCALER (((F_CPU/(USART_BAUDRATE*16UL)))-1)
#define ASYNCHRONOUS (0<<UMSEL00)  //seleção do modo de operação da USART

//configura o tipo de pairidade
#define DISABLED (0<<UPM00)
#define EVEN_PARITY (2<<UPM00)
#define ODD_PARITY (3<<UPM00)
#define PARITY_MODE DISABLED

//configurações de stopbit
#define ONE_BIT (0<<USBS0)
#define TWO_BIT (1<<USBS0)
#define STOP_BIT ONE_BIT	  //USART stop bit selection

//seleção de quantidade de bits para transmitir
#define FIVE_BIT (0<<UCSZ00)
#define SIX_BIT (1<<UCSZ00)
#define SEVEN_BIT (2<<UCSZ00)
#define EIGHT_BIT (3<<UCSZ00)
#define DATA_BIT EIGHT_BIT

#define RX_COMPLETE_INTERRUPT (1<<RXCIE0)
#define DATA_REGISTER_EMPTY_INTERRUPT (1<<UDRIE0)

volatile uint8_t USART_ReceiveBuffer; //global buffer

volatile int vidas = 3;						// Variável de controle da vida
volatile uint16_t contador = 0;				// Contador para controle de invulnerabilidade
volatile uint8_t invulneravel = 0;			// Flag de invulnerabilidade
volatile uint16_t contador_laser = 0;        // Contador para o tempo do laser

void TIMER0_init() {
	// Configura o Timer0 para gerar interrupções a cada 1 ms
	TCCR0A = (1 << WGM01);               // Modo CTC
	TCCR0B = (1 << CS01) | (1 << CS00);  // Prescaler 64
	OCR0A = 249;                         // F_CPU = 16 MHz -> (16e6 / (64 * 1e3)) - 1 = 249
	TIMSK0 = (1 << OCIE0A);              // Habilita interrupção de comparação
}

void USART_INIT(){
	//configura Baudrate
	UBRR0H = BAUD_PRESCALER>>8;
	UBRR0L = BAUD_PRESCALER;
	
	//configura o formato do frame
	UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;
	
	//habilita transmissão e recepção
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	//habilita a interrupção global
	sei();
}

void ADC_INIT() {
	// Configura o ADC
	ADMUX = (1 << REFS0) | LDR_PIN; // Referência AVCC, entrada no PC1
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1); // Habilita ADC, interrupção e prescaler de 64
}

void start_ADC_conversion() {
	ADCSRA |= (1 << ADSC); // Inicia a conversão ADC
	
}

void apagar_led() {
	switch (vidas) {
		case 3: PORTD &= ~(1 << 4); break; // Apaga LED3
		case 2: PORTD &= ~(1 << 6); break; // Apaga LED2
		case 1: PORTD &= ~(1 << 7); break; // Apaga LED1
		default: break; // Nenhuma ação se não houver LEDs restantes
	}
}

void reset_leds() {
	vidas = 3; // Reinicia a contagem dos LEDs
	PORTD |= (1 << 4) | (1 << 6) | (1 << 7); // Liga todos os LEDs
}

int main(void)
{
	ADC_INIT();
	USART_INIT();
	start_ADC_conversion();
	TIMER0_init();
	
	DDRB &= ~(1 << 0);	//define a porta do botão que reseta as vidas como entrada
	PCICR |= (1 << PCIE0);      // Habilita interrupção do PCINT0
	PCMSK0 |= (1 << PCINT0);    // Habilita interrupção para PB0
	
	UCSR0B|=RX_COMPLETE_INTERRUPT;
	DDRB |= (1<<1) | (1<<2);					//define as portas dos motores como saída
	DDRC |= (1<<0) | (1<<2) | (1<<3) | (1<<4);	//define as portas dos motores como saída
	DDRD |= (1<<4) | (1<<5) | (1<<6) | (1<<7);	//define as portas dos LEDS e do laser como saída
	

	PORTD |= (1<<4) | (1<<6) | (1<<7);			//inicia os leds ligados
	
	while (1)
	{

	}
}

ISR(ADC_vect) {
	uint16_t adc_value = ADC; // Lê o valor do ADC (10 bits)
	if (adc_value > THRESHOLD && vidas > 0 && !invulneravel) {
		apagar_led(); // Apaga o LED correspondente
		vidas--;           // Decrementa a variável de vida
		invulneravel = 1;           // Ativa o estado de invulnerabilidade
		contador = 0;                // Reinicia o contador
	}
	start_ADC_conversion(); // Recomeça a conversão
}

ISR(TIMER0_COMPA_vect) {
	if (invulneravel) {
		PORTB&=~(1<<1) & ~(1<<2);	//desliga o enable dos motores
		contador++; // Incrementa o contador a cada 1 ms
		if (contador >= TEMPO_INVULNERABILIDADE && vidas!=0) {
			PORTB |=(1<<1) | (1<<2);	//liga o enable dos motores
			invulneravel = 0; // Sai do estado de invulnerabilidade após o tempo definido
		}
	}
	
	// Controle do LED piscante
	contador_laser++;
	if (contador_laser >= BLINK_INTERVAL && vidas != 0) {
		contador_laser = 0;      // Reinicia o contador
		PORTD ^= (1<<5);		//Inverte o estado do laser
	}
	if(vidas == 0){
		PORTB &= ~(1<<5);
	}
}

ISR(PCINT0_vect) {
	// Interrupção do botão (PB0)
	if (PINB & (1 << 0)) { // Verifica se o botão foi pressionado
		reset_leds(); // Reinicia a contagem e acende os LEDs
	}
}

ISR(USART_RX_vect){
	USART_ReceiveBuffer = UDR0;
	if(USART_ReceiveBuffer == 'F'){
		PORTB|=(1<<1) | (1<<2); //liga os enables dos motores
		PORTC |= (1<<0) | (1<<4);
		PORTC &= ~(1<<2) & ~(1<<3);
	}
	else if(USART_ReceiveBuffer == 'B'){
		PORTB|=(1<<1) | (1<<2); //liga os enables dos motores
		PORTC |= (1<<2) | (1<<3);
		PORTC &= ~(1<<0) & ~(1<<4);
	}
	else if(USART_ReceiveBuffer == 'S'){
		PORTB&=~(1<<1) & ~(1<<2);	//desliga o enable dos motores
	}
	else if(USART_ReceiveBuffer == 'L'){
		PORTB|=(1<<1); //liga o enable do motor 1
		PORTB &= ~(1<<2); //desliga o enable do motor 2
		PORTC |= (1<<0);
		PORTC &= ~(1<<2);
	}
	else if(USART_ReceiveBuffer == 'R'){
		PORTB|=(1<<2); //liga o enable do motor 2
		PORTB &= ~(1<<1); //desliga o enable do motor 1
		PORTC |= (1<<4);
		PORTC &= ~(1<<3);
	}
}