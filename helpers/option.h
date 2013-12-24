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

typedef void (*voidfun)();
void dummy() {
}

class virtualoption {
  public:
    virtual void initialize() =0;
    virtual void shortinitialize() =0;
    virtual ~virtualoption() {};
    virtual char callme() =0;
    virtual bool isbool() =0;
    virtual bool isneeded() =0;
    virtual bool isset() =0;
    virtualoption() {};
    virtual void set(char*) =0;
    virtual void set() =0;
    virtual void help() =0;
    virtual void apply() =0;
    virtual void enter_helpmessage(std::string) =0;
};


template <class T>
class option : public virtualoption {
  private:
    char m_caller;
    T* m_value;
    T storage;
    T default_value;
    bool m_set;
    bool m_needed;
    std::string helpmessage;
    voidfun function;
  public:
    void help() {std::cout << "option -" << callme() << "\t" << helpmessage << std::endl;}
    T value() {if (m_set) return *m_value; else return default_value;} ;
    void initialize();
    void shortinitialize();
    void set();
    void set(T);
    void set(char* value);// {set((T)value);}
    char callme() {return m_caller;};
    bool isbool();
    bool isset() {return m_set;};
    bool isneeded() {return m_needed;};
    void enter_helpmessage(std::string message) {helpmessage = message;};
    /// call voidfun
    void apply() {function();}
    option(char caller);
    option(char caller, T def);
    option(char caller, T def, T* value);
    option(char caller, T def, T* value, bool need);
    option(char caller, T def, T* value, bool need, voidfun function);
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
    void enter_helpmessage(std::string message) {helpmessage = message;};
    void initialize() ;
    void parse(int argc, char** argv);
    options() ;
    options(bool needs_overflow) ;
    std::vector<std::string> rest() {return overflow;}
    virtualoption* get_option(const char caller);
};
#endif
