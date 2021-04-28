struct Foo{
    explicit Foo()=default;
    explicit Foo(int) {}
};
struct Wrapped_Foo{
    Foo foo;
};
struct Wrapped_Foo2{
    Foo foo;
    int i=100;
};
struct Wrapped_Foo3{
    Foo foo;
    Wrapped_Foo3()=default;
};
struct Wrapped_Foo_Non_Agg{
    Foo foo;
    virtual void some_func();
};
struct Inherited_Foo : private Foo {using Foo::Foo;};
int main(){
    // These two are naturally error.
    Foo f={};
    Foo f2=1;

    Wrapped_Foo wf={}; // Error.
    Wrapped_Foo2 wf2={}; // Error.
    // The previous 2 may look confusing, if you look at the following:
    // [class.default.ctor] - If there is no user-declared constructor for class X, a non-explicit constructor having no parameters is implicitly declared as defaulted (9.5).
    // However, since both `Wrapped_Foo` and `Wrapped_Foo2`(unless with C++11) are aggregate, aggregate-initialization is performed, which directly calls the member ctors, instead of value-initialzation, that calls the member ctors indirectly.
    // So for example, `Wrapped_Foo2 wf2={};` does the following two things directly: `foo={}`, and `i=100`. No call to `Wrapped_Foo2()` is made. Since `foo={}` would make an error, the whole initialization is an error too.

    Wrapped_Foo_Non_Agg wf_ng={}; // Now this is OK. Directly calls `Wrapped_Foo_Non_Agg()`, which is non-explicit.
    Wrapped_Foo3 wf3={}; // This compiles, of course.

    Inherited_Foo if1={}; // OK. Actually the implicit-default-ctor of `Inherited_Foo`, which is non-explicit, shadows the inherited ctor 'Foo()'.
    Inherited_Foo if2=1; // Error.
}