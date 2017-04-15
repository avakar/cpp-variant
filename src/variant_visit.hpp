#ifndef AVAKAR_VARIANT_VISIT_HPP
#define AVAKAR_VARIANT_VISIT_HPP

namespace avakar {
namespace detail {

template <size_t I, typename Visitor, typename Variant>
struct variant_one_visitor
{
	using return_type = decltype(invoke(std::declval<Visitor>(), get<I>(std::declval<Variant>())));

	static return_type visit(Visitor && vis, Variant && var)
	{
		return invoke(std::forward<Visitor>(vis), get<I>(var));
	}
};

template <typename Visitor, typename Variant, typename Indexes>
struct variant_indexed_visitor;

template <typename Visitor, typename Variant, size_t... I>
struct variant_indexed_visitor<Visitor, Variant, std::index_sequence<I...>>
{
	using return_type = std::common_type_t<typename variant_one_visitor<I, Visitor, Variant>::return_type...>;

	static return_type visit(Visitor && vis, Variant && var)
	{
		using fn_type = return_type(Visitor && vis, Variant && var);

		static fn_type * const getters[] = { &variant_one_visitor<I, Visitor, Variant>::visit... };
		if (var.index() >= sizeof...(I))
			throw bad_variant_access();

		return getters[var.index()](std::forward<Visitor>(vis), std::forward<Variant>(var));
	}
};

}
}

#endif // AVAKAR_VARIANT_VISIT_HPP
