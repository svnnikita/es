#include <libopencm3/stm32/rcc.h>   // reset and clock contol
#include <libopencm3/stm32/gpio.h>  // general purpose input-output
#include <libopencm3/stm32/timer.h>   // timers

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
   
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);

    rcc_periph_clock_enable(RCC_TIM6);
    timer_set_prescaler(TIM6, 8000 - 1);
    timer_set_period(TIM6, 1000 - 1);
    timer_enable_counter(TIM6);

    while (true) {
        if (timer_get_counter(TIM6) < 500) {
            gpio_set(GPIOD, GPIO15);
        }
        else {
            gpio_clear(GPIOD, GPIO15);
        }
    }
}
