#include<type_traits>
#include<cstdio>

template<typename T,typename...>
using always_type = T;

template<typename...>
using always_true_type = std::true_type;

template<typename...>
inline constexpr bool always_true_v = true;

struct Base{};
struct Derived : Base {};

// 1. Hide certain overloads based on traits of a type. This is the most straightforward use of SFINAE.
template<typename T>
constexpr std::enable_if_t<std::is_base_of_v<Base,T>,unsigned> func_1(const T&) {return 1;}
template<typename T>
constexpr std::enable_if_t<std::is_array_v<T>,unsigned> func_1(const T&) {return 2;}
template<typename T>
constexpr std::enable_if_t<std::is_same_v<T,int>,unsigned> func_1(const T&) {return 3;}
// Alternative. This is espescially useful with constructors, which have no return type syntactically.
template<typename T,std::enable_if_t<std::is_base_of_v<Base,T>,int> =0> // `...,int> =0` can be replaced by `...,bool> =true`,'...,long> =100', etc.
constexpr unsigned func_1_2(const T&) {return 1;}
template<typename T,std::enable_if_t<std::is_array_v<T>,int> =0>
constexpr unsigned func_1_2(const T&) {return 2;}
template<typename T,std::enable_if_t<std::is_same_v<T,int>,int> =0>
constexpr unsigned func_1_2(const T&) {return 3;}
// C++20 equivalents
template<typename T> requires std::is_base_of_v<Base,T>
constexpr unsigned func_1_cpp20(const T&) {return 1;}
template<typename T> requires std::is_array_v<T>
constexpr unsigned  func_1_cpp20(const T&) {return 2;}
template<typename T> requires std::is_same_v<T,int>
constexpr unsigned  func_1_cpp20(const T&) {return 3;}


// 2. Check if a class has a certain member typedef.
struct class_has_foo_type{typedef int foo_type;};

template<typename,typename=void>
struct has_foo_type : std::false_type {};
template<typename T>
struct has_foo_type<T,std::void_t<typename T::foo_type>> : std::true_type {};
// Alternative.
template<typename T>
auto has_foo_type_2_helper(int) -> always_true_type<typename T::foo_type>;
template<typename T>
auto has_foo_type_2_helper(long) -> std::false_type;

template<typename T>
struct has_foo_type_2 : decltype(has_foo_type_2_helper<T>(0)) {};
// Alternative
template<typename T,std::enable_if_t<always_true_v<typename T::foo_type>,int> =0>
auto has_foo_type_3_helper(int) -> std::true_type;
template<typename T>
auto has_foo_type_3_helper(long) -> std::false_type;

template<typename T>
struct has_foo_type_3 : decltype(has_foo_type_3_helper<T>(0)) {};
// Alternative
template<typename T>
auto has_foo_type_4_helper(int) -> std::conditional_t<always_true_v<typename T::foo_type>,std::true_type,std::false_type>;
template<typename T>
auto has_foo_type_4_helper(long) -> std::false_type;

template<typename T>
struct has_foo_type_4 : decltype(has_foo_type_4_helper<T>(0)) {};
// Alternative.
// Adapted from https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function  - Nicola Bonelli.
template <typename T>
struct has_foo_type_5{
    typedef char one;
    struct two{char a[2];};

    template <typename U>
    static one test(decltype( (void)std::declval<typename U::foo_type>() , int(0) ));
    template <typename U>
    static two test(long);

    static constexpr bool value = (sizeof(test<T>(0))==sizeof(one));
};
// ... or simplified to this:
template<typename T>
std::true_type has_foo_type_6_helper(decltype( (void)std::declval<typename T::foo_type>() , int(0) ));
template<typename T>
std::false_type has_foo_type_6_helper(long);

template <typename T>
struct has_foo_type_6 : decltype(has_foo_type_6_helper<T>(0)) {};


// 3. Check if a class has a certain member function.
struct class_has_func{
    void func(const char*,const Base&,Derived&&);
};
struct class_has_func_2{
    void func(const char*,const Base&,int);
};
template<typename T,typename R,typename... Args>
auto has_func_helper(int) -> std::conditional_t<std::is_same_v<decltype(std::declval<T>().func(std::declval<Args>()...)),R>,std::true_type,std::false_type>;
template<typename T,typename R,typename... Args>
auto has_func_helper(long) -> std::false_type;

template<typename T,typename R,typename... Args>
struct has_func : decltype(has_func_helper<T,R,Args...>(0)) {};
// Alternative.
template<typename,typename,typename,typename...>
struct has_func_2_helper : std::false_type {};
template<typename T,typename R,typename... Args>
struct has_func_2_helper<std::void_t<decltype(std::declval<T>().func(std::declval<Args>()...))>,T,R,Args...> : std::true_type {};

template<typename T,typename R,typename... Args>
struct has_func_2 : has_func_2_helper<void,T,R,Args...> {};
// Alternative.
template<typename T,typename... Args>
auto has_func_3_helper(int) -> always_true_type<decltype(std::declval<T>().func(std::declval<Args>()...))>;
template<typename T,typename... Args>
auto has_func_3_helper(long) -> std::false_type;

template<typename T,typename... Args>
struct has_func_3 : decltype(has_func_3_helper<T,Args...>(0)) {};

int main(){
    {
    Derived a;
    int b[5];
    int c;
    static_assert(func_1(a)==1);
    static_assert(func_1(b)==2);
    static_assert(func_1(c)==3);
    static_assert(func_1_2(a)==1);
    static_assert(func_1_2(b)==2);
    static_assert(func_1_2(c)==3);
    static_assert(func_1_cpp20(a)==1);
    static_assert(func_1_cpp20(b)==2);
    static_assert(func_1_cpp20(c)==3);
    }
    {
    static_assert(has_foo_type<class_has_foo_type>::value==true);
    static_assert(has_foo_type<Base>::value==false);
    static_assert(has_foo_type_2<class_has_foo_type>::value==true);
    static_assert(has_foo_type_2<Base>::value==false);
    static_assert(has_foo_type_3<class_has_foo_type>::value==true);
    static_assert(has_foo_type_3<Base>::value==false);
    static_assert(has_foo_type_4<class_has_foo_type>::value==true);
    static_assert(has_foo_type_4<Base>::value==false);
    static_assert(has_foo_type_5<class_has_foo_type>::value==true);
    static_assert(has_foo_type_5<Base>::value==false);
    static_assert(has_foo_type_6<class_has_foo_type>::value==true);
    static_assert(has_foo_type_6<Base>::value==false);
    }
    {
    static_assert(has_func<class_has_func,void,const char*,const Base&,Derived&&>::value==true);
    static_assert(has_func<class_has_func_2,void,const char*,const Base&,Derived&&>::value==false);
    static_assert(has_func<class_has_func_2,void,const char*,const Base&,int>::value==true);
    static_assert(has_func<class_has_func_2,int,const char*,const Base&,Derived&&>::value==false);

    static_assert(has_func_2<class_has_func,void,const char*,const Base&,Derived&&>::value==true);
    static_assert(has_func_2<class_has_func_2,void,const char*,const Base&,Derived&&>::value==false);
    static_assert(has_func_2<class_has_func_2,void,const char*,const Base&,int>::value==true);
    static_assert(has_func_2<class_has_func_2,int,const char*,const Base&,Derived&&>::value==false);

    static_assert(has_func_3<class_has_func,const char*,const Base&,Derived&&>::value==true);
    static_assert(has_func_3<class_has_func_2,const char*,const Base&,Derived&&>::value==false);
    static_assert(has_func_3<class_has_func_2,const char*,const Base&,int>::value==true);
    }
}
