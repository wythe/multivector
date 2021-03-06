#include "multivectorunit.h"

#include <string>
#include <wythe/multivector.h>

void multivector_unit::empty_multivectors() {
    // default constructor
    wythe::multivector<int> a;
    IT_ASSERT(a.empty());
    IT_ASSERT(a.size() == 0);
    IT_ASSERT(a.root().empty());

    // copy constructable
    wythe::multivector<int> b(a);
    IT_ASSERT(b.empty());
    IT_ASSERT(b.size() == 0);
    IT_ASSERT(b.root().empty());

    // assignable
    wythe::multivector<int> c;
    c = a;

    IT_ASSERT(c.empty());

    // equality
    IT_ASSERT(a == a);
    IT_ASSERT(a == b);
    IT_ASSERT(a == c);
    IT_ASSERT(b == c);

    // clear
    a.clear();
    IT_ASSERT(a.empty());
    IT_ASSERT(a.size() == 0);
    IT_ASSERT(a.root().empty());
}

void multivector_unit::adding() {
    typedef wythe::multivector<int> imultivector;
    {
        wythe::multivector<int> tree;
        tree.root().emplace(42);
        IT_ASSERT(!tree.root().empty());
        IT_ASSERT(tree.size() == 1);
        IT_ASSERT(tree.root().size() == 1);
        
        imultivector::cursor root = tree.root();
        imultivector::cursor first = root.begin();
        IT_ASSERT(*first == 42);

        IT_ASSERT(*tree.root().begin() == 42);
        IT_ASSERT(tree.root().begin().empty());
    }
    { 
        wythe::multivector<int> tree;
        tree.root().emplace(43).emplace(44).emplace(45);
        IT_ASSERT(tree.root().size() == 1);
        IT_ASSERT(tree.root().begin().size() == 1);
        IT_ASSERT(tree.root().begin().begin().size() == 1);
        IT_ASSERT(tree.size() == 3);

        // clear and repeat
        tree.clear();
        tree.root().emplace(43).emplace(44).emplace(45);
        IT_ASSERT(tree.root().size() == 1);
        IT_ASSERT(tree.root().begin().size() == 1);
        IT_ASSERT(tree.root().begin().begin().size() == 1);
        IT_ASSERT(tree.size() == 3);
    }
}

void size_one_test(const wythe::multivector<int> & x) {
    IT_ASSERT(!x.empty());
    IT_ASSERT(x.size() == 1);
    IT_ASSERT(!x.root().empty());
    IT_ASSERT(x.root().size() == 1);
    IT_ASSERT(x.root()[0] == 42);
    IT_ASSERT(*x.root().begin() == 42);
    IT_ASSERT(x.root().begin().empty());
}

void size_one_mutate(wythe::multivector<int> & x) {
    auto i = x.root().begin();
    IT_ASSERT_MSG(*i, *i == 42);
    *i = 21;
    IT_ASSERT_MSG(*i, *i == 21);
    IT_ASSERT(*x.root().begin() == 21);
    *i = 42;
    IT_ASSERT(*x.root().begin() == 42);
}

void size_one_copy_mutate(wythe::multivector<int> x) {
    size_one_mutate(x);
}

void multivector_unit::size_one() {

    // default constructor
    wythe::multivector<int> a;
    a.root().emplace(42);
    size_one_test(a);

    // copy constructable
    wythe::multivector<int> b(a);
    size_one_test(b);

    // assignable
    wythe::multivector<int> c;
    c = a;
    size_one_test(c);

    // equality
    IT_ASSERT(a == a);
    IT_ASSERT(a == b);
    IT_ASSERT(a == c);
    IT_ASSERT(b == c);

    // copy and mutate
    size_one_copy_mutate(a);
    size_one_test(a); // make sure it didn't change

    // mutate
    size_one_mutate(a);
}

void size_two_test(std::string m, wythe::multivector<int> & x) {
    IT_ASSERT_MSG(m, !x.empty());
    IT_ASSERT_MSG(m, x.size() == 2);
    IT_ASSERT_MSG(m, !x.root().empty());
    IT_ASSERT_MSG(m, x.root().size() == 1);
    IT_ASSERT_MSG(m, x.root().begin().size() == 1);
    IT_ASSERT_MSG(m, x.root()[0] == 42);
    IT_ASSERT_MSG(m, x.root().begin()[0] == 43);
    IT_ASSERT_MSG(m, x.root().begin()[0] == 43);
    IT_ASSERT_MSG(m, !x.root().begin().empty());
    IT_ASSERT_MSG(m, x.root().begin().begin().empty());
    IT_ASSERT_MSG(m, x.root().begin().begin().empty());
}

