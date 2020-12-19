

void interrupts_test_interrupts(){

    init_interrupts();

    printf("Hello, interrupts");
}
void interrupt_init(){

    #ifdef INTERRUPTS
    _usart_interruptInit();

    sei();
    #endif
}