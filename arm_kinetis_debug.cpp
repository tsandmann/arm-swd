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

#include "arm_kinetis_debug.h"


ARMKinetisDebug::ARMKinetisDebug(const uint8_t clockPin, const uint8_t dataPin, const LogLevel logLevel) : ARMDebug { clockPin, dataPin, logLevel } {}

bool ARMKinetisDebug::detect() {
    // Make sure we're on a compatible chip. The MDM-AP peripheral is Freescale-specific.
    uint32_t idr;
    if (!apRead(REG_MDM_IDR, idr)) {
        log(LOG_ERROR, "ARMKinetisDebug::detect(): apRead failed.");
        return false;
    }

    /*
     * This needs verification, but I think the low half is a version number.
     * The K20 CPU from Fadecandy has an IDR of 0x001c0000,
     * but the Cortex-M0 based KE04Z has an IDR of 0x001c0020. Okay then!
     */
    if ((idr & 0xffff0000) != 0x001C0000) {
        log(LOG_ERROR, "ARMKinetisDebug::detect(): Didn't find a supported MDM-AP peripheral");
        return false;
    }

    return true;
}

bool ARMKinetisDebug::reset(const bool check_sec_bit) {
    log(LOG_TRACE_MEM, "ARMKinetisDebug::reset()...");
    // System resets can be slow, give them more time than the default.
    const unsigned resetRetries = 2000;

    // Put the control register in a known state, and make sure we aren't already in the middle of a reset
    uint32_t status;
    if (!apWrite(REG_MDM_CONTROL, REG_MDM_CONTROL_CORE_HOLD_RESET)) {
        return false;
    }
    if (!apReadPoll(REG_MDM_STATUS, status, REG_MDM_STATUS_SYS_NRESET, -1, resetRetries)) {
        return false;
    }

    // System reset
    if (!apWrite(REG_MDM_CONTROL, REG_MDM_CONTROL_SYS_RESET_REQ)) {
        return false;
    }
    if (!apReadPoll(REG_MDM_STATUS, status, REG_MDM_STATUS_SYS_NRESET, 0)) {
        return false;
    }
    if (!apWrite(REG_MDM_CONTROL, 0)) {
        return false;
    }

    // Wait until the flash controller is ready & system is out of reset.
    // Also wait for security bit to be cleared. Early in reset, the chip is determining
    // its security status. When the security bit is set, AHB-AP is disabled.
    if (!apReadPoll(REG_MDM_STATUS, status, REG_MDM_STATUS_SYS_NRESET | REG_MDM_STATUS_FLASH_READY | (check_sec_bit ? REG_MDM_STATUS_SYS_SECURITY : 0),
            REG_MDM_STATUS_SYS_NRESET | REG_MDM_STATUS_FLASH_READY, resetRetries)) {
        return false;
    }

    log(LOG_TRACE_MEM, "ARMKinetisDebug::reset() done.");

    return true;
}

bool ARMKinetisDebug::sys_reset_request() {
    log(LOG_TRACE_MEM, "ARMKinetisDebug::sys_reset_request()...");

    if (!apWrite(REG_MDM_CONTROL, REG_MDM_CONTROL_SYS_RESET_REQ)) {
        return false;
    }

    log(LOG_TRACE_MEM, "ARMKinetisDebug::sys_reset_request() done.");

    return true;
}
