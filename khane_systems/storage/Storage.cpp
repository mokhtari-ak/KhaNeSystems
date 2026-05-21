#include "Storage.hpp"
#include <algorithm>
#include <cstring>

namespace khane::storage {

Storage::Storage(hw::ISdCardRawDriver& driver) 
    : driver_(driver), 
      active_buffer_(buffer_a_.data()), 
      write_buffer_(buffer_b_.data()) {}

bool Storage::log(const uint8_t* data, size_t size) {
    if (offset_ + size > BUFFER_SIZE) {
        if (write_pending_) {
            return false; // Buffer overflow, drop data
        }
        // Swap buffers
        std::swap(active_buffer_, write_buffer_);
        write_pending_ = true;
        offset_ = 0;
    }

    std::memcpy(active_buffer_ + offset_, data, size);
    offset_ += size;
    return true;
}

void Storage::process() {
    if (write_pending_ && driver_.is_ready()) {
        if (driver_.write_block(current_sector_++, write_buffer_)) {
            write_pending_ = false;
        }
    }
}

} // namespace khane::storage
