#include "helpers/option.h"
#include "helpers/logstream.h"
#include <string>
#include <iostream>

//int myloglevel = logstreams::logstream::logLevel();

template <class T>
void noise(T) {
  logstreams::debug << "MAKE SOME NOISE" << std::endl;
}

void reset_log(int myloglevel) {
  std::cout << "my log level " << myloglevel << std::endl;
  logstreams::logstream::setLogLevel(myloglevel);
}

int main(int argc, char** argv) {
  /// default instantiation, option 'h' exists by default
  options opts;
  /// adding additional information what will be done with other arguments
  opts.enter_helpmessage(" are useless");

  option<int> loglevel('v',1,&reset_log); ///_v_erbosity
//option<int> loglevel('v',1,&myloglevel,false,&reset_log); ///_v_erbosity
////option<int> loglevel('v',logstreams::logstream::logLevel()); ///_v_erbosity
  opts.push_back<int>(&loglevel);

  opts.push_back<char>(new option<char>('n',(char)'b',&(noise<char>)));

  opts.parse(argc,argv);
////logstreams::logstream::setLogLevel(loglevel.value());


  logstreams::debug << "this is debug output" << std::endl;
  logstreams::warn << "this is warn output" << std::endl;
  logstreams::info << "this is info output" << std::endl;
  logstreams::error << "this is error output" << std::endl;
  logstreams::fatal << "this is fatal output" << std::endl;
  logstreams::always << "this is always output" << std::endl;

  return 0;
}
