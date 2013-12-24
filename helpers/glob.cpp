#include "glob.h"
#include "logstream.h"

void glob_reset_log(int myloglevel) {
  logstreams::logstream::setLogLevel(myloglevel);
}


options& init_glob_options() {
  static options opts;
  opts.push_back<int>(new option<int>('v',3,&glob_reset_log));
  return opts;
}

options glob_options = init_glob_options();
