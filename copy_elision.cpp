// With C++14 you get 4 errors. With C++17/20 you get only 1 error.

struct Non_Copyable{
    Non_Copyable() = default;
    Non_Copyable(const Non_Copyable&) = delete;
};

struct Non_Copyable_Destructible{
    Non_Copyable_Destructible() = default;
    Non_Copyable_Destructible(const Non_Copyable&) = delete;
    ~Non_Copyable_Destructible() = delete;
};

struct Wrapped_NC{
    Non_Copyable nc;
    Wrapped_NC(const Non_Copyable& nc_other) : nc(nc_other) {}; // Ill-formed, even after C++17.
};

Non_Copyable func(){
    return Non_Copyable(); // Well-formed after C++17.
}

Non_Copyable_Destructible func2(){
    return Non_Copyable_Destructible(); // Ill-formed, even after C++17, although no object is to be destroyed.
}

int main(){
    Non_Copyable nc = Non_Copyable(); // Well-formed after C++17. Equivalent to 'Non_Copyable nc{}'.
    Non_Copyable nc2 = func(); // Well-formed after C++17. Equivalent to `Non_Copyable nc2{}`.
    Non_Copyable nc_arr[100]={Non_Copyable(),Non_Copyable()}; // Well-formed after C++17.
}

// These are mandatory copy-elisions.
// Note, however, that in the case of the non-mandatory copy-elisions, the copy/move-ctor must be available, even though they might not be actually called.
