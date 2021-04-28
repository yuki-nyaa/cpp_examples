#include<cstdio>
#include<type_traits>

struct A{
    A(int) {}
};

struct B{
    constexpr B(int) {}
};

// [dcl.constexpr] "If the instantiated template specialization of a constexpr function template or member function of a class template would fail to satisfy the requirements for a constexpr function, that specialization is still a constexpr function, even though a call to such a function cannot appear in a constant expression. If no specialization of the template would satisfy the requirements for a constexpr function when considered as a non-template function, the template is ill-formed, no diagnostic required."
template<typename T>
struct C : T{
    constexpr C(int) : T(0) {}
};




template<typename T>
constexpr int func() {return 100;}

// [dcl.constexpr] " [Note: An explicit specialization can differ from the template declaration with respect to the constexpr or consteval specifier. — end note]"
// Actually this specialization cannot be declared as `constexpr`.
template<>
int func<A>() {A a(0);return 200;}




struct D{
    int func() {return 300;}
};

struct E{
    constexpr int func() {return 400;}
};

template<typename T>
struct F : T{
    constexpr int func() {return T::func();}
};




int main(){
    // constexpr C<A> ca = 0; // Error.
    const C<A> ca = 0;
    constexpr C<B> cb = 0;

    constexpr int i1 = func<B>();
    // constexpr int i2 = func<A>(); // Error.

    // constexpr int i3 = F<D>{}.func(); // Error.
    constexpr int i3 = F<E>{}.func();
}