void multivector_unit::size_two() {
    // default constructor
    wythe::multivector<int> a;
    a.root().emplace(42).emplace(43);
    IT_ASSERT(a == a);
    size_two_test("default constructor", a);

    // copy constructable
    wythe::multivector<int> b(a);
    size_two_test("copy constructor", b);

    // assignable
    wythe::multivector<int> c;
    c = a;
    size_two_test("copy constructor", c);

    // equality
    IT_ASSERT(a == b);
    IT_ASSERT(a == c);
    IT_ASSERT(b == c);


}

template <typename T>
void add_recursive(T x, unsigned int n) {
    auto y = x.emplace(n);
    if (n > 0) add_recursive(y, n - 1);
}

void create_complicated(wythe::multivector<int> & tree) {
    add_recursive(tree.root(), 3);
    add_recursive(tree.root(), 3);
    add_recursive(tree.root(), 3);

    tree.root().emplace(33).emplace(34).emplace(35);

    int c = 4;
    for (auto n : {1, 2, 3}) {
        tree.root().emplace(n);
        auto i = tree.root().end() - 1;
        for (int n = 0; n < 3; ++n) {
            i.emplace(c++);
            auto j = i.begin() + i.size() - 1;
            for (int n = 0; n < 2; ++n) j.emplace(n);
        }
    }
}

wythe::multivector<int> create_complicated() {
    wythe::multivector<int> comp;
    create_complicated(comp);
    return comp;
}

void multivector_unit::complicated() {

    wythe::multivector<int> a = create_complicated();

    IT_ASSERT(a.size() == 45);

    //IT_ASSERT(a.root().child_count() == 45);

    // copy constructable
    wythe::multivector<int> b(a);

    IT_ASSERT(!a.empty());
    IT_ASSERT(!b.empty());
    // assignable
    wythe::multivector<int> c;
    c = a;

    IT_ASSERT(!c.empty());

    // equality
    IT_ASSERT(a == b);
    IT_ASSERT(a == c);
    IT_ASSERT(b == c);

    auto x = wythe::compact_string(c);
    IT_ASSERT_MSG(x, x ==
    "3 {2 {1 {0}}} 3 {2 {1 {0}}} 3 {2 {1 {0}}} 33 {34 {35}} 1 {4 {0 1} 5 {0 1} 6 {0 1}} 2 {7 {0 1} 8 {0 1} 9 {0 1}} 3 {10 {0 1} 11 {0 1} 12 {0 1}}");

    c.root()[0] = 1;

    x = wythe::compact_string(c);
    IT_ASSERT_MSG(x, x ==
    "1 {2 {1 {0}}} 3 {2 {1 {0}}} 3 {2 {1 {0}}} 33 {34 {35}} 1 {4 {0 1} 5 {0 1} 6 {0 1}} 2 {7 {0 1} 8 {0 1} 9 {0 1}} 3 {10 {0 1} 11 {0 1} 12 {0 1}}");
}

void multivector_unit::iterators() {
    wythe::multivector<int> tree;
    tree.root().emplace(1);
    tree.root().emplace(2).emplace(10).emplace(100);
    tree.root().emplace(3).emplace(11).emplace(101);
    wythe::verify(tree);

    auto s = wythe::to_text(tree);

    for (auto i = tree.begin(); i != tree.end(); ++i) {
        wythe::multivector<int>::cursor c = i;
        wythe::verify(c);
    }

}

struct boo {
    boo() {
        //IT_WARN("default constructor");
    }
    ~boo() {
        //IT_WARN("destroying " << name);
    }
    boo(int n, const std::string & bits, const std::string & name) : n(n), bits(bits), name(name) {
        //IT_WARN("constructor " << name);
    }
    boo(const boo& b) : n(b.n), bits(b.bits), name(b.name) {
        //IT_WARN("copying " << name);
    }
    int n;
    std::string bits;
    std::string name;
    bool operator==(const boo & b) const {
        return n == b.n && bits == b.bits && name == b.name;
    }
};


