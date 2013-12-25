#include "FunctorHandle.h"
#include <TH1D.h>

FunctorHandle::FunctorHandle() {
  std::cerr << "You shouldn't do that!" << std::endl;
}

FunctorHandle::FunctorHandle(std::string prefix, ZooFunctor* fun, unsigned bins, double low, double high) {
  m_fun = fun;
  m_low = low;
  m_high = high;
  m_bins = bins;
  std::string thename = prefix + name();
  hist = new TH1D(thename.c_str(),name().c_str(),bins,low,high);
}

FunctorHandle::~FunctorHandle() {
  delete m_fun;
}

void FunctorHandle::Fill(const ZooP* part, const ZooEv* ev, double w) {
  hist->Fill(m_fun->eval(part,ev),w);
}
