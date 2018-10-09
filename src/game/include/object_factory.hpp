// object_factory.hpp

#pragma once

#include <functional>
#include <memory>

#include "abstract_object.hpp"
#include "button.hpp"

namespace deep_space {
std::shared_ptr<levi::abstract_object>
create_object(std::string type, levi::params params,
              callback callback = nullptr);
};
