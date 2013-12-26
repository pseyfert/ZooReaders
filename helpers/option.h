/** @class option
 *  @brief templated class for command line a command line option for executables
 *
 *  @author Paul Seyfert
 *  @date   2011-12-25
 */

/** @class virtualoption
 *  @brief abstract base class for option
 *
 *  @author Paul Seyfert
 *  @date   2011-12-25
 */

/** @class options
 *  @brief container class for option
 *
 *  @author Paul Seyfert
 *  @date   2011-12-25
 */

#ifndef OPTION_H
#define OPTION_H 1

#include <vector>
#include <cstring>
#include <stdlib.h>
//#include <boost/foreach.hpp>
#include "logstream.h"
#include <iostream>

template <class T>
int dummy(T) ;

class virtualoption {
  public:
    virtual int initialize() =0;
    virtual void shortinitialize() =0;
    virtual ~virtualoption() ;
    virtual char callme() =0;
    virtual bool isbool() =0;
    virtual bool isneeded() =0;
    virtual bool isset() =0;
    virtualoption() ;
    virtual void set(char*) =0;
    virtual void set() =0;
    virtual void help() =0;
    virtual int apply() =0;
    virtual void enter_helpmessage(std::string) =0;
};


template <class T>
class option : public virtualoption {

  /**
   * @brief a void function to be executed in initialize
   *
   * @param whatever will be returned by value()
   *
   * @return no return
   */
  typedef int (*myfun)(T);
  private:
    char m_caller;
    T* m_value;
    T storage;
    T default_value;
    bool m_set;
    bool m_needed;
    std::string helpmessage;
    myfun function;
  public:
    void help() ;
    T value()  ;
    int initialize();
    void shortinitialize();
    void set();
    void set(T);
    void set(char* value);// {set((T)value);}
    char callme() ;
    bool isbool();
    bool isset() ;
    bool isneeded() ;
    void enter_helpmessage(std::string message) ;
    /// call voidfun
    int apply() ;
    option(char caller);
    option(char caller, T def);
    option(char caller, T def, T* value);
    option(char caller, T def, T* value, bool need);
    option(char caller, T def, T* value, bool need, myfun function);
    option(char caller, T def, bool need);
    option(char caller, T def, bool need, myfun function);
    option(char caller, T def, myfun function);
    ~option();
};


class options {
  private:
    std::vector<virtualoption*> vec;
    std::vector<std::string> overflow;
    bool m_needoverflow;
    bool m_printhelp;
    std::string helpmessage;
  public:
    template<class T>
    option<T>* push_back(option<T>*);
    void help();
    void show_settings();
    void enter_helpmessage(std::string message) ;
    int initialize() ;
    int parse(int argc, char** argv);
    void set_needsOverflow(bool n) ;
    bool overflow_needed() ;
    options() ;
    options(bool needs_overflow) ;
    std::vector<std::string> rest() ;
    virtualoption* get_option(const char caller);
};
#endif