std::ostream & operator<<(std::ostream & os, const boo & b) {
    os << b.n << ' ' << b.bits << ' ' << b.name;
    return os;
}

void multivector_unit::non_pod() {
    wythe::multivector<boo> tree;
    //IT_WARN("A");
    tree.root().emplace_back(boo{ 0, "@1", "mark" });
    tree.root().emplace_back(boo{ 1, "@11", "allan" });
    tree.root().emplace_back(boo{ 2, "@111", "beckwith" });
    tree.root().emplace_back(boo{ 3, "@1111", "phil" });
    //IT_WARN("B");
    tree.clear();
    //IT_WARN("C");
    tree.root().emplace(0, "@1", "mark");
    tree.root().emplace(1, "@11", "allan");
    tree.root().emplace(2, "@111", "beckwith");
    tree.root().emplace(3, "@1111", "phil");
    //IT_WARN("D");

    auto i = tree.root().begin();
    IT_ASSERT(i->n == 0);
    IT_ASSERT(i->bits == "@1");
    IT_ASSERT(i->name == "mark");
    IT_ASSERT(*i == (boo{ 0, "@1", "mark" }));

    auto j = i;
    IT_ASSERT(j->n == 0);
    IT_ASSERT(j->bits == "@1");
    IT_ASSERT(j->name == "mark");
    IT_ASSERT(*j == (boo{ 0, "@1", "mark" }));

    auto b = boo{4, "@0", "joe"};
    *j = b;
    IT_ASSERT(j->n == 4);
    IT_ASSERT(j->bits == "@0");
    IT_ASSERT(j->name == "joe");
    IT_ASSERT(*j == (boo{ 4, "@0", "joe" }));

    IT_ASSERT(i->n == 4);
    IT_ASSERT(i->bits == "@0");
    IT_ASSERT(i->name == "joe");
    IT_ASSERT(*i == (boo{ 4, "@0", "joe" }));
    //IT_WARN('\n' << tree);
}

struct Custom {
    Custom() {}
    Custom(const std::string & name, int length, uint64_t value, const std::string & description) :
        name(name), length(length), value(value), description(description) {}
    std::string name;
    int length;
    uint64_t value; 
    std::string description;
    bool operator==(const std::string & n) const { return name == n; }
    bool operator==(const std::string & n) { return name == n; }
};

std::string name_of(const Custom & value) 
{ 
    //IT_WARN("getting name_of");
    return value.name; }

std::string to_string(const Custom & value) { return value.name; }


std::ostream & operator<<(std::ostream & os, const Custom & c) {
    os << c.name << " " << c.length << " " << c.value << " " << c.description;
    return os;
}

void multivector_unit::initializer_list() {
   auto tree = wythe::multivector<int>{ 1, 2, {4, 5, 6, {7, 8, 9}}, 5, {7}, 3 };
   IT_ASSERT(wythe::compact_string(tree) == "1 2 {4 5 6 {7 8 9}} 5 {7} 3");

   auto tree2 = wythe::multivector<int>{ 1, 2, {{4, 5, 6, {7, 8, 9}}}, 5, {7}, 3 };
   IT_ASSERT(wythe::compact_string(tree2) == "1 2 {0 {4 5 6 {7 8 9}}} 5 {7} 3");
}

void multivector_unit::totally_ordered() {
    auto a = wythe::multivector<std::string>{ "a" };
    auto b = wythe::multivector<std::string>{ "b" };
    IT_ASSERT(a < b);
    auto c = wythe::multivector<std::string>{ "a", "b" };
    IT_ASSERT(a < c);
    IT_ASSERT(b > c);

    auto a1 = wythe::multivector<std::string>{ "a", { "b" } };
    IT_ASSERT(a < a1);
    auto a2 = wythe::multivector<std::string>{ "a", { "c" } };
    IT_ASSERT(a1 < a2);
    IT_ASSERT(a2 > a1);
    IT_ASSERT(a2 != a1);
    auto a3 = wythe::multivector<std::string>{ "a", { "b", "c", "d", { "e", "f", "g" } } };
    auto a4 = wythe::multivector<std::string>{ "a", { "b", "c", "d", { "e", "f", "g" } } };
    IT_ASSERT(a3 == a4);
}

void multivector_unit::moving() {
    auto a3 = wythe::multivector<std::string>{ "a", { "b", "c", "d", { "e", "f", "g" } } };
}

