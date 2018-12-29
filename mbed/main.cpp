
 
#include "mbed.h"
#include "USBSerial.h"
#include "BufferedSerial.h"

#define RING_BUF_SIZE     256

Serial uart(USBTX, USBRX);
// USBSerial pc;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

DigitalOut pwr(A1);
DigitalOut gnd(A2);

AnalogIn probe(A0);


uint16_t ringbuf[RING_BUF_SIZE] = {0,};
uint16_t ringbuf_index = 0;
int avg = 0;



int main()
{
    uart.baud(115200);
    led1 = 1;
    led2 = 0;
    led3 = 0;
    led4 = 0;

    gnd = 0;
    pwr = 1;

    wait(1);

    while (1) {
        uint32_t v_sum = 0;
        uint16_t v_avg = 0;
        uint16_t v_max = 0;
        uint16_t v_min = 0xFFFF;
        
        uint16_t v = probe.read_u16();

        ringbuf[ringbuf_index] = v;
        ringbuf_index = (ringbuf_index + 1) % RING_BUF_SIZE;

        for (uint16_t i=0; i<(RING_BUF_SIZE/2); i++) {
            uint16_t x = ringbuf[(i + ringbuf_index) % RING_BUF_SIZE];
            if (x > v_max) {
                v_max = x;
            }

            if (x < v_min) {
                v_min = x;
            }

            v_sum += x;
        }


        v_avg = v_sum / (RING_BUF_SIZE/2);
        // if (((v_max - v_avg) < 64) && ((v_avg - v_min) < 64))
        {
            avg = v_avg;
        }

        // pc.printf("%d, %d, %d\n", v_min, v_avg, v_max);
        uart.printf("%d, %d, %d - %d\n", v_min, v_avg, v_max, avg);

        // pc.printf("%d\n", v);

        uart.printf("%d\n", v);

        wait(0.001);

        led4 = !led4;

        if ((avg - v) >= 128) {
            pwr = 0;
            wait(2);
            pwr = 1;
            wait(0.1);
        }

    }
}
