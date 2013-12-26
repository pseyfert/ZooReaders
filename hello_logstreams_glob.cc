#include "helpers/option.h"
#include "helpers/logstream.h"
#include "helpers/glob.h"
#include <string>
#include <iostream>

///@todo in this example -h doesn't work!


int main(int argc, char** argv) {
  std::string text;
  glob_options->push_back<std::string>(new option<std::string>('m',"",&text,true));

  glob_options->parse(argc,argv); /// parsing will throw exception if needed options are not given!


  logstreams::debug << "this is debug output" << std::endl;
  logstreams::warn << "this is warn output" << std::endl;
  logstreams::info << "this is info output" << std::endl;
  logstreams::error << "this is error output" << std::endl;
  logstreams::fatal << "this is fatal output" << std::endl;
  logstreams::always << "this is always output" << std::endl;

  logstreams::always << "You entered at -w: " << text << std::endl;

  return 0;
}
