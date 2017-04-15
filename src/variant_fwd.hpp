#ifndef AVAKAR_VARIANT_VARIANT_FWD_HPP
#define AVAKAR_VARIANT_VARIANT_FWD_HPP

#include <stdlib.h>

namespace avakar {

template <typename... Types>
struct variant;

template <size_t I, class T>
struct variant_alternative;

template <size_t I, class T>
using variant_alternative_t = typename variant_alternative<I, T>::type;

constexpr size_t variant_npos = -1;

template <typename T, typename... Types>
bool holds_alternative(variant<Types...> const & v) noexcept;

}

#endif // AVAKAR_VARIANT_VARIANT_FWD_HPP
