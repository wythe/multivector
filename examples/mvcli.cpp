#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>

#include <wythe/multivector.h>
#include "command.h"

template <typename T>
void add_recursive(T x, unsigned int n) {
    auto y = x.emplace(n);
    if (n > 0) add_recursive(y, n - 1);
}

wythe::multivector<int> one() {
    wythe::multivector<int> tree;
    wythe::verify(tree);

    add_recursive(tree.root(), 3);

    wythe::verify(tree);

    add_recursive(tree.root(), 3);
    wythe::verify(tree);

    add_recursive(tree.root(), 3);
    wythe::verify(tree);

    tree.root().emplace(33).emplace(34).emplace(35);

    int c = 4;
    for (auto n : {1, 2, 3}) {
        tree.root().emplace_back(n);
        auto i = tree.root().end() - 1;
        for (int n = 0; n < 3; ++n) {
            i.emplace_back(c++);
            auto j = i.begin() + i.size() - 1;
            //--j;
            for (int n = 0; n < 2; ++n) j.emplace_back(n);
        }
    }
    return tree;
}

#if 0
// something like this would be cool
template <typename Cursor>
Cursor navigate(Cursor parent, Args...) {
}
#endif

wythe::multivector<int> two() {
    wythe::multivector<int> tree;
    for (int i = 0; i < 10; ++i) tree.root().emplace_back(i);
    for (int i = 40; i < 50; ++i) (tree.begin() + 5).emplace_back(i);
    for (int i = 100; i < 103; ++ i) ((tree.begin() + 5).begin() + 3).emplace_back(i);
    for (int i = 0; i < 10; ++i) ((tree.begin() + 5).begin() + 7).emplace_back(i);
    wythe::verify(tree);
    return tree;
}

bool grow(wythe::multivector<int> & tree) {
    for (int i = 200; i < 240; ++i) (tree.begin() + 5).emplace_back(i);
    wythe::verify(tree);
    return true;
}

void iterate() {
    wythe::multivector<int> tree = two();

    std::cout << "print recursively using cursor\n";
    std::cout << wythe::to_text(tree);

    std::cout<< "\nprint top level nodes only\n";
    for (int i : tree.root()) std::cout << i << "\n";

    std::cout << "\nprint all items using linear cursor:\n";
    for (wythe::multivector<int>::linear_cursor i = tree.begin(); i != tree.end(); ++i) std::cout << *i << "\n";

    std::cout << "\nagain, using range for loop:\n";
    for (int i : tree) std::cout << i << "\n";

    std::cout<< "\nprint first level children of tree[5] only\n";
    for (int i : (tree.begin() + 5)) std::cout << i << "\n";

    std::cout << "\nagain, using linear cursor:\n";
    for (wythe::multivector<int>::linear_cursor i = tree.begin(); i != tree.end(); ++i) std::cout << *i << "\n";

    std::cout<< "\nprint all children (including subchildren) of tree[5] only\n";
    auto fifth = tree.begin() + 5;
    for (wythe::multivector<int>::linear_cursor i = fifth.begin(); i!=fifth.end(); ++i) std::cout << *i << "\n";

    std::cout << "\nreverse cursor over nodes from tree[5][7][9] on up:\n";
    auto s = --(((tree.begin() + 5).begin() + 7).end());
    for (auto i = wythe::to_precursor(s); !i.is_root(); ++i) std::cout << *i << "\n";
}

