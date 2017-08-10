#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
#include <mach/upmu_common.h>

static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 2,
	.polling_mode_ps =0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    .als_level  = {0, 59, 386, 631, 1430, 4471, 5258, 8617, 16598, 20770, 24870, 38843, 47976, 47976, 47976},
    .als_value  = {0, 132, 301, 500, 1002, 2001, 3006, 5007, 8006, 10010, 12010, 16010, 20010, 20010, 20010, 20010},
    .ps_threshold_high = 22,
    .ps_threshold_low = 18,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}

