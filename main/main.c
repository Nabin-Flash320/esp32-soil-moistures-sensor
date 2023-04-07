

#include "esp_err.h"
#include "esp_log.h"
#include "driver/adc.h" 
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "stdio.h"


#define REFERENCE_VOLTAGE 1100


static esp_adc_cal_characteristics_t* characteristics;
static const adc_channel_t channel = ADC_CHANNEL_1;
static const adc_bits_width_t bit_width = ADC_WIDTH_BIT_12;
static const adc_atten_t attenuation = ADC_ATTEN_DB_11;


static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}


void app_main()
{
    ESP_LOGI(__FILE__, "(%d, func: %s)", __LINE__, __func__);

    adc1_config_width(bit_width);
    adc1_config_channel_atten(channel, attenuation);

    characteristics = (esp_adc_cal_characteristics_t*)malloc(sizeof(esp_adc_cal_characteristics_t));
    memset(characteristics, 0, sizeof(esp_adc_cal_characteristics_t));

    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, attenuation, bit_width, REFERENCE_VOLTAGE, characteristics);
    print_char_val_type(val_type);

    while (1)
    {
        uint32_t adc_data = adc1_get_raw(channel);
        // uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_data, characteristics);
        // double percent_voltage = ((3112 - voltage) / 3112.0) * 100;
        double percent_data = ((4095 - adc_data) / 4095.0) * 100;
        // ESP_LOGI(__FILE__, "(%d, func: %s) (percent_data, percent_voltage) = (%f percent, %.3f percent)", __LINE__, __func__, percent_data, percent_voltage);
        ESP_LOGI(__FILE__, "(%d, func: %s) (percent_data = %.3f percent)", __LINE__, __func__, percent_data);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



