#include<cstdio>

struct Foo{
    int a;
    Foo()=default;
};

struct Foo2{
    int a;
    Foo2();
};
Foo2::Foo2()=default;

int main(){
    Foo foo{};
    Foo2 foo2{};
    printf("%d %d\n",foo.a,foo2.a); // `foo.a` is guaranteed to be 0, while `foo2.a` has indeterminate value.
    // This is because a constructor is user-provided if it is user-declared and not explicitly defaulted on its FIRST declaration. So `Foo()=default;` is not user-provided but `Foo2::Foo2()=default;` is.
    // This leads to different behaviour with value-initialization. If the default-ctor is not user-provided, the object is zero-initialized first before calling the default-ctor.
    Foo foo3;
    Foo2 foo4;
    printf("%d %d\n",foo3.a,foo4.a); // Both have indeterminate value.
}