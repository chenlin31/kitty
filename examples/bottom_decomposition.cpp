    
#include "kitty/kitty.hpp"
#include <cstdint>
#include <unordered_set>
#include<fstream>
#include <iostream>
using namespace std;
using namespace kitty;
int main()
{
    kitty::dynamic_truth_table tt(5),tt1(5),tt2(5);
    kitty::create_from_expression(tt, "[{(ab)e}c]");
    cout<<"(ab+e)^c = ";
    kitty::print_hex(tt);
    kitty::create_from_expression(tt1, "[{ae}c]");
    cout<<",    (a+e)^c = ";
    kitty::print_hex(tt1);
    kitty::create_from_expression(tt2, "[ac]");
    cout<<",     a^d = ";
    kitty::print_hex(tt2);

    std::cout << std::endl;

    auto result = kitty::bottom_decomposition_impl(tt);

    print_bottom_decomposed_information(tt);


}
