#ifndef AVAKAR_VARIANT_VARIANT_SIZE_HPP
#define AVAKAR_VARIANT_VARIANT_SIZE_HPP

#include "variant_fwd.hpp"
#include <type_traits>

namespace avakar {

template <typename T>
struct variant_size;

template <typename T>
struct variant_size<T const>
	: variant_size<T>
{
};

template <typename T>
struct variant_size<T volatile>
	: variant_size<T>
{
};

template <typename T>
struct variant_size<T const volatile>
	: variant_size<T>
{
};

template <typename... Types>
struct variant_size<variant<Types...>>
	: std::integral_constant<size_t, sizeof...(Types)>
{
};

template <typename T>
constexpr size_t variant_size_v = variant_size<T>::value;

}

#endif // AVAKAR_VARIANT_VARIANT_SIZE_HPP
