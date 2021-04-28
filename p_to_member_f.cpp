#include<cstdio>

struct C{
    void f(int) & {printf("nc ");}
    void f(int) const& noexcept {printf("c ");}
};

int main(){
    void (C::* p)(int) & = &C::f;
    // void (C::* p3)(int) & noexcept = &C::f; // Error
    void (C::* p2)(int) const& = &C::f; // OK.

    C c;
    (c.*p)(1); // Prints "nc"
    const C c2;
    (c2.*p2)(1); // Prints "c"

    // (c2.*p)(1); // Error
    (c.*p2)(1); // OK. Prints "c"
}