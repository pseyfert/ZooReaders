#ifndef FUNCTORHANDLE_H
#define FUNCTORHANDLE_H 1

/** @class FunctorHandle
 *  wrapper to hold a ZooFunctor and a corresponding histogram
 *
 * @author Paul Seyfert
 * @date   2012-01-21
 */

#include "ZooFunctor.h"

class FunctorHandle {
  private:
    ZooFunctor* m_fun;
  public:
    FunctorHandle();
    FunctorHandle(std::string, ZooFunctor*, unsigned, double, double);
    ~FunctorHandle();
    std::string name() {return m_fun->name;}
    void Fill(const ZooP* p, const ZooEv* ev, double w = 1.);
    double m_low;
    double m_high;
    unsigned m_bins;
    TH1*     histo() {return hist;}
    TH1*     hist;
};

#endif
