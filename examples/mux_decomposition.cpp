#include <kitty/kitty.hpp>
#include <cstdint>
#include <unordered_set>
#include <fstream>
#include <iostream>
using namespace std;
using namespace kitty;
int main()
{

  //1、(ab+!ac)^d
  kitty::dynamic_truth_table func( 5 );
  create_from_expression( func, "[{(ab)(!ac)}d]" );
  std::cout << "(ab+!ac)^d = ";
  print_hex( func );
  cout << endl;
  print_mux_decomposed_information( func );
   cout << endl;


  //2、(ab+!ac)+d+!de
  kitty::dynamic_truth_table func1( 5 );
  create_from_expression( func1, "{{{(ab)(!ac)}d}(!de)}" );
  std::cout << "(ab+!ac)+d+!de = ";
  print_hex( func1 );
  cout << endl;
  print_mux_decomposed_information( func1 );
 cout << endl;

  //3、(ab+!ac)d+!de
  kitty::dynamic_truth_table func2( 5 );
  create_from_expression( func2, "{({(ab)(!ac)}d)(!de)}" );
  std::cout << "(ab+!ac)d+!de = ";
  print_hex( func2 );
  cout << endl;
  print_mux_decomposed_information( func2 );
 cout << endl;

  //4、(ab+!ac)d+!de+f
  kitty::dynamic_truth_table func3( 5 );
  create_from_expression( func3, "{{({(ab)(!ac)}d)(!de)}f}" );
  std::cout << "(ab+!ac)d+!de+f = ";
  print_hex( func3 );
  cout << endl;
  print_mux_decomposed_information( func3 );
 cout << endl;

  //5、(ab+!ac)^d+!de
  kitty::dynamic_truth_table func4( 5 );
  create_from_expression( func4, "{[{(ab)(!ac)}d](!de)}" );
  std::cout << "(ab+!ac)^d+!de = ";
  print_hex( func4 );
  cout << endl;
  print_mux_decomposed_information( func4 );
 cout << endl;

  //6、a+bc+!bd
  kitty::dynamic_truth_table func5( 5 );
  create_from_expression( func5, "{{a(bc)}(!bd)}" );
  std::cout << "a+bc+!bd = ";
  print_hex( func5 );
  cout << endl;
  print_mux_decomposed_information( func5 );
 cout << endl;

  //7、a(bc+!bd)+!ae
  kitty::dynamic_truth_table func6( 5 );
  create_from_expression( func6, "{(a{(bc)(!bd)})(!ae)}" );
  std::cout << "a(bc+!bd)+!ae = ";
  print_hex( func6 );
  cout << endl;
  print_mux_decomposed_information( func6 );
 cout << endl;

  //8、ab^(cd+!ce)
  kitty::dynamic_truth_table func7( 5 );
  create_from_expression( func7, "[(ab){(cd)(!ce)}]" );
  std::cout << "ab^(cd+!ce) = ";
  print_hex( func7 );
  cout << endl;
  print_mux_decomposed_information( func7 );

}
