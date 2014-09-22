#ifndef __DATA_INIT_H
#define __DATA_INIT_H

#include "include.h"

extern void save_flag(uint16_t write_addr, uint8_t flag_value,  uint8_t bit);
extern uint8_t read_flag(uint16_t read_addr, uint8_t bit);

extern void save_param(uint16_t write_addr, uint8_t *pTemp,  uint8_t len);
extern void read_param(uint16_t read_addr,  uint8_t *pTemp,  uint8_t len);
extern void save_default_parameter(void);
extern void read_all_parameter(void);
#endif

