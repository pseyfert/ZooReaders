#include "helpers/option.h"
#include "helpers/logstream.h"
#include "helpers/glob.h"
#include <string>
#include <iostream>


int main(int argc, char** argv) {
  glob_options.parse(argc,argv);
  glob_options.initialize();


  logstreams::debug << "this is debug output" << std::endl;
  logstreams::warn << "this is warn output" << std::endl;
  logstreams::info << "this is info output" << std::endl;
  logstreams::error << "this is error output" << std::endl;
  logstreams::fatal << "this is fatal output" << std::endl;
  logstreams::always << "this is always output" << std::endl;

  return 0;
}
