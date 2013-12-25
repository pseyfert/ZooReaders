#ifndef MYKIN_H
#define MYKIN_H 1

class TDirectory;
/**
 * @class MyKin
 * My (Paul Seyfert) version of the glorious KinPlot
 *
 * @author Paul Seyfert
 * @date   2012-01-21
 */

#include "FunctorHandle.h"
//class ZooP;

class MyKin {
  private:
    std::vector<FunctorHandle*> meths;
    std::string m_name;
    void construct(const ZooP* example = NULL);
  public:
    MyKin();
    MyKin(std::string name, const ZooP* example);
    ~MyKin();
    void Fill(const ZooP* p , const ZooEv* ev, double w = 1.);
    void Write(TDirectory* dir);
    void SetName(std::string name);
};

#endif

