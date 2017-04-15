#include <variant>
#include <mutest/test.h>

using namespace avakar;

namespace {

struct counting
{
	explicit counting(size_t & counter)
		: counter_(counter)
	{
		++counter;
	}

	~counting()
	{
		--counter_;
	}

	size_t & counter_;
};

struct init_list
{
	init_list(std::initializer_list<int> il)
	{
	}
};

}

TEST("variant<1> can default construct")
{
	avakar::variant<int> v;
	chk v.index() == 0;
	chk v.valueless_by_exception() == false;
	chk get<0>(v) == 0;
}

TEST("variant<2> can default construct")
{
	avakar::variant<int, float> v;
	chk v.index() == 0;
	chk v.valueless_by_exception() == false;
	chk get<0>(v) == 0;
}

TEST("variant<2> with non-default-constructible can default construct")
{
	avakar::variant<int, counting> v;
	chk v.index() == 0;
	chk v.valueless_by_exception() == false;
	chk get<0>(v) == 0;

	// FAILS: avakar::variant<counting, int> v2;
}

TEST("variant supports in-place-index construction")
{
	{
		avakar::variant<int, long> v(avakar::in_place_index_t<0>(), 1);
		chk v.index() == 0;
		chk get<0>(v) == 1;
	}

	{
		avakar::variant<int, long> v(avakar::in_place_index_t<1>(), 2);
		chk v.index() == 1;
		chk get<1>(v) == 2;
	}
}

TEST("variant<int, int> supports in-place-index construction")
{
	{
		avakar::variant<int, int> v(avakar::in_place_index_t<0>(), 1);
		chk v.index() == 0;
		chk get<0>(v) == 1;
	}

	{
		avakar::variant<int, int> v(avakar::in_place_index_t<1>(), 2);
		chk v.index() == 1;
		chk get<1>(v) == 2;
	}

	// FAILS: avakar::variant<int, int> v(avakar::in_place_type_t<int>(), 2);
}

TEST("variant supports in-place-type construction")
{
	{
		avakar::variant<int, long> v(avakar::in_place_type_t<int>(), 1);
		chk v.index() == 0;
		chk get<0>(v) == 1;
	}

	{
		avakar::variant<int, long> v(avakar::in_place_type_t<long>(), 2);
		chk v.index() == 1;
		chk get<1>(v) == 2;
	}
}

TEST("variant correctly copies and moves")
{
	avakar::variant<int, long> v1;
	v1.emplace<long>(2);

	avakar::variant<int, long> v2(v1);
	chk v2.index() == 1;
	chk get<long>(v2) == 2;

	avakar::variant<int, long> v3(std::move(v1));
	chk v2.index() == 1;
	chk get<long>(v2) == 2;
}

TEST("variant correctly assigns")
{
	avakar::variant<int, long> v1, v2, v3;
	v1.emplace<long>(2);

	v2 = v1;
	chk v2.index() == 1;
	chk get<long>(v2) == 2;

	v3 = std::move(v1);
	chk v2.index() == 1;
	chk get<long>(v2) == 2;
}

TEST("variant correctly destroys")
{
	size_t counter = 0;

	{
		avakar::variant<int, counting> v;

		v.emplace<counting>(counter);
		chk counter == 1;

		v.emplace<int>(1);
		chk counter == 0;

		v.emplace<counting>(counter);
		chk counter == 1;
	}

	chk counter == 0;
}

TEST("variant support all get versions")
{
	avakar::variant<int> v;
	avakar::variant<int> const & cv = v;

	int & vref = get<0>(v);
	int const & cvref = get<0>(cv);
	int && vrref = get<0>(std::move(v));
	int const && cvrref = get<0>(std::move(cv));

	int & vref2 = get<int>(v);
	int const & cvref2 = get<int>(cv);
	int && vrref2 = get<int>(std::move(v));
	int const && cvrref2 = get<int>(std::move(cv));

	chk vref == 0;
	chk &cvref == &vref;
	chk &vrref == &vref;
	chk &cvrref == &vref;
	chk &vref2 == &vref;
	chk &cvref2 == &vref;
	chk &vrref2 == &vref;
	chk &cvrref2 == &vref;

	vref = 1;

	chk get<0>(v) == 1;
}

TEST("variant<long, int> throws on invalid get")
{
	avakar::variant<long, int> v;

	chk_exc(avakar::bad_variant_access, get<int>(v));
	chk_exc(avakar::bad_variant_access, get<1>(v));
	chk get<long>(v) == 0;
	chk get<0>(v) == 0;

	v.emplace<1>(1);

	chk get<int>(v) == 1;
	chk get<1>(v) == 1;
	chk_exc(avakar::bad_variant_access, get<long>(v));
	chk_exc(avakar::bad_variant_access, get<0>(v));
}

TEST("variant<int, int> supports get<int>")
{
	avakar::variant<int, int> v;

	chk_exc(avakar::bad_variant_access, get<1>(v));
	chk get<int>(v) == 0;
	chk get<0>(v) == 0;

	v.emplace<1>(1);

	chk get<int>(v) == 1;
	chk get<1>(v) == 1;
	chk_exc(avakar::bad_variant_access, get<0>(v));
}

TEST("variant::emplace<I> works")
{
	avakar::variant<int, long> v;
	chk v.index() == 0;
	chk v.valueless_by_exception() == false;
	chk get<0>(std::move(v)) == 0;

	v.emplace<1>(10);
	chk v.index() == 1;
	chk v.valueless_by_exception() == false;

	v.emplace<0>(10);
	chk v.index() == 0;
	chk v.valueless_by_exception() == false;
}

TEST("variant<int, int>::emplace<I> works")
{
	avakar::variant<int, int> v;
	chk v.index() == 0;
	chk v.valueless_by_exception() == false;
	chk get<0>(std::move(v)) == 0;

	v.emplace<1>(10);
	chk v.index() == 1;
	chk v.valueless_by_exception() == false;

	v.emplace<0>(10);
	chk v.index() == 0;
	chk v.valueless_by_exception() == false;

	// FAILS: v.emplace<int>(1);
}

TEST("variant::emplace<T> works")
{
	avakar::variant<int, long> v;

	v.emplace<int>(1);
	chk v.index() == 0;
	chk get<0>(v) == 1;

	v.emplace<long>(2);
	chk v.index() == 1;
	chk get<1>(v) == 2;
}

TEST("variant::emplace with initializer list")
{
	avakar::variant<int, init_list> v;

	v.emplace<init_list>({ 1, 2, 3 });
	chk v.index() == 1;
}

TEST("variant::visit<2> works")
{
	auto fn = [](auto const & v) {
		return (size_t)(v + 1);
	};

	avakar::variant<int, long> v;
	chk visit(fn, v) == 1;

}
