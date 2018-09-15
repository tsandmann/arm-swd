/*
 * Simple ARM debug interface for Arduino, using the SWD (Serial Wire Debug) port.
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

#ifndef ARM_DEBUG_H_
#define ARM_DEBUG_H_

#include <cstdint>


class ARMDebug {
public:
    enum LogLevel { LOG_NONE = 0, LOG_ERROR, LOG_NORMAL, LOG_TRACE_MEM, LOG_TRACE_AP, LOG_TRACE_DP, LOG_TRACE_SWD, LOG_MAX };

    ARMDebug(const uint8_t clockPin, const uint8_t dataPin, const LogLevel logLevel = LOG_NORMAL);

    // Higher level API

    /**
     * Reinitialize the debug interface, and identify the connected chip.
     * This resets the target chip, putting it in SWD mode and logging its
     * identity.
     *
     * Returns true on success, logs and returns false on error.
     */
    bool begin();

    // CPU register operations, when halted (via DCRSR)
    bool regWrite(unsigned num, uint32_t data);
    bool regRead(unsigned num, uint32_t& data);

    // Memory operations (AHB bus)
    bool memStore(uint32_t addr, uint32_t data);
    bool memStore(uint32_t addr, const uint32_t* data, unsigned count);
    bool memLoad(uint32_t addr, uint32_t& data);
    bool memLoad(uint32_t addr, uint32_t* data, unsigned count);

    // Write with verify
    bool memStoreAndVerify(uint32_t addr, uint32_t data);
    bool memStoreAndVerify(uint32_t addr, const uint32_t* data, unsigned count);

    // Byte load/store operations (AHB bus)
    bool memStoreByte(uint32_t addr, uint8_t data);
    bool memLoadByte(uint32_t addr, uint8_t& data);

    // Halfword (16-bit) load/store operations (AHB bus)
    bool memStoreHalf(uint32_t addr, uint16_t data);
    bool memLoadHalf(uint32_t addr, uint16_t& data);

    // Poll for an expected value
    bool memPoll(unsigned addr, uint32_t& data, uint32_t mask, uint32_t expected, unsigned retries = DEFAULT_RETRIES);
    bool memPollByte(unsigned addr, uint8_t& data, uint8_t mask, uint8_t expected, unsigned retries = DEFAULT_RETRIES);

    // Write to the log, printf-style
    void log(int level, const char* fmt, ...) const;

    // Change log levels, optionally returning the old level so it can be restored.
    void setLogLevel(LogLevel newLevel);
    void setLogLevel(LogLevel newLevel, LogLevel& prevLevel);

    // Lower level API

    // Low-level wire interface (LSB-first)
    void wireWrite(uint32_t data, unsigned nBits) const;
    uint32_t wireRead(unsigned nBits) const;
    void wireWriteTurnaround() const;
    void wireReadTurnaround() const;
    void wireWriteIdle() const;

    // Error diagnostics and recovert
    bool handleFault();
    bool dumpMemPortRegisters();

    // Packet assembly tools
    static constexpr uint8_t packHeader(unsigned addr, bool APnDP, bool RnW) {
        bool a2 = (addr >> 2) & 1;
        bool a3 = (addr >> 3) & 1;
        bool parity = APnDP ^ RnW ^ a2 ^ a3;
        return (1 << 0) | // Start
            (APnDP << 1) | (RnW << 2) | ((addr & 0xC) << 1) | (parity << 5) | (1 << 7); // Park
    }

    static constexpr bool evenParity(uint32_t word) {
        word = (word & 0xFFFF) ^ (word >> 16);
        word = (word & 0xFF) ^ (word >> 8);
        word = (word & 0xF) ^ (word >> 4);
        word = (word & 0x3) ^ (word >> 2);
        word = (word & 0x1) ^ (word >> 1);
        return word;
    }

    // Debug core register handshaking
    bool regTransactionHandshake();

    // Debug port layer
    bool dpWrite(unsigned addr, bool APnDP, uint32_t data);
    bool dpRead(unsigned addr, bool APnDP, uint32_t& data);
    bool dpSelect(unsigned addr);

    // Access port layer
    bool apWrite(unsigned addr, uint32_t data);
    bool apRead(unsigned addr, uint32_t& data);

    // Internal MEM-AP functions
    bool memWait();
    bool memWriteCSW(uint32_t data);

    // Poll for an expected value
    bool dpReadPoll(unsigned addr, uint32_t& data, uint32_t mask, uint32_t expected, unsigned retries = DEFAULT_RETRIES);
    bool apReadPoll(unsigned addr, uint32_t& data, uint32_t mask, uint32_t expected, unsigned retries = DEFAULT_RETRIES);

    // Individual initialization steps (already included in begin)
    bool getIDCODE(uint32_t& idcode);
    bool debugPortPowerup();
    bool initMemPort();

    // Debug port registers
    enum DebugPortReg { ABORT = 0x0, IDCODE = 0x0, CTRLSTAT = 0x4, SELECT = 0x8, RDBUFF = 0xC };

    // CTRL/STAT bits
    enum CtrlStatBit {
        CSYSPWRUPACK = 1 << 31,
        CSYSPWRUPREQ = 1 << 30,
        CDBGPWRUPACK = 1 << 29,
        CDBGPWRUPREQ = 1 << 28,
        CDBGRSTACK = 1 << 27,
        CDBGRSTREQ = 1 << 26,
    };

    // Memory Access Port registers
    enum MemPortReg {
        MEM_CSW = 0x00,
        MEM_TAR = 0x04,
        MEM_DRW = 0x0C,
        MEM_IDR = 0xFC,
    };

    // MEM-AP CSW bits
    enum MemCSWBit {
        CSW_8BIT = 0,
        CSW_16BIT = 1,
        CSW_32BIT = 2,
        CSW_ADDRINC_OFF = 0,
        CSW_ADDRINC_SINGLE = 1 << 4,
        CSW_ADDRINC_PACKED = 2 << 4,
        CSW_DEVICE_EN = 1 << 6,
        CSW_TRIN_PROG = 1 << 7,
        CSW_SPIDEN = 1 << 23,
        CSW_HPROT = 1 << 25,
        CSW_MASTER_DEBUG = 1 << 29,
        CSW_SPROT = 1 << 30,
        CSW_DBGSWENABLE = 1 << 31,
    };

    static constexpr unsigned CSW_DEFAULTS { static_cast<unsigned>(CSW_DBGSWENABLE | CSW_MASTER_DEBUG | CSW_HPROT) };
    static constexpr unsigned DEFAULT_RETRIES { 50 };

protected:
    static constexpr uint32_t REG_SCB_DHCSR { 0xE000EDF0 }; // Debug Halting Control and Status Register
    static constexpr uint32_t REG_SCB_DCRSR { 0xE000EDF4 }; // Debug Core Register Selector Register
    static constexpr uint32_t REG_SCB_DCRDR { 0xE000EDF8 }; // Debug Core Register Data Register

private:
    const uint8_t clockPin, dataPin;
    LogLevel logLevel;

    // Cached versions of ARM debug registers
    struct {
        uint32_t select;
        uint32_t csw;
    } cache;

    // Relevant bits in cache.select
    static constexpr uint32_t kSelectMask { 0xFF0000F0 };
};

#endif /* ARM_DEBUG_H_ */
