#include <libopencm3/stm32/rcc.h> 
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/dac.h>
#include <math.h>

static void setup(){
    rcc_periph_clock_enable(RCC_GPIOE);
    rcc_periph_clock_enable(RCC_TIM1);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_DAC);
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4);
    dac_disable(DAC1, DAC_CHANNEL1);
    dac_enable(DAC1, DAC_CHANNEL1);

}


int main(void) {
    setup();
    int index = 0;


    while (1) {
        uint16_t sinw= uint16_t((1+sin(6.24*index/8000))*2047);
       dac_load_data_buffer_single(DAC1, sinw, DAC_ALIGN_RIGHT12, DAC_CHANNEL1);
       index++;      
    }
}