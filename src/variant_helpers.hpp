#include <new>
#include <type_traits>
#include <utility>

namespace avakar {
namespace detail {

template <typename... Types>
struct variant_storage_accessor
{
	static void * get(variant<Types...> & v);
	static void const * get(variant<Types...> const & v);
};

template <typename T>
void copy_at(void * storage, T const & v)
{
	new(storage) T(v);
}

template <typename T>
void move_at(void * storage, T && v)
{
	new(storage) T(std::move(v));
}

template <typename T>
void destroy_at(T * p)
{
	p->~T();
}

template <typename F, typename... Args>
auto invoke(F && f, Args &&... args)
{
	return std::forward<F>(f)(std::forward<Args>(args)...);
}

template <size_t I, typename T, typename... Types>
struct variant_index_impl;

template <size_t I, typename T>
struct variant_index_impl<I, T>
{
	static constexpr size_t first_index = variant_npos;
	static constexpr size_t count = 0;
};

template <size_t I, typename T, typename T0, typename... Types>
struct variant_index_impl<I, T, T0, Types...>
{
	static constexpr size_t first_index
		= std::is_same<T, T0>::value? I: variant_index_impl<I + 1, T, Types...>::first_index;

	static constexpr size_t count = variant_index_impl<I + 1, T, Types...>::count + std::is_same<T, T0>::value;
};

template <typename T, typename... Types>
struct variant_index
{
	static constexpr size_t first_index
		= variant_index_impl<0, T, Types...>::first_index;

	static constexpr bool unique
		= variant_index_impl<0, T, Types...>::count == 1;

	static bool is_valid(size_t idx)
	{
		static bool const valid_indexes[] = { std::is_same<T, Types>::value... };
		return idx < sizeof...(Types) && valid_indexes[idx];
	}
};

}
}