template <typename T>
void compare_linear(const wythe::multivector<T> & a, const std::vector<T> b) {
    typedef typename wythe::multivector<T>::const_linear_cursor linear_type;
    auto first = wythe::to_linear(a.begin());
    auto last = wythe::to_linear(a.end());
    IT_ASSERT(std::equal(first, last, b.begin()));
}

void multivector_unit::linear() {
    auto a1 = wythe::multivector<std::string>{ "a", "b", "c" };
    auto v1 = std::vector<std::string>{ "a", "b", "c" };
    compare_linear(a1, v1);

    a1 = wythe::multivector<std::string>{ "a", { "b" }, "c" };
    compare_linear(a1, v1);

    a1 = wythe::multivector<std::string>{ "a", "b", { "c" } };
    compare_linear(a1, v1);

    a1 = wythe::multivector<std::string>{ "a", { "b", "c" } };
    compare_linear(a1, v1);

    a1 = wythe::multivector<std::string>{ "a", { "b", { "c" } } };
    compare_linear(a1, v1);
}

void multivector_unit::promote() {
    {
        auto a = wythe::multivector<int>{ 1, 2, 3, { 10, 11, 12 } };
        auto b = wythe::multivector<int>{ 1, 2, 10, 11, 12 };

        IT_ASSERT(a != b);

        wythe::promote_last(a.root());
        wythe::verify(a);

        IT_ASSERT(a == b);
    }
    {
        auto a = wythe::multivector<int>{ 1, 2, 3, { 10, 11, 12, { 100, 101, 102}} };
        auto b = wythe::multivector<int>{ 1, 2, 10, 11, 12, { 100, 101, 102}};

        IT_ASSERT(a != b);
        a.root().promote_last();
        wythe::verify(a);

        IT_ASSERT_MSG(wythe::to_text(a), a == b);
    }
    {
        auto a = wythe::multivector<int>{ 1, 2, 3, { 10, 11, 12, { 100, 101, 102, { 201, 202, 203 }}} };
        auto b = wythe::multivector<int>{ 1, 2, 10, 11, 12, { 100, 101, 102, { 201, 202, 203 }}};

        IT_ASSERT(a != b);
        a.root().promote_last();
        wythe::verify(a);

        IT_ASSERT_MSG(wythe::to_text(a), a == b);
    }
}

void multivector_unit::precursor() {
    auto m = wythe::multivector<int>{1, {10, { 100, 101, 102}}, 2, 3, 4};
    auto last = wythe::to_precursor(--m.end());
    IT_ASSERT_MSG(*last, *last == 4);
    std::ostringstream os;
    while (!last.is_root()) {
        os << *last << ' ';
        ++last;
    }
    auto s = os.str();
    IT_ASSERT_MSG(s, s == "4 3 2 1 ");

}

void multivector_unit::precursor2() {
    std::ostringstream os;
    auto m = wythe::multivector<int>{1, {10, { 100, 101, 102}}, 2, 3, 4};
    
    auto n = m.root().begin().begin().end();                // n points to one past 102
    --n;                                                    // n points to 102
    IT_ASSERT_MSG(*n, *n == 102);
    auto last = wythe::multivector<int>::precursor(n); // convert to an precursor
    // last points to 102
    IT_ASSERT_MSG(*last, *last == 102);
    auto x = wythe::to_precursor(--m.begin().begin().end());
    IT_ASSERT(*x == 102);
    IT_ASSERT(last == x);
    while (!last.is_root()) {
        os << *last << ' ';
        ++last;
    }
    IT_ASSERT_MSG(os.str(), os.str() == "102 101 100 10 1 ");
}

void multivector_unit::append_children() {
    auto m = wythe::multivector<int>{1, {10, { 100, 101, 102}}, 2, 3, 4};
    auto q = wythe::multivector<int>();
    wythe::append(q.root(), m.root());
    IT_ASSERT(q == m);

    auto c = q.begin().begin();
    auto r = wythe::multivector<int>{ 99, 100, 101 };
    IT_ASSERT(*c == 10); // make sure we are where we think we are
    wythe::append(c, r.root());
    auto s = wythe::multivector<int>{1, {10, { 100, 101, 102, 99, 100, 101}}, 2, 3, 4};
    IT_ASSERT(q == s);
}


int main (int, char **) {
    multivector_unit test;
    wythe::unit_test<multivector_unit> ut(&test);
    return ut.run();
}
