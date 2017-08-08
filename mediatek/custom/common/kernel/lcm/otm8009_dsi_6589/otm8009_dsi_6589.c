/********************************************************/
/*      file name:otm8009_dsi_6589.c                    */
/*      By KosBeg for Lenovo A656                       */
/********************************************************/
#ifndef BUILD_LK
#include <linux/string.h>
#endif
#include "lcm_drv.h"

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
#elif defined(BUILD_UBOOT)
	#include <asm/arch/mt_gpio.h>
#else
	#include <mach/mt_gpio.h>
#endif

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------
#define FRAME_WIDTH  										(480)
#define FRAME_HEIGHT 										(854)

#define REGFLAG_DELAY             							0xFD
#define REGFLAG_END_OF_TABLE      							0xFE   // END OF REGISTERS MARKER

// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------
static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    								(lcm_util.set_reset_pin((v)))

#define UDELAY(n) 											(lcm_util.udelay(n))
#define MDELAY(n) 											(lcm_util.mdelay(n))

// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update)	lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update)		lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)										lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)					lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)										lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   			lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)       

static struct LCM_setting_table {
    unsigned cmd;
    unsigned char count;
    unsigned char para_list[64];
};

static void push_table(struct LCM_setting_table *table, unsigned int count, unsigned char force_update) {
  unsigned int i;
  for(i = 0; i < count; i++) {
    unsigned cmd;
    cmd = table[i].cmd;
    switch (cmd) {
      case REGFLAG_DELAY :
        MDELAY(table[i].count);
        break;
      case REGFLAG_END_OF_TABLE :
        break;
      default:
        dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update);
    }
  }
};

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util) {
  memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
};

static void lcm_get_params(LCM_PARAMS *params) {
  memset(params, 0, sizeof(LCM_PARAMS));
  params->type = 2;
  params->dsi.LANE_NUM = 2;
  params->dsi.data_format.format = 2;
  params->dsi.packet_size = 256;
  params->dsi.intermediat_buffer_num = 2;
  params->dsi.PS = 2;
  params->dsi.vertical_sync_active = 3;
  params->dsi.vertical_frontporch = 2;
  params->width = 480;
  params->dsi.horizontal_sync_active = 10;
  params->height = 854;
  params->dbi.te_mode = 1;
  params->dbi.te_edge_polarity = 0;
  params->dsi.mode = 1;
  params->dsi.data_format.color_order = 0;
  params->dsi.data_format.trans_seq = 0;
  params->dsi.data_format.padding = 0;
  params->dsi.vertical_backporch = 12;
  params->dsi.vertical_active_line = 854;
  params->dsi.horizontal_backporch = 50;
  params->dsi.horizontal_frontporch = 50;
  params->dsi.horizontal_active_pixel = 480;
  params->dsi.pll_div1 = 1;
  params->dsi.pll_div2 = 0;
  params->dsi.fbk_div = 13;
  params->dsi.compatibility_for_nvk = 0;
};

static unsigned int lcm_compare_id(void) {
  return 1;
};

