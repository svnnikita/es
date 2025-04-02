#include <libopencm3/stm32/rcc.h>   // reset and clock contol
#include <libopencm3/stm32/gpio.h>  // general purpose input-output
#include <libopencm3/stm32/timer.h>   // timers
#include <libopencm3/cm3/nvic.h>

// class Rcc {
// public:
//     Rcc () {
//         rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
//     };
// };

// Rcc clock_system;

//==============================================================================
int main() {
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_TIM6);
   
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);

    timer_set_prescaler(TIM6, 8000 - 1);
    timer_set_period(TIM6, 250 - 1);

    timer_enable_counter(TIM6);

    timer_enable_irq(TIM6, TIM_DIER_UIE);
    nvic_enable_irq(NVIC_TIM6_DAC_IRQ);

    while (true) {
        
    }
}

void tim6_dac_isr (void) {
    timer_clear_flag(TIM6, TIM_SR_UIF);
    gpio_toggle(GPIOD, GPIO15);
}