void basics() {
    wythe::multivector<int> tree = one();
    wythe::verify(tree);

    {
        wythe::multivector<int>::cursor a;
        a = tree.begin();
        wythe::verify(a);
        wythe::multivector<int>::cursor b;
        b = a;
        wythe::verify(b);
    }


    tree = two();
    wythe::verify(tree);

    std::cout << wythe::to_text(tree) << "\n\n";

    std::cout << "iterator over top level nodes only:\n";
    for (auto i : tree.root()) std::cout << i << "\n";

    std::cout << "\nprint tree in compact string representation:\n";
    std::cout << wythe::compact_string(tree) << "\n";

    {
        auto tree2 = wythe::multivector<int> { 1, 2, 3, { 1, 2, 3, { 1, 2, 3 }}, 4 };
        std::cout << "\nprint another one:\n";
        std::cout << wythe::compact_string(tree2) << "\n";
    }

    std::cout << "\ncause reallocation of tree[5] and print:\n";
    grow(tree);
    std::cout << wythe::to_text(tree) << "\n\n";
    wythe::verify(tree);

    auto a = tree.begin();
    wythe::multivector<int>::cursor b;
    b = a;
    wythe::multivector<int>::cursor c(a);

    if (a == b) std::cout << "equal\n";
    if (a != b) std::cout << "not equal\n";

    std::cout << "*a is " << *a << "\n";

    ++a;
    (void)a++;
    *a++;
    std::swap(a, b);

#if 0
    std::cout << "\nfind 102:\n";
    auto i = std::find(tree.begin(), tree.end(), 102);
    if (i != tree.end()) std::cout << "found " << *i << "\n";

    std::cout << "\nfind 99:\n";
    i = std::find(tree.begin(), tree.end(), 99);
    if (i == tree.end()) std::cout << "not found\n";

    std::cout << "\nfind 43 with reverse cursor starting at tree[5][7][3]\n";
    auto r = wythe::rfind(tree.root()[5][7][3], "43");
    if (!r.is_root()) std::cout << "found " << *r << "\n";
#endif
}

void strings() {
    std::cout << "\nnow create a multivector of strings:\n";
    wythe::multivector<std::string> smultivector;
    auto cursor = smultivector.root().emplace("one").emplace("two").emplace("three");
    cursor.emplace_back("four");
    cursor.emplace_back("five");
    cursor.emplace_back("six");

    std::cout << wythe::to_text(smultivector) << "\n";

    wythe::verify(smultivector);
}

struct Custom {
    std::string name;
    int length;
    uint64_t value;
    std::string description;
    bool operator==(const std::string & n) const { return name == n; }
    bool operator==(const std::string & n) { return name == n; }
};

std::ostream & operator<<(std::ostream & os, const Custom & c) {
    os << c.name << " " << c.length << " " << c.value << " " << c.description;
    return os;
}

void custom() {
    wythe::multivector<Custom> tree;

    auto i = tree.root().emplace(Custom{"record", 5, 5, "0"});

    i.emplace_back(Custom{"bar", 1, 6, "a"});
    i.emplace_back(Custom{"goo", 8, 23, "b"});
    i.emplace_back(Custom{"foo", 32, 154, "c"});
    tree.root().emplace_back(Custom{"crc", 80, 5555555, "d"});

    std::cout << "\nnow create custom message:\n";
    std::cout << wythe::to_text(tree) << "\n";

    auto sub = wythe::multivector<Custom>(tree.begin());
    std::cout << "\nnow create a sub message:\n";
    std::cout << wythe::to_text(sub) << "\n";
}

void constvector(const std::vector<int> v) {
    std::vector<int>::const_iterator first = v.begin();
    std::vector<int>::const_iterator last = v.end();
    while (first != last) {
        std::cout << *first << "\n";
        ++first;
    }

}

template <typename T>
void print_reverse_cursor(T c) {
    while (!c.is_root()) {
        std::cout << *c << "\n";
        ++c;
    }
}

void constmultivector(const wythe::multivector<int> & tree) {
    // forward iterator
    std::cout << "print tree using forward const_iterator\n";
    wythe::verify(tree);

    auto first = tree.begin();
    auto last = tree.end();
    while (first != last) {
        std::cout << *first << "\n";
        ++first;
    }
}

void constness() {
    wythe::multivector<int> tree = two();
    std::cout << wythe::to_text(tree);
    constmultivector(tree);
}

int main(int argc, char **argv) {

    try {
        wythe::command line("multivectorcli", "wythe::multivector exerciser", "mvcli [options]");
        line.add(wythe::option("basics", 'b', "Display basic multivector calls",      []{ basics(); } ));
        line.add(wythe::option("iterate", 'i', "Display iterator calls",       []{ iterate(); } ));
        line.add(wythe::option("strings", 's', "Display string multivector calls",    []{ strings(); } ));
        line.add(wythe::option("custom", 'c', "Display custom multivector calls",     []{ custom(); } ));
        line.add(wythe::option("constness", 'C', "const iterators/cursors",      []{ constness(); } ));
        line.add(wythe::option("All", 'A', "run all", []{
            basics();
            iterate();
            strings();
            custom();
            constness();
        } ));

        line.parse(argc, argv);
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
}

