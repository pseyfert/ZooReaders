#include "helpers/option.h"
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  int w_int;
  /// default instantiation, option 'h' exists by default
  options opts;
  /// adding additional information what will be done with other arguments
  opts.enter_helpmessage(" are useless");
  /// adding and instantiating in one line
  opts.push_back<std::string>(new option<std::string>('t'));

  /// or in two lines
  option<int> s_option('s'); /// NB: this triggers an uninitialised int!
  opts.push_back<int>(&s_option);
  /// constructor with default value
  option<int> v_option('v',1337);
  opts.push_back<int>(&v_option);
  /// storing the option's value at dedicated location
  option<int> w_option('w',1337,&w_int);
  opts.push_back<int>(&w_option);

  opts.parse(argc,argv);

  std::cout << " let's see what got parsed " << std::endl;
  logstreams::logstream::setLogLevel(1);
  opts.show_settings();

  /// retrieve option and value via get_option
  std::string t_string = (dynamic_cast<option<std::string>*>(opts.get_option('t')))->value();

  int s_int = s_option.value();

  std::cout << " s is " << s_int << std::endl;
  if (s_option.isset()) 
    std::cout << " s has been set by user " << std::endl;
  else
    std::cout << " s has NOT been set by user " << std::endl;
  std::cout << " t is " << t_string << std::endl;
  std::cout << " u is " << opts.get_option('u') << std::endl;  // should be NULL
  std::cout << " v is " << v_option.value() << std::endl;
  std::cout << " w is " << w_int << std::endl;

  /// int can also be passed as hexadecimal numbers 0xa

  return 0;
}
