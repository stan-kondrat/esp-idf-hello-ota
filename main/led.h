#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void blink_led(void);
void configure_led(void);
void led_task(void *arg);

#ifdef __cplusplus
}
#endif
