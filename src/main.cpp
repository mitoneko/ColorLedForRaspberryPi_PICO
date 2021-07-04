#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/structs/rosc.h"
#include "led.pio.h"

static inline uint32_t rgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 24) | ((uint32_t)(r) << 16) | ((uint32_t)(b) << 8) | 0;
}

int main() {
    const int led_count = 60; // 制御LED数
    // データバッファ
    volatile uint32_t data_buff[60];
    for (int i=0; i<60; i++) data_buff[i]=0;
    
    // 乱数の初期化
    uint32_t seed = 0;
    for (int i=0 ; i<32 ; i++) {
        seed = (seed << 1) | rosc_hw->randombit;
    }
    srand(seed);

    // PIOの起動(PIO0,SM0を使用)
    pio_add_program(pio0, &led_program);
    led_program_init(15, (uint32_t *)&data_buff[0], led_count); // 出力PIN GP15

    while(1) {
        int index;
        index = rand()%led_count;
        data_buff[index] = rgb_u32(rand()%32, rand()%32, rand()%32);
        index = rand()%led_count;
        for (int i=0; i<3; i++) {
            if (data_buff[index] != 0) break;
            index = rand()%led_count;
        }
        data_buff[index] = 0;

        // DMAデータ転送開始
        while( dma_channel_is_busy(0) ) {}
        dma_channel_transfer_from_buffer_now(0, &data_buff[0], led_count);
        sleep_ms(50);
    }
}
