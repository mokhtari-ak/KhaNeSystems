#pragma once

#include <string>
#include <map>
#include <variant>

namespace khane::navigation {

using ParamValue = std::variant<float, int32_t>;

struct ParamMetadata {
    ParamValue min;
    ParamValue max;
    ParamValue default_val;
};

class ParameterServer {
public:
    ParameterServer() = default;

    void register_param(const std::string& name, ParamMetadata meta);
    bool set_param(const std::string& name, ParamValue value);
    ParamValue get_param(const std::string& name) const;

private:
    std::map<std::string, ParamMetadata> registry_;
    std::map<std::string, ParamValue> values_;
};

} // namespace khane::navigation
