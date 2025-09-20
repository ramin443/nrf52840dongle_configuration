#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(servo_demo5, LOG_LEVEL_INF);

/* Period and pulse-widths (MG90S) */
#define SERVO_PERIOD         PWM_USEC(20000)   /* 20 ms = 50 Hz */
#define SERVO_MIN_US         500               /* ~0°  */
#define SERVO_MAX_US         2500              /* ~180° */
#define SERVO_CENTER_US      1500              /* ~90° */

/* PWM controller nodes */
#define PWM0_NODE            DT_NODELABEL(pwm0)
#define PWM1_NODE            DT_NODELABEL(pwm1)

static inline uint32_t pulse_from_deg(int deg)
{
    if (deg < 0) deg = 0;
    if (deg > 180) deg = 180;
    return PWM_USEC(SERVO_MIN_US + ((SERVO_MAX_US - SERVO_MIN_US) * deg) / 180);
}

/* A simple descriptor for a servo: which device + which channel */
struct servo_desc {
    const struct device *dev;
    uint8_t ch;
};

/* We’ll fill these at runtime after getting the devices */
static struct servo_desc servos[5];

static int servo_set_deg(const struct servo_desc *s, int deg)
{
    return pwm_set(s->dev, s->ch, SERVO_PERIOD, pulse_from_deg(deg), PWM_POLARITY_NORMAL);
}

void main(void)
{
    /* Get PWM devices */
    const struct device *pwm0 = DEVICE_DT_GET(PWM0_NODE);
    const struct device *pwm1 = DEVICE_DT_GET(PWM1_NODE);

    if (!device_is_ready(pwm0)) { LOG_ERR("pwm0 not ready"); return; }
    if (!device_is_ready(pwm1)) { LOG_ERR("pwm1 not ready"); return; }

    /* Map servos to: pwm0 ch0..3 and pwm1 ch0 */
    servos[0] = (struct servo_desc){ .dev = pwm0, .ch = 0 };  /* S1 */
    servos[1] = (struct servo_desc){ .dev = pwm0, .ch = 1 };  /* S2 */
    servos[2] = (struct servo_desc){ .dev = pwm0, .ch = 2 };  /* S3 */
    servos[3] = (struct servo_desc){ .dev = pwm0, .ch = 3 };  /* S4 */
    servos[4] = (struct servo_desc){ .dev = pwm1, .ch = 0 };  /* S5 */

    LOG_INF("5-Servo demo starting (pwm0 ch0-3, pwm1 ch0)");

    /* Center all servos first so you see life */
    for (int i = 0; i < 5; ++i) {
        int ret = pwm_set(servos[i].dev, servos[i].ch, SERVO_PERIOD,
                          PWM_USEC(SERVO_CENTER_US), PWM_POLARITY_NORMAL);
        if (ret) { LOG_ERR("center s%d failed: %d", i+1, ret); }
    }
    k_sleep(K_SECONDS(1));

    while (1) {
        /* 0° on all */
        for (int i = 0; i < 5; ++i) {
            int ret = servo_set_deg(&servos[i], 0);
            if (ret) LOG_ERR("0deg s%d failed: %d", i+1, ret);
        }
        k_sleep(K_SECONDS(1));

        /* 180° on all */
        for (int i = 0; i < 5; ++i) {
            int ret = servo_set_deg(&servos[i], 180);
            if (ret) LOG_ERR("180deg s%d failed: %d", i+1, ret);
        }
        k_sleep(K_SECONDS(1));
    }
}
