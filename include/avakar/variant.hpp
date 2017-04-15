#ifndef AVAKAR_VARIANT_HPP
#define AVAKAR_VARIANT_HPP

#include "../../src/variant_fwd.hpp"
#include "../../src/variant_alternative.hpp"
#include "../../src/variant_size.hpp"
#include <type_traits>
#include <initializer_list>

#include "../../src/variant_helpers.hpp"

#include <exception>

namespace avakar {

template <size_t I>
struct in_place_index_t
{
};

template <typename T>
struct in_place_type_t
{
};

struct bad_variant_access
	: std::exception
{
};

template <typename... Types>
struct variant
{
	template <typename = std::enable_if_t<std::is_default_constructible<variant_alternative_t<0, variant>>::value>>
	variant();

	variant(variant const & o);
	variant(variant && o);

	variant & operator=(variant const & o);
	variant & operator=(variant && o);

	template <typename T, typename... Args,
		typename = std::enable_if_t<detail::variant_index<T, Types...>::unique>>
	explicit variant(in_place_type_t<T>, Args &&... args);

	template <typename T, typename U, typename... Args,
		typename = std::enable_if_t<detail::variant_index<T, Types...>::unique>>
	explicit variant(in_place_type_t<T>, std::initializer_list<U> il, Args &&... args);

	template <size_t I, typename... Args>
	explicit variant(in_place_index_t<I>, Args &&... args);

	template <size_t I, typename U, typename... Args>
	explicit variant(in_place_index_t<I>, std::initializer_list<U> il, Args &&... args);

	~variant();

	template <typename T, typename... Args,
		typename = std::enable_if_t<detail::variant_index<T, Types...>::unique>>
	T & emplace(Args &&... args);

	template <typename T, typename U, typename... Args,
		typename = std::enable_if_t<detail::variant_index<T, Types...>::unique>>
	T & emplace(std::initializer_list<U> il, Args &&... args);

	template <size_t I, typename... Args>
	auto emplace(Args &&... args) -> variant_alternative_t<I, variant> &;

	template <size_t I, typename U, typename... Args>
	auto emplace(std::initializer_list<U> il, Args &&... args) -> variant_alternative_t<I, variant> &;

	size_t index() const noexcept;
	bool valueless_by_exception() const noexcept;

private:
	size_t index_;
	std::aligned_union_t<0, Types...> storage_;

	friend detail::variant_storage_accessor<Types...>;
};

template <std::size_t I, typename... Types>
auto get(variant<Types...> & v)
	-> variant_alternative_t<I, variant<Types...>> &;

template <std::size_t I, typename... Types>
auto get(variant<Types...> const & v)
	-> variant_alternative_t<I, variant<Types...>> const &;

template <std::size_t I, typename... Types>
auto get(variant<Types...> && v)
	-> variant_alternative_t<I, variant<Types...>> &&;

template <std::size_t I, typename... Types>
auto get(variant<Types...> const && v)
	-> variant_alternative_t<I, variant<Types...>> const &&;

template <typename T, typename... Types>
T & get(variant<Types...> & v);

template <typename T, typename... Types>
T const & get(variant<Types...> const & v);

template <typename T, typename... Types>
T && get(variant<Types...> && v);

template <typename T, typename... Types>
T const && get(variant<Types...> const && v);

template <typename Visitor, typename Variant>
auto visit(Visitor && vis, Variant && var);

}

#include "../../src/variant_impl.hpp"

#endif // AVAKAR_VARIANT_HPP
