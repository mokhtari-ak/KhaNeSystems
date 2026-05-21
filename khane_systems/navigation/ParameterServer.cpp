#include "ParameterServer.hpp"
#include <algorithm>

namespace khane::navigation {

void ParameterServer::register_param(const std::string& name, ParamMetadata meta) {
    registry_[name] = meta;
    values_[name] = meta.default_val;
}

bool ParameterServer::set_param(const std::string& name, ParamValue value) {
    if (registry_.find(name) == registry_.end()) return false;
    
    auto& meta = registry_[name];
    
    // Simple validation (works for float or int via std::visit)
    if (std::holds_alternative<float>(value)) {
        float v = std::get<float>(value);
        if (v < std::get<float>(meta.min) || v > std::get<float>(meta.max)) return false;
    } else {
        int32_t v = std::get<int32_t>(value);
        if (v < std::get<int32_t>(meta.min) || v > std::get<int32_t>(meta.max)) return false;
    }

    values_[name] = value;
    return true;
}

ParamValue ParameterServer::get_param(const std::string& name) const {
    return values_.at(name);
}

} // namespace khane::navigation
