#ifndef CLOCK_H
#define CLOCK_H

#include <stdbool.h>
#include <stdint.h>

bool set_sys_clock_to_72MHz (void);
void clocking_port (uint32_t io_port_reset, uint32_t clock_en);

#endif // CLOCK_H
