#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(servo_demo, LOG_LEVEL_INF);

/* Controller + channel match your overlay (PWM0 CH0 -> P0.13) */
#define SERVO_PWM_NODE      DT_NODELABEL(pwm0)
#define SERVO_PWM_CHANNEL   0

/* MG90S needs ~50 Hz, 1–2 ms (often ~0.5–2.5 ms works) */
#define SERVO_PERIOD        PWM_USEC(20000)   /* 20 ms = 50 Hz */
#define SERVO_PULSE_0DEG    PWM_USEC(500)     /* ~0° */
#define SERVO_PULSE_180DEG  PWM_USEC(2500)    /* ~180° */
#define SERVO_PULSE_CENTER  PWM_USEC(1500)    /* ~90° */

static inline uint32_t pulse_from_deg(int deg)
{
    if (deg < 0) deg = 0;
    if (deg > 180) deg = 180;
    /* Linear map 0..180 -> 500..2500 us */
    return PWM_USEC(500 + ((2500 - 500) * deg) / 180);
}

void main(void)
{
    const struct device *pwm = DEVICE_DT_GET(SERVO_PWM_NODE);
    if (!device_is_ready(pwm)) {
        LOG_ERR("PWM device not ready");
        return;
    }

    /* Park at center briefly so you can see life */
    int ret = pwm_set(pwm, SERVO_PWM_CHANNEL, SERVO_PERIOD, SERVO_PULSE_CENTER,
                      PWM_POLARITY_NORMAL);
    if (ret) {
        LOG_ERR("pwm_set(center) failed: %d", ret);
        return;
    }
    LOG_INF("Centered (1.5 ms)");
    k_sleep(K_SECONDS(1));

    while (1) {
        /* 0° */
        ret = pwm_set(pwm, SERVO_PWM_CHANNEL, SERVO_PERIOD, SERVO_PULSE_0DEG,
                      PWM_POLARITY_NORMAL);
        if (ret) { LOG_ERR("pwm_set(0deg) failed: %d", ret); }
        else { LOG_INF("0 deg (0.5 ms)"); }
        k_sleep(K_SECONDS(1));

        /* 180° */
        ret = pwm_set(pwm, SERVO_PWM_CHANNEL, SERVO_PERIOD, SERVO_PULSE_180DEG,
                      PWM_POLARITY_NORMAL);
        if (ret) { LOG_ERR("pwm_set(180deg) failed: %d", ret); }
        else { LOG_INF("180 deg (2.5 ms)"); }
        k_sleep(K_SECONDS(1));
    }
}
