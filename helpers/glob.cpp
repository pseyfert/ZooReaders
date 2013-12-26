#include "glob.h"
#include "logstream.h"

int glob_reset_log(int myloglevel) {
  logstreams::logstream::setLogLevel(myloglevel);
  return 0;
}


options* init_glob_options() {
  options* opts = new options();
  opts->push_back<int>(new option<int>('v',3,&glob_reset_log));
  return opts;
}

options* glob_options = init_glob_options();
