#include <iostream>
#include <limits>
#include "logstream.h"
#include "option.h"
#include "TString.h"

template <class T>
option<T>::option(char caller) {
  m_needed = false;
  m_caller = caller;
  m_value = &storage;
  m_set = false;
}

///@todo: merge initialize and help

template <class T>
void option<T>::initialize() {
  std::cout << "option: ";
  return shortinitialize();
}

template <class T>
void option<T>::shortinitialize() {
  std::cout << "\t-" << m_caller;
  std::cout << "\t" << value();
  if (m_set)
    std::cout << " [" << default_value << "]";
  else
    std::cout << " (default value)";
  std::cout << "\tstored at " << m_value;
  std::cout << std::endl;
  return;
}

template <>
option<int>::option(char caller) {
  m_needed = false;
  m_value = &storage;
  m_caller = caller;
  m_set = false;
  default_value = std::numeric_limits<int>::max();
}

template <>
option<bool>::option(char caller) {
  m_needed = false;
  m_value = &storage;
  m_caller = caller;
  m_set=false;
  *m_value = true;
  default_value = false;
}

template <>
option<std::string>::option(char caller) {
  m_value = &storage;
  m_caller = caller;
  m_set=false;
  *m_value = std::string("");
  default_value = std::string("");
}


template <class T>
option<T>::~option() {
}

template <class T>
option<T>::option(char caller, T def)   {
  m_needed = false;
  m_value = &storage;
  m_caller = caller;
  m_set = false;
  default_value = def;
}

template <class T>
option<T>::option(char caller, T def, T* value) {
  m_needed = false;
  m_value = value;
  m_caller = caller;
  m_set = false;
  default_value = def;
  *value = def;
}

template <class T>
option<T>::option(char caller, T def, T* value, bool need) {
  m_needed = need;
  m_value = value;
  m_caller = caller;
  m_set = false;
  default_value = def;
}

template <class T>
void option<T>::set(T arg) {
  m_set = true;
  *m_value = arg;
}

void options::help() {
  for (unsigned jj = 0 ; jj < vec.size() ; ++jj) {
    vec[jj]->help();
  }
  std::cout << "all other arguments " << helpmessage << std::endl;
}

void options::initialize() {
  std::cout << "possible options are: " << std::endl;
  //BOOST_FOREACH(virtualoption* opt, vec) {
  for (unsigned jj = 0 ; jj < vec.size() ; ++jj) {
    virtualoption* opt = vec[jj];
    opt->shortinitialize();
  }
  std::cout << "values in square brackets are the default values" << std::endl;
  if (overflow.size()) {
    std::cout << "other arguments:" << std::endl;
    //BOOST_FOREACH(std::string arg, overflow) {
    for (unsigned jj = 0 ; jj < overflow.size() ; ++jj) {
      std::string arg = overflow[jj];
      std::cout << arg << std::endl;
    }
  }
}

template<class T>
option<T>* options::push_back(option<T>* opt) {
  //BOOST_FOREACH(virtualoption* presentoption, vec) {
  for (unsigned jj = 0 ; jj < vec.size() ; ++jj) {
    virtualoption* presentoption = vec[jj];
    if (opt->callme() == presentoption->callme()) {
      std::cerr << "YOU MAY NOT USE AN OPTION TWICE!!!" << std::endl;
      return NULL;
    }
  }
  vec.push_back((virtualoption*)opt);
  return opt;
}

template <>
void option<bool>::set(char* value) {
  if (0==strcmp(value,"true"))
    set(true);
  else if (0==strcmp(value,"false"))
    set(false);
  else
    std::cerr << "I don't understand you" << std::endl;
}

template <>
void option<int>::set(char* value) {
  //set(int(atoi(value)));
  char* buffer;
  set(int(strtol(value,&buffer,0)));
}

template <>
void option<unsigned>::set(char* value) {
  //set(int(atoi(value)));
  char* buffer;
  set(unsigned (strtol(value,&buffer,0)));
}
template <>
void option<std::string>::set(char* value) {
  set(std::string(value));
}

template <>
void option<TString>::set(char* value) {
  set(TString(value));
}

template <>
void option<bool>::set() {
  set(true);
}

template <>
bool option<bool>::isbool() {
  return true;
}

