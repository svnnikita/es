#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/cm3/nvic.h>

// Прототипы функций
void pwm_setup(void);
void adc_setup(void);
uint16_t read_adc(void);

int main(void) {
    // Настройка тактовой частоты
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_180MHZ]);
    
    // Настройка ШИМ и АЦП
    pwm_setup();
    adc_setup();
    
    while (1) {
        // Читаем значение с потенциометра (0-4095)...
        uint16_t adc_value = read_adc();
        
        uint16_t pwm_value = 0;
        if (adc_value <= 100) {
            pwm_value = 0;
        }
        else pwm_value = 700 + (adc_value * 1000) / 4096;
        
        // ...и устанавливаем новую скважность
        timer_set_oc_value(TIM1, TIM_OC3, pwm_value);
        
        for (volatile int i = 0; i < 100000; i++);
    }
}

// Конфигурация ШИМ-сигнала (с разрешением вывода)
void pwm_setup(void) {
    // Включаем тактирование порта E и таймера 1
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_TIM1);
    
    // Настраиваем PA10 (TIM1_CH3) как таймер ==> 1 таймер 3 канал 10 кгц
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10);
    gpio_set_af(GPIOA, GPIO_AF1, GPIO10);
    
    // Настройка таймера 1
    timer_set_prescaler(TIM1, 18-1);    // 180 МГц/18 = 10 МГц
    timer_set_period(TIM1, 1000-1);     // 10 МГц/1000 = 10 кГЦ -- по заданию
    
    // Настройка канала 1 в режиме PWM1
    timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_value(TIM1, TIM_OC3, 0);
    
    // Разблокируем вывод таймера, чтобы его сигнал появился на выводе
    timer_enable_oc_output(TIM1, TIM_OC3);  // включаем выход таймера

    // Даже если включены отдельные каналы, главный выход должен быть разрешен:
    timer_enable_break_main_output(TIM1);
    
    // Включаем счетчик
    timer_enable_counter(TIM1);
}

void adc_setup(void) {
    // Включаем тактирование ADC1 и порта A
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_GPIOA);
    
    // Настраиваем PA0 (ADC1_IN0) как аналоговый вход
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0);
    
    // Сбрасываем настройки ADC
    adc_power_off(ADC1);
    
    // Настраиваем параметры ADC
    // Устанавливаем время выборки сигнала = 56 циклов АЦП
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_56CYC);

    // Устанавливаем 12-битное разрешение (значения 0-4095)
    adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);

    // Выравниваем результат по правому краю 16-битного регистра
    adc_set_right_aligned(ADC1);
    
    // Режим одиночного преобразования -- АЦП выполняет одно преобразование и останавливается
    adc_set_single_conversion_mode(ADC1);
    
    // Настраиваем регулярную последовательность из 1 канала (Channel 0)
    adc_set_regular_sequence(ADC1, 1, ADC_CHANNEL0);
    
    // Включаем питание преобразователя
    adc_power_on(ADC1);

    // Маленькая задержка для стабилизации перед преобразованием
    for (volatile int i = 0; i < 1000; i++);
}

// Делаем одиночное преобразование и возвращаем результат
uint16_t read_adc(void) {
    
    // АЦП начинает оцифровывать аналоговый сигнал на канале 0
    adc_start_conversion_regular(ADC1);
   
    // "Пока преобразование НЕ завершено..."
    while (!adc_eoc(ADC1));
    
    // Читаем результат
    return adc_read_regular(ADC1);
}