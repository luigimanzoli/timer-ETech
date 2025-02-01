#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"

// Definição dos LEDs RGB
#define RLED_PIN 13
#define GLED_PIN 11
#define BLED_PIN 12

absolute_time_t turn_off_time;  // Armazena o tempo em que o LED deve ser desligado

int contador = 0;

void init_all() {
    gpio_init(RLED_PIN);
    gpio_set_dir(RLED_PIN, GPIO_OUT);
    gpio_put(RLED_PIN, 0);

    gpio_init(GLED_PIN);
    gpio_set_dir(GLED_PIN, GPIO_OUT);
    gpio_put(GLED_PIN, 0);

    gpio_init(BLED_PIN);
    gpio_set_dir(BLED_PIN, GPIO_OUT);
    gpio_put(BLED_PIN, 0);
}

void get_led(bool R, bool G, bool B) {
    gpio_put(RLED_PIN, R);
    gpio_put(GLED_PIN, G);
    gpio_put(BLED_PIN, B);
}

// Inicializa o sistema de clock
void inicializar_clock() {
    bool ok = set_sys_clock_khz(100000, false);
    if (ok) {
        printf("Clock set to %ld\n", clock_get_hz(clk_sys));
    } else {
        printf("Falha ao configurar o clock\n");
    }
}

bool repeating_timer_callback(struct repeating_timer *t){
    contador++;
    if (contador == 1){
        get_led(1,0,0);
    }
    else if (contador == 2){
        get_led(0,1,0);
    }
    else if (contador == 3){
        get_led(0,0,1);
        contador = 0;
    }
}

// Função principal
int main() {
    // Inicializa clock, stdio e configurações
    stdio_init_all();
    init_all();
    inicializar_clock();

    printf("Sistema inicializado. Aguardando entrada...\n");

    // Configura um temporizador repetitivo que chama a função de callback a cada 3 segundo (3000 ms).
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true){
        sleep_ms(5000);
    }

    return 0;
}