template <class T>
bool option<T>::isbool() {
  return false;
}

template <class T>
void option<T>::set() {
  std::cerr << "no argument given for option -" << m_caller << std::endl;
}


virtualoption* options::get_option(const char caller) {
  /// @todo: this will be so cool with C++11
  std::vector<virtualoption*>::const_iterator iter = vec.begin();
  for ( ; vec.end() != iter ; ++iter ) {
    if (caller == iter->callme()) {
      return *iter;
    }
  }
  return NULL;
}

options::options() {
  overflow.clear();
  m_needoverflow=false;
  helpmessage = std::string("will be ignored");
  push_back<bool>(new option<bool>('h',false,&m_printhelp,false))->enter_helpmessage(std::string("print this help message"));
}
options::options(bool needs_overflow) {
  overflow.clear();
  m_needoverflow = needs_overflow;
  if(needs_overflow) 
    helpmessage = std::string("must be given"); 
  else 
    helpmessage = std::string("will be ignored");
  push_back<bool>(new option<bool>('h',false,&m_printhelp,false))->enter_helpmessage(std::string("print this help message"));
}

void options::parse(int argc, char** argv) {
  for (unsigned i = 1; i < argc ; ++i) {
    bool found = false;
    if ('-'==argv[i][0]) {
      //BOOST_FOREACH(virtualoption* opt, vec) {
      for (unsigned jj = 0 ; jj < vec.size() ; ++jj) {
        virtualoption* opt = vec[jj];
        if(opt->callme() == argv[i][1]) {
          if (opt->isbool()) {
            opt->set();
            if (i+1<argc) {// check if bool value is explicitely given
              if (0==strcmp(argv[i+1],"true")||strcmp(argv[i+1],"false")==0)
                opt->set(argv[++i]);
            }
          } else {
            if (i+1>=argc) std::cerr << "option -" << argv[i][1] << " needs an argument" << std::endl;
            else {
              if (argv[i+1][0] == '-')
                opt->set();//(char*)"true");
              else
                opt->set(argv[++i]);
            }
          }
          found = true;
          break;
        }
      }
      if (found) continue;
      std::cerr << "unrecognized option: -" << argv[i][1] << std::endl;
      continue;
    }
    overflow.push_back(std::string(argv[i]));
  }
  if(m_printhelp)
    help();
  //  if(UNLIKELY(logstreams::logstream::logLevel() <= logstreams::DEBUG))
  //   initialize();

  if (m_needoverflow && overflow.empty()) {
    std::cerr << "not enough arguments" << std::endl;
    help();
    throw 30;
  }
    //BOOST_FOREACH(virtualoption* opt, vec) {
    for (unsigned jj = 0 ; jj < vec.size() ; ++jj) {
      virtualoption* opt = vec[jj];
      if (opt->isneeded() && !opt->isset()) {
      std::cerr << "option -" << opt->callme() << " not defined" << std::endl;
      opt->help();
      throw 31;
    }
  }
}

template class option<bool>;
template class option<int>;
template class option<unsigned>;
template class option<std::string>;
template class option<TString>;
template option<bool>* options::push_back<bool>(option<bool>*);
template option<int>* options::push_back<int>(option<int>*);
template option<unsigned>* options::push_back<unsigned>(option<unsigned>*);
template option<TString>* options::push_back<TString>(option<TString>*);
template option<std::string>* options::push_back<std::string>(option<std::string>*);
/*
int dummy() {
  options opts;
  bool val;
  opts.push_back<int>(new option<int>('a',3));
  opts.push_back<bool>(new option<bool>('a'));
  opts.push_back<bool>(new option<bool>('a',true,&val));
  return 0;
}
*/

//int main(int argc, char** argv) {
//  bool the_d = true;
//  options opts;
//  opts.push_back<int>(new option<int>('c'));
//  opts.push_back<bool>(new option<bool>('d',true,&the_d));
//  opts.push_back<int>(new option<int>('e'));
//  opts.push_back<int>(new option<int>('e',100));
//
//  opts.initialize();
//  opts.parse(argc,argv);
//
//  opts.initialize();
//
//  std::cout << "the d is stored at " << &the_d << " and holds " << the_d << std::endl;
//
//  return 0;
//}