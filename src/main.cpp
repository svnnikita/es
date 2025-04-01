#include <libopencm3/stm32/rcc.h>   // reset and clock contol
#include <libopencm3/stm32/gpio.h>  // general purpose input-output

class Rcc {
public:
    Rcc () {
        rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    };
};

Rcc clock_system;

//==============================================================================
int main() {
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);

    rcc_periph_clock_enable(RCC_TIM6);
    timer_set_prescaler(TIM6, 8000 - 1);
    timer_set_period(TIM6, 1000 - 1);
    timer_enable_counter(TIM6);

    while (true) {
        gpio_toggle(GPIOD, GPIO15);
        for (volatile uint32_t i = 0; i < 2'000'000; ++i);   // апострофы - разделение числа на разряды
    }
}
