/*
 * Simple ARM debug interface for Arduino, using the SWD (Serial Wire Debug) port.
 * Extensions for Freescale Kinetis chips.
 *
 * Copyright (c) 2013 Micah Elizabeth Scott
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Updated for use with a Teensy Board and its HalfKay bootloader by Timo Sandmann, 2018
 */

#ifndef ARM_KINETIS_DEBUG_H_
#define ARM_KINETIS_DEBUG_H_

#include "arm_debug.h"


class ARMKinetisDebug : public ARMDebug {
public:
    ARMKinetisDebug(const uint8_t clockPin, const uint8_t dataPin, const LogLevel logLevel = LOG_NORMAL);

    bool detect(); // Detect supported Kinetis hardware
    bool reset(const bool check_sec_bit = true); // System reset
    bool sys_reset_request(); // System reset request

protected:
    // MDM-AP debug port registers
    static constexpr uint32_t REG_MDM_STATUS { 0x01000000 };
    static constexpr uint32_t REG_MDM_CONTROL { 0x01000004 };
    static constexpr uint32_t REG_MDM_IDR { 0x010000FC };

    // MDM-AP status bits
    static constexpr uint32_t REG_MDM_STATUS_FLASH_READY { 1 << 1 };
    static constexpr uint32_t REG_MDM_STATUS_SYS_SECURITY { 1 << 2 };
    static constexpr uint32_t REG_MDM_STATUS_SYS_NRESET { 1 << 3 };

    // MDM-AP control bits
    static constexpr uint32_t REG_MDM_CONTROL_SYS_RESET_REQ { 1 << 3 };
    static constexpr uint32_t REG_MDM_CONTROL_CORE_HOLD_RESET { 1 << 4 };
};

#endif /* ARM_KINETIS_DEBUG_H_ */
