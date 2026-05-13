#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <vector>

namespace Tests::Hooks::I2c {

enum class I2cHookFault {
    None,
    Timeout,
    BusError
};

struct I2cTransaction {
    std::uint8_t address = 0;
    std::vector<std::uint8_t> tx;
    std::vector<std::uint8_t> rx;
    bool success = true;
    I2cHookFault fault = I2cHookFault::None;
};

struct I2cTransferResult {
    bool success = false;
    I2cHookFault fault = I2cHookFault::None;
    std::size_t rx_size = 0;
};

class I2cHookDriver {
public:
    using FaultType = I2cHookFault;
    using TransferResult = I2cTransferResult;

    void init(std::uint32_t bus_hz) noexcept {
        bus_hz_ = bus_hz;
        initialized_ = true;
    }

    void scriptTransaction(const I2cTransaction& transaction) {
        scripted_.push_back(transaction);
    }

    TransferResult transfer(std::uint8_t address,
                            const std::uint8_t* tx_data,
                            std::size_t tx_size,
                            std::uint8_t* rx_data,
                            std::size_t rx_size) {
        I2cTransaction observed{};
        observed.address = address;
        observed.tx.assign(tx_data, tx_data + tx_size);

        I2cTransferResult result{};
        if (!initialized_ || scripted_.empty()) {
            observed.success = false;
            observed.fault = I2cHookFault::BusError;
            observed_log_.push_back(observed);
            result.fault = observed.fault;
            return result;
        }

        const I2cTransaction expected = scripted_.front();
        scripted_.pop_front();

        observed.rx = expected.rx;
        observed.success = expected.success;
        observed.fault = expected.fault;
        observed_log_.push_back(observed);

        if (!expected.success || expected.fault != I2cHookFault::None) {
            result.success = false;
            result.fault = expected.fault;
            return result;
        }

        const std::size_t copy_size = (rx_size < expected.rx.size()) ? rx_size : expected.rx.size();
        for (std::size_t i = 0; i < copy_size; ++i) {
            rx_data[i] = expected.rx[i];
        }

        result.success = true;
        result.fault = I2cHookFault::None;
        result.rx_size = copy_size;
        return result;
    }

    const std::vector<I2cTransaction>& observedLog() const noexcept { return observed_log_; }

    std::size_t remainingScriptedTransactions() const noexcept { return scripted_.size(); }

private:
    std::uint32_t bus_hz_ = 0;
    bool initialized_ = false;
    std::deque<I2cTransaction> scripted_{};
    std::vector<I2cTransaction> observed_log_{};
};

} // namespace Tests::Hooks::I2c
