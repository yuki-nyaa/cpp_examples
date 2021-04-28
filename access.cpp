struct A{
    int a;
    int fun_a() const {return a;}
};

struct B:A{
  private:
    using A::a;
    using A::fun_a;
};

struct C:B{
    int foo() const {return A::a;} // GCC error.   
    int foo1() const {return a;} // Both error.
    int foo2() const {return A::fun_a();} // Both fine. Funny that GCC treats data member and function member differently.
    int foo3() const {return fun_a();} // Both error.
    int foo4() const {return static_cast<const A*>(this)->a;} // Both fine.
};

struct C1:B{
    using A::a; // GCC error.
    using A::fun_a; // GCC error.
};

struct B2 : private A {}; // Aside from pointer cast accessibility, I suppose this should be equivalent to `B`.

struct C2:B2{
    // Both error for all below.
    int foo() const {return A::a;}  
    int foo1() const {return a;}
    int foo2() const {return A::fun_a();}
    int foo3() const {return fun_a();}
    int foo4() const {return static_cast<const A*>(this)->a;}
};

int main() {}