static struct LCM_setting_table lcm_initialization_setting[] = {
    { 0x00, 0x01, {0x00}},
    { 0xFF, 0x04, {0x80, 0x09, 0x01, 0x01}},
    { 0x00, 0x01, {0x80}},
    { 0xFF, 0x02, {0x80, 0x09}},
    { 0x00, 0x01, {0x90}},
    { 0xB3, 0x01, {0x02}},
    { 0x00, 0x01, {0x92}},
    { 0xB3, 0x01, {0x45}},
    { 0x00, 0x01, {0xC0}},
    { 0xB3, 0x01, {0x11}},
    { 0x00, 0x01, {0xA6}},
    { 0xB3, 0x01, {0x20}},
    { 0x00, 0x01, {0xA7}},
    { 0xB3, 0x01, {0x01}},
    { 0x00, 0x01, {0xA3}},
    { 0xC0, 0x01, {0x00}},
    { 0x00, 0x01, {0xB4}},
    { 0xC0, 0x01, {0x50}},
    { 0x00, 0x01, {0x80}},
    { 0xC4, 0x02, {0x00, 0x04}},
    { 0x00, 0x01, {0x80}},
    { 0xC4, 0x01, {0x30}},
    { REGFLAG_DELAY, 20, {}},
    { 0x00, 0x01, {0x8A}},
    { 0xC4, 0x01, {0x40}},
    { REGFLAG_DELAY, 20, {}},
    { 0x00, 0x01, {0x80}},
    { 0xC5, 0x03, {0x03, 0x00, 0x03}},
    { 0x00, 0x01, {0x90}},
    { 0xC5, 0x07, {0x96, 0x2B, 0x04, 0x7B, 0x33, 0x33, 0x34}},
    { 0x00, 0x01, {0x00}},
    { 0xD8, 0x02, {0x70, 0x70}},
    { 0x00, 0x01, {0x00}},
    { 0xD9, 0x01, {0x3A}},
    { 0x00, 0x01, {0x81}},
    { 0xC1, 0x01, {0x66}},
    { 0x00, 0x01, {0x00}},
    { 0xE1, 0x10, {0x08, 0x12, 0x17, 0x0D, 0x06, 0x0E, 0x0A, 0x08, 0x05, 0x08, 0x0E, 0x09, 0x0F, 0x0F, 0x09, 0x03}},
    { 0x00, 0x01, {0x00}},
    { 0xE2, 0x10, {0x08, 0x12, 0x17, 0x0D, 0x06, 0x0E, 0x0A, 0x08, 0x05, 0x08, 0x0E, 0x09, 0x0F, 0x0F, 0x09, 0x03}},
    { 0x00, 0x01, {0x80}},
    { 0xCE, 0x0C, {0x85, 0x01, 0x00, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xA0}},
    { 0xCE, 0x0E, {0x18, 0x02, 0x03, 0x5B, 0x00, 0x00, 0x00, 0x18, 0x01, 0x03, 0x5C, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xB0}},
    { 0xCE, 0x0E, {0x18, 0x04, 0x03, 0x5D, 0x00, 0x00, 0x00, 0x18, 0x03, 0x03, 0x5E, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xC0}},
    { 0xCF, 0x0A, {0x01, 0x01, 0x20, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xD0}},
    { 0xCF, 0x01, {0x00}},
    { 0x00, 0x01, {0x80}},
    { 0xCB, 0x0A, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0x90}},
    { 0xCB, 0x0F, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xA0}},
    { 0xCB, 0x0F, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xB0}},
    { 0xCB, 0x0A, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xC0}},
    { 0xCB, 0x0F, {0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xD0}},
    { 0xCB, 0x0F, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xE0}},
    { 0xCB, 0x0A, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xF0}},
    { 0xCB, 0x0A, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}},
    { 0x00, 0x01, {0x80}},
    { 0xCC, 0x0A, {0x00, 0x26, 0x09, 0x0B, 0x01, 0x25, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0x90}},
    { 0xCC, 0x0F, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x0A, 0x0C, 0x02}},
    { 0x00, 0x01, {0xA0}},
    { 0xCC, 0x0F, {0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xB0}},
    { 0xCC, 0x0A, {0x00, 0x25, 0x0C, 0x0A, 0x02, 0x26, 0x00, 0x00, 0x00, 0x00}},
    { 0x00, 0x01, {0xC0}},
    { 0xCC, 0x0F, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x0B, 0x09, 0x01}},
    { 0x00, 0x01, {0xD0}},
    { 0xCC, 0x0F, {0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    { 0x35, 0x01, {0x00}},
    { 0x51, 0x01, {0x00}},
    { REGFLAG_DELAY, 10, {}},
    { 0x11, 0x01, {0x00}},
    { REGFLAG_DELAY, 150, {}},
    { 0x29, 0x01, {0x00}},
    { REGFLAG_DELAY, 100, {}},
    { 0x2C, 0x01, {0x00}},
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_sleep_out_setting[] = {
    { 0x28, 0x01, {0x00}},
    { REGFLAG_DELAY, 20, {}},
    { 0x10, 0x01, {0x00}},
    { REGFLAG_DELAY, 120, {}},
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static struct LCM_setting_table lcm_deep_sleep_mode_in_setting[] = {
    { 0x11, 0x01, {0x00}},
    { REGFLAG_DELAY, 120, {}},
    { 0x29, 0x01, {0x00}},
    { REGFLAG_DELAY, 20, {}},
    { REGFLAG_END_OF_TABLE, 0x00, {}}
};

static void lcm_init(void) {
  mt_set_gpio_mode(131, 0);
  mt_set_gpio_dir(131, 1);
  mt_set_gpio_out(131, 1);
  MDELAY(5);

  mt_set_gpio_out(131, 0);
  MDELAY(30);

  mt_set_gpio_out(131, 1);
  MDELAY(30);
  
  push_table(lcm_initialization_setting, sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
};

static void lcm_suspend(void) {
  push_table(lcm_deep_sleep_mode_in_setting, sizeof(lcm_deep_sleep_mode_in_setting) / sizeof(struct LCM_setting_table), 1);
};

static void lcm_resume(void) {
  push_table(lcm_sleep_out_setting, sizeof(lcm_sleep_out_setting) / sizeof(struct LCM_setting_table), 1);
};

LCM_DRIVER otm8009_dsi_6589_lcm_drv = 
{
    .name           = "otm8009_dsi_6589",
    .set_util_funcs = lcm_set_util_funcs,
    .compare_id     = lcm_compare_id,
    .get_params     = lcm_get_params,
    .init           = lcm_init,
    .suspend        = lcm_suspend,
    .resume         = lcm_resume,
};