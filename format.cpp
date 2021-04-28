#include<fmt/core.h>

FILE* log_this = fopen("format.log","w");

struct Foo{
    int a;
    const char* b;
};

template<>
struct fmt::formatter<Foo,char>{
    auto parse(fmt::basic_format_parse_context<char>& ctx)->typename fmt::basic_format_parse_context<char>::iterator {
        // Print the string to be parsed.
        fprintf(log_this,"parse: ");
        auto b = ctx.begin();
        auto e = ctx.end();
        for(;b!=e;++b){
            fprintf(log_this,"%c",*b);
        }
        fprintf(log_this,"\n");
        // Real parsing.
        if(ctx.begin()!=ctx.end()){
            switch (*ctx.begin()){
                case 'a' : {
                    mode=0;
                    break;
                }
                case 'b' : {
                    mode=1;
                    break;
                }
            }
        }
        return std::next(ctx.begin());
    }

    template<typename OutputIt>
    auto format(Foo const& foo,fmt::basic_format_context<OutputIt,char>& ctx) const -> typename fmt::basic_format_context<OutputIt,char>::iterator {
        switch(mode){
            case 0 :
                return fmt::format_to(ctx.out(),"({},)",foo.a);
            case 1 :
                return fmt::format_to(ctx.out(),"(,{})",foo.b);
            default :
                return fmt::format_to(ctx.out(),"({},{})",foo.a,foo.b);
        }
    }
  private:
    unsigned char mode=2;
};

struct Foo2{
    int a;
    const char* b;
};
template<>
struct fmt::formatter<Foo2,char>{
    static constexpr auto parse(fmt::basic_format_parse_context<char>& ctx) -> typename fmt::basic_format_parse_context<char>::iterator {return ctx.begin();}

    template<typename OutputIt>
    static auto format(Foo2 const& foo2,fmt::basic_format_context<OutputIt,char>& ctx) -> typename fmt::basic_format_context<OutputIt,char>::iterator {
        return fmt::format_to(ctx.out(),"{}",foo2.a);
    }
};

int main(){
    Foo foo1={1,"first"};
    Foo foo2={2,"second"};
    fmt::print(fmt::runtime("{:1}\n{:b}\n{}\n"),foo1,foo2,foo1);
    /* Expected:
    (1,first)
    (,second)
    (1,first)
    */

   Foo2 foo21 = {3,"third"};
   fmt::print("{}",foo21);
}
