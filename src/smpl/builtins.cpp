#include "smpl/builtins.hpp"
#include <string>
#include <unordered_map>


namespace builtin {
    static std::unordered_map<std::string, BuiltInKind> builtins = {
        {"print", PrintFunc},
    };

    bool is_builtin(const std::string& name) {
        return builtins.contains(name);
    }

    BuiltInKind get_builtin(const std::string &name) {
        if (builtins.contains(name)) {
            return builtins.at(name);
        }

        return None;
    }
}
