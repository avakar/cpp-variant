#ifndef AVAKAR_MONOSTATE_HPP
#define AVAKAR_MONOSTATE_HPP

namespace avakar {

struct monostate
{
};

constexpr bool operator==(monostate, monostate) noexcept { return true; }
constexpr bool operator!=(monostate, monostate) noexcept { return false; }
constexpr bool operator<(monostate, monostate) noexcept { return false; }
constexpr bool operator>(monostate, monostate) noexcept { return false; }
constexpr bool operator<=(monostate, monostate) noexcept { return true; }
constexpr bool operator>=(monostate, monostate) noexcept { return true; }

}

#endif // AVAKAR_MONOSTATE_HPP
