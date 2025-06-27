#pragma once

#include <string>

namespace builtin {
    enum BuiltInKind {
        PrintFunc,
        None,
    };
    bool is_builtin(const std::string& name);
    BuiltInKind get_builtin(const std::string& name);
}
