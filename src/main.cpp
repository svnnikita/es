// #include <libopencm3/stm32/rcc.h> 
// #include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/timer.h>
// #include <libopencm3/stm32/dac.h>
// #include <math.h>

// static void setup(){
//     rcc_periph_clock_enable(RCC_GPIOE);
//     rcc_periph_clock_enable(RCC_TIM1);
//     rcc_periph_clock_enable(RCC_GPIOA);
//     rcc_periph_clock_enable(RCC_DAC);
//     gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4);
//     dac_disable(DAC1, DAC_CHANNEL1);
//     dac_enable(DAC1, DAC_CHANNEL1);

// }



// int main(void) {
//     setup();
//     int t = 0;

//     int A = 5;
//     int T = 2;


//     while (1) {
//         // uint16_t sinw= uint16_t((1+sin(6.24*index/8000))*2047);
//         uint16_t triangle = uint16_t((A/T * 2 * abs(t/T) - floor(t/T + 0.5)));
//         dac_load_data_buffer_single(DAC1, triangle, DAC_ALIGN_RIGHT12, DAC_CHANNEL1);
//         index++;      
//     }
// }

#include <libopencm3/stm32/rcc.h> 
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/dac.h>

static void setup() {
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
    
    // Параметры треугольного сигнала
    const uint32_t A = 2047;        // Амплитуда (макс значение для 12-бит ЦАП = 4095)
    const uint32_t T = 5000;        // Период в отсчетах
    uint32_t index = 0;
    
    while (1) {
        // Вычисление фазы (0 до T-1)
        uint32_t phase = index % T;
        uint32_t triangle;
        
        // Формула треугольного сигнала
        if (phase < T / 2) {
            // Восходящий фронт: от 0 до 2A
            triangle = (4 * A * phase) / T;
        } else {
            // Нисходящий фронт: от 2A до 0
            triangle = (4 * A * (T - phase)) / T;
        }
        
        // Загрузка данных в ЦАП (12-бит выравнивание)
        dac_load_data_buffer_single(DAC1, triangle, DAC_ALIGN_RIGHT12, DAC_CHANNEL1);
        
        index++;
        
        // Простая задержка (регулирует частоту обновления)
        for (volatile int i = 0; i < 1000; i++);
    }
}