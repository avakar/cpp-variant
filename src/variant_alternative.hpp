#ifndef AVAKAR_VARIANT_VARIANT_ALTERNATIVE_HPP
#define AVAKAR_VARIANT_VARIANT_ALTERNATIVE_HPP

#include "variant_fwd.hpp"
#include <type_traits>

namespace avakar {

template <size_t I, typename T>
struct variant_alternative<I, T const>
{
	using type = variant_alternative_t<I, T> const;
};

template <size_t I, typename T>
struct variant_alternative<I, T volatile>
{
	using type = variant_alternative_t<I, T> volatile;
};

template <size_t I, typename T>
struct variant_alternative<I, T const volatile>
{
	using type = variant_alternative_t<I, T> const volatile;
};

template <typename T0, typename... Types>
struct variant_alternative<0, variant<T0, Types...>>
{
	using type = T0;
};

template <size_t I, typename T0, typename... Types>
struct variant_alternative<I, variant<T0, Types...>>
{
	using type = variant_alternative_t<I - 1, variant<Types...>>;
};

}

#endif // AVAKAR_VARIANT_VARIANT_ALTERNATIVE_HPP
