#include<initializer_list>

struct Simple{
    Simple()=default;
};
struct Simple_e{
    explicit Simple_e()=default;
};

struct Foo{
    explicit Foo(int,int) {}
    Foo(int,double) {}
};

struct Ini_List{
    explicit Ini_List(std::initializer_list<int>) {}
};

struct Pathological{
    Pathological()=default;
    Pathological(int) {}
    explicit Pathological(const Pathological&)=default;
};

void func_foo(Foo) {}

Foo func_rt_foo_1() {return {1,2};} // Error.
Foo func_rt_foo_2() {return Foo{1,2};} // OK.
Foo func_rt_foo_3() {return {1,3.14159};} // OK.

void func_pathological(Pathological) {}

Pathological func_rt_pathological_1() {return Pathological(1);} // OK.
Pathological func_rt_pathological_2() {Pathological p(1); return p;} // Error.
Pathological func_rt_pathological_3() {return 1;} // OK.

int main(){
    Simple sp1;
    Simple_e spe1;
    Simple sp2{};
    Simple_e spe2{};
    Simple sp3={};
    Simple_e spe3={}; // Error.

    Foo foo_1={1,2}; // Error.
    Foo foo_2={1,3.14159}; // OK.
    Foo foo_3{1,2}; // OK.

    Ini_List il1={1,1,1,1,1}; // Error.
    Ini_List il2{1,2,3,4,5}; // OK.

    Pathological p1={}; // OK. This is because the so-called copy-initialization (well, not exactly, after C++11 this is form is called copy-list-initialization) does NOT always require copy-ctor. (Rather stupid IMO. The name is really confusing.)  Copy-ctor is required only when the rhs is of the declared type (or a class derived from it, from the perspective of the declaration).
    Pathological p2={5}; // Also OK.
    Pathological p3=p1; // Error.

    func_foo({1,2}); // Error.
    func_foo(Foo{1,2}); // OK.
    func_foo({1,3.14159}); // OK.

    // Aggregate initialization always performs copy-initialization for the individual initializers, even if the syntax resembling direct-initialization (i.e. direct-list-initialization) is used.
    // Therefore the followings are all ill-formed, including the second one.
    Foo foo_arr_1[6]={{1,2}};
    Foo foo_arr_2[6]{{1,2}}; // Error!! Surprised?
    Foo foo_arr_3[6]={Foo(1,3.14159),{}};

    func_pathological(p1); // Explicit copy-ctor means that you cannot pass by value in the natural way.
    func_pathological(Pathological(p1)); // This, however, is OK.

    try{
        throw Pathological{};
    }catch(Pathological){ // Catch by VALUE. // This should be an error by standard. Clang 11.0 does this correctly but g++ 10.3 does not consider it an error.

    }
}
