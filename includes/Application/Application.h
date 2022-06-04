#pragma once

#include <memory>

namespace Cerelatis {

class Application {
    // interface
public:
    // constructors
public:
    static std::unique_ptr<Application> create();
};

} // namespace Cerelatis