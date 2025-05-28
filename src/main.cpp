#include "circular_buffer.hpp"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>


int main() {
    Circular_buffer b;

    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2|GPIO3);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2|GPIO3);

    
    rcc_periph_clock_enable(RCC_USART2);

    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_stopbits(USART2, USART_STOPBITS_1);

    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_enable(USART2);

    while(true) {

        if (usart_get_flag(USART2, USART_SR_RXNE) and (not b.full())) {
            b.put(static_cast<uint8_t>(usart_recv(USART2)));
        }

        if (not b.empty()) usart_send_blocking(USART2, b.get());
            // uint16_t data = usart_recv(USART2);
        

            // if (!full) {
            //     buffer[wr_idx] = static_cast<uint8_t>(data);
            //     usart_send_blocking(USART2, buffer[wr_idx]);

            //     wr_idx++;
            //     wr_idx %= SIZE;

            //     if (wr_idx == rd_idx) full = true;
            // }
        }
}