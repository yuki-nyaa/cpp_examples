#include<vector>
#include<cassert>
int main(){
    // From https://quuxplusone.github.io/blog/2019/02/18/knightmare-of-initialization/
    std::vector<char> vec_1{42,'x'}; // Brace
    std::vector<char> vec_2(42,'x'); // Normal parenthesis
    assert(vec_1.size()==2); // Namely, 42 (as a char) and 'x'.
    assert(vec_2.size()==42); // Namely, 42 'x's.
    // This is because with the list-initialization syntax, the constructor callable with a single argument of type `std::initializer_list<T>`, if any, is considered first.
    // Suggestion: Use the list syntax only when you are listing the elements, or when you want to initialize an aggregate, or when you want to value-initialize (i.e. `T t={}` or `T t{}`).
}