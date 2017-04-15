#include <new>
#include "variant_visit.hpp"

template <typename... Types>
void * avakar::detail::variant_storage_accessor<Types...>::get(variant<Types...> & v)
{
	return &v.storage_;
}

template <typename... Types>
void const * avakar::detail::variant_storage_accessor<Types...>::get(variant<Types...> const & v)
{
	return &v.storage_;
}

template <typename... Types>
template <typename>
avakar::variant<Types...>::variant()
	: index_(0)
{
	new(&storage_) variant_alternative_t<0, variant>();
}

template <typename... Types>
avakar::variant<Types...>::variant(variant const & o)
	: index_(o.index())
{
	visit([this](auto const & v) { detail::copy_at(&storage_, v); }, o);
}

template <typename... Types>
avakar::variant<Types...>::variant(variant && o)
	: index_(o.index())
{
	visit([this](auto && v) { detail::move_at(&storage_, std::move(v)); }, o);
}

template <typename... Types>
avakar::variant<Types...> & avakar::variant<Types...>::operator=(variant const & o)
{
	visit([](auto & v) { detail::destroy_at(&v); }, *this);
	index_ = variant_npos;

	visit([this](auto const & v) { detail::copy_at(&storage_, v); }, o);
	index_ = o.index();

	return *this;
}

template <typename... Types>
avakar::variant<Types...> & avakar::variant<Types...>::operator=(variant && o)
{
	visit([](auto & v) { detail::destroy_at(&v); }, *this);
	index_ = variant_npos;

	visit([this](auto && v) { detail::move_at(&storage_, std::move(v)); }, o);
	index_ = o.index();

	return *this;
}

template <typename... Types>
template <typename T, typename... Args, typename>
avakar::variant<Types...>::variant(in_place_type_t<T>, Args &&... args)
	: variant(in_place_index_t<detail::variant_index<T, Types...>::first_index>(), std::forward<Args>(args)...)
{
}

template <typename... Types>
template <typename T, typename U, typename... Args, typename>
avakar::variant<Types...>::variant(in_place_type_t<T>, std::initializer_list<U> il, Args &&... args)
	: variant(in_place_index_t<detail::variant_index<T, Types...>::value>(), il, std::forward<Args>(args)...)
{
}

template <typename... Types>
template <size_t I, typename... Args>
avakar::variant<Types...>::variant(in_place_index_t<I>, Args &&... args)
	: index_(I)
{
	new(&storage_) variant_alternative_t<I, variant>(std::forward<Args>(args)...);
}

template <typename... Types>
template <size_t I, typename U, typename... Args>
avakar::variant<Types...>::variant(in_place_index_t<I>, std::initializer_list<U> il, Args &&... args)
	: index_(I)
{
	new(&storage_) variant_alternative_t<I, variant>(il, std::forward<Args>(args)...);
}

template <typename... Types>
avakar::variant<Types...>::~variant()
{
	visit([](auto & v) { detail::destroy_at(&v); }, *this);
}

template <typename... Types>
template <typename T, typename... Args, typename>
T & avakar::variant<Types...>::emplace(Args &&... args)
{
	return this->emplace<detail::variant_index<T, Types...>::first_index>(std::forward<Args>(args)...);
}

template <typename... Types>
template <typename T, typename U, typename... Args, typename>
T & avakar::variant<Types...>::emplace(std::initializer_list<U> il, Args &&... args)
{
	return this->emplace<detail::variant_index<T, Types...>::first_index>(il, std::forward<Args>(args)...);
}

template <typename... Types>
template <size_t I, typename... Args>
auto avakar::variant<Types...>::emplace(Args &&... args)
	-> variant_alternative_t<I, variant> &
{
	visit([](auto & v) { detail::destroy_at(&v); }, *this);
	index_ = variant_npos;

	auto * r = new(&storage_) variant_alternative_t<I, variant>(std::forward<Args>(args)...);
	index_ = I;

	return *r;
}

template <typename... Types>
template <size_t I, typename U, typename... Args>
auto avakar::variant<Types...>::emplace(std::initializer_list<U> il, Args &&... args)
	-> variant_alternative_t<I, variant> &
{
	visit([](auto & v) { detail::destroy_at(&v); }, *this);
	index_ = variant_npos;

	auto * r = new(&storage_) variant_alternative_t<I, variant>(il, std::forward<Args>(args)...);
	index_ = I;

	return *r;
}

template <typename... Types>
size_t avakar::variant<Types...>::index() const noexcept
{
	return index_;
}

template <typename... Types>
bool avakar::variant<Types...>::valueless_by_exception() const noexcept
{
	return index_ == variant_npos;
}

template <std::size_t I, typename... Types>
auto avakar::get(variant<Types...> & v)
	-> variant_alternative_t<I, variant<Types...>> &
{
	if (v.index() != I)
		throw bad_variant_access();
	return *static_cast<variant_alternative_t<I, variant<Types...>> *>(
		detail::variant_storage_accessor<Types...>::get(v));
}

template <std::size_t I, typename... Types>
auto avakar::get(variant<Types...> const & v)
	-> variant_alternative_t<I, variant<Types...>> const &
{
	if (v.index() != I)
		throw bad_variant_access();
	return *static_cast<variant_alternative_t<I, variant<Types...>> const *>(
		detail::variant_storage_accessor<Types...>::get(v));
}

template <std::size_t I, typename... Types>
auto avakar::get(variant<Types...> && v)
	-> variant_alternative_t<I, variant<Types...>> &&
{
	if (v.index() != I)
		throw bad_variant_access();
	return std::move(*static_cast<variant_alternative_t<I, variant<Types...>> *>(
		detail::variant_storage_accessor<Types...>::get(v)));
}

template <std::size_t I, typename... Types>
auto avakar::get(variant<Types...> const && v)
	-> variant_alternative_t<I, variant<Types...>> const &&
{
	if (v.index() != I)
		throw bad_variant_access();
	return std::move(*static_cast<variant_alternative_t<I, variant<Types...>> const *>(
		detail::variant_storage_accessor<Types...>::get(v)));
}

template <typename T, typename... Types>
T & avakar::get(variant<Types...> & v)
{
	if (!detail::variant_index<T, Types...>::is_valid(v.index()))
		throw bad_variant_access();
	return *static_cast<T *>(
		detail::variant_storage_accessor<Types...>::get(v));
}

template <typename T, typename... Types>
T const & avakar::get(variant<Types...> const & v)
{
	if (!detail::variant_index<T, Types...>::is_valid(v.index()))
		throw bad_variant_access();
	return *static_cast<T const *>(
		detail::variant_storage_accessor<Types...>::get(v));
}

template <typename T, typename... Types>
T && avakar::get(variant<Types...> && v)
{
	if (!detail::variant_index<T, Types...>::is_valid(v.index()))
		throw bad_variant_access();
	return std::move(*static_cast<T *>(
		detail::variant_storage_accessor<Types...>::get(v)));
}

template <typename T, typename... Types>
T const && avakar::get(variant<Types...> const && v)
{
	if (!detail::variant_index<T, Types...>::is_valid(v.index()))
		throw bad_variant_access();
	return std::move(*static_cast<T const *>(
		detail::variant_storage_accessor<Types...>::get(v)));
}

template <typename Visitor, typename Variant>
auto avakar::visit(Visitor && vis, Variant && var)
{
	return detail::variant_indexed_visitor<Visitor, Variant, std::make_index_sequence<variant_size_v<std::remove_reference_t<Variant>>>>::visit(
		std::forward<Visitor>(vis), std::forward<Variant>(var));
}
