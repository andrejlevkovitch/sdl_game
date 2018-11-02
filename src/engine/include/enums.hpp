// dop_clases.hpp

#pragma once

namespace levi {
/**\brief if you need set some type, use types begin value user_type, because,
 * all values from 0 to it is reserved*/
enum object_type { unknown, user_type = 100 };

/**\brief this values can be combinated by | operation*/
enum flip { none = 0, horizontal = 1, vertical = 2 };

enum class id_state { none, menu, playing, pause, over, user_id = 100 };

const float back_plane{0.9};
const float midle_plane{0.5};
const float front_plane{0};
}; // namespace levi
