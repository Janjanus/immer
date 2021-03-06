//
// immer - immutable data structures for C++
// Copyright (C) 2016 Juan Pedro Bolivar Puente
//
// This file is part of immer.
//
// immer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// immer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with immer.  If not, see <http://www.gnu.org/licenses/>.
//

#include <immer/array.hpp>

#include <doctest.h>
#include <boost/range/adaptors.hpp>

#include <algorithm>
#include <numeric>
#include <vector>

using namespace immer;

TEST_CASE("instantiation")
{
    auto v = array<int>{};
    CHECK(v.size() == 0u);
}

TEST_CASE("push back one element")
{
    SUBCASE("one element")
    {
        const auto v1 = array<int>{};
        auto v2 = v1.push_back(42);
        CHECK(v1.size() == 0u);
        CHECK(v2.size() == 1u);
        CHECK(v2[0] == 42);
    }

    SUBCASE("many elements")
    {
        const auto n = 666u;
        auto v = array<unsigned>{};
        for (auto i = 0u; i < n; ++i) {
            v = v.push_back(i * 42);
            CHECK(v.size() == i + 1);
            for (auto j = 0u; j < v.size(); ++j)
                CHECK(v[j] == j * 42);
        }
    }
}

TEST_CASE("update")
{
    const auto n = 42;
    auto v = array<unsigned>{};
    for (auto i = 0u; i < n; ++i)
        v = v.push_back(i);

    SUBCASE("set")
    {
        const auto u = v.set(3u, 13u);
        CHECK(u.size() == v.size());
        CHECK(u[2u] == 2u);
        CHECK(u[3u] == 13u);
        CHECK(u[4u] == 4u);
        CHECK(u[40u] == 40u);
        CHECK(v[3u] == 3u);
    }

    SUBCASE("update")
    {
        const auto u = v.update(10u, [] (auto x) { return x + 10; });
        CHECK(u.size() == v.size());
        CHECK(u[10u] == 20u);
        CHECK(v[40u] == 40u);

        const auto w = v.update(40u, [] (auto x) { return x - 10; });
        CHECK(w.size() == v.size());
        CHECK(w[40u] == 30u);
        CHECK(v[40u] == 40u);
    }
}

TEST_CASE("iterator")
{
    const auto n = 666u;
    auto v = array<unsigned>{};
    for (auto i = 0u; i < n; ++i)
        v = v.push_back(i);

    SUBCASE("works with range loop")
    {
        auto i = 0u;
        for (const auto& x : v)
            CHECK(x == i++);
        CHECK(i == v.size());
    }

    SUBCASE("works with standard algorithms")
    {
        auto s = std::vector<unsigned>(n);
        std::iota(s.begin(), s.end(), 0u);
        std::equal(v.begin(), v.end(), s.begin(), s.end());
    }

    SUBCASE("can go back from end")
    {
        CHECK(n-1 == *--v.end());
    }

    SUBCASE("works with reversed range adaptor")
    {
        auto r = v | boost::adaptors::reversed;
        auto i = n;
        for (const auto& x : r)
            CHECK(x == --i);
    }

    SUBCASE("works with strided range adaptor")
    {
        auto r = v | boost::adaptors::strided(5);
        auto i = 0u;
        for (const auto& x : r)
            CHECK(x == 5 * i++);
    }

    SUBCASE("works reversed")
    {
        auto i = n;
        for (auto iter = v.rbegin(), last = v.rend(); iter != last; ++iter)
            CHECK(*iter == --i);
    }

    SUBCASE("advance and distance")
    {
        auto i1 = v.begin();
        auto i2 = i1 + 100;
        CHECK(*i2 == 100u);
        CHECK(i2 - i1 == 100);
        CHECK(*(i2 - 50) == 50u);
        CHECK((i2 - 30) - i2 == -30);
    }
}
