#include "MyKin.h"
#include <TDirectory.h>
//#include <boost/foreach.hpp>
#include "functions.h"

void MyKin::Fill(const ZooP* p, const ZooEv* ev, double w) {
  //BOOST_FOREACH(FunctorHandle* handle, meths) {
  for (unsigned jj = 0 ; jj < meths.size() ; ++jj) {
    FunctorHandle* handle = meths[jj];
    handle->Fill(p,ev,w);
  }
}

MyKin::MyKin() {
  m_name = std::string("unnamed");
  construct();
}

void MyKin::SetName(std::string name) {
  m_name = name;
}

MyKin::MyKin(std::string name, const ZooP* example) {
  m_name = name;
  construct(example);
}

MyKin::~MyKin() {
  //BOOST_FOREACH(FunctorHandle* handle,meths) {
  for (unsigned jj = 0 ; jj < meths.size() ; ++jj) {
    FunctorHandle* handle = meths[jj];
    delete handle;
  }
  meths.clear();
}

void MyKin::construct(const ZooP* example) {
  
  meths.push_back(new FunctorHandle(m_name, new ZooMass(), 2000,000.,6000.));
  meths.push_back(new FunctorHandle(m_name, new ZooMomentum(), 200,0.,250000.));
  meths.push_back(new FunctorHandle(m_name, new ZooPt(), 200,0.,25000.));
  meths.push_back(new FunctorHandle(m_name, new ZooEta(), 100,1.5,6));
  meths.push_back(new FunctorHandle(m_name, new ZooPhi(), 100,-TMath::Pi(),TMath::Pi()));
  /// @fixme push far more

  if (example && example->DLL()) {
    meths.push_back(new FunctorHandle(m_name, new ZooMDll(), 100, -20., 20.));
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&muk,"DLL(#mu-K)"), 100, -20., 20.));
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&kpi,"DLL(K-#pi)"), 100, -20., 20.));
  }
  if (example && example->St()) {
    meths.push_back(new FunctorHandle(m_name, new ZooRealTrackChi(), 20, 0., 10.));
  }
  if (example && example->DLL()) {
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&richK,"RICH kaonLL"),100,-100.,100.));
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&richMU,"RICH muonLL"),100,-100.,100.));
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&ecalMU,"ECAL muonLL"),100,-5.,5.));
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&hcalMU,"HCAL muonLL"),100,-5.,5.));
  }

  if (example && example->Dc()) {
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&ZooP::ct,"LifeTime"), 200, 0., 0.1));
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&ZooP::ctErr,"LifeTimeError"), 200, 0., 0.1));
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&ZooP::chi2,"VertexChi2"), 20, 0., 10.));
    meths.push_back(new FunctorHandle(m_name, new ZooPointer(&ZooP::nDoF,"VertexDoF"), 20, 0., 10.));
  } 
}

void MyKin::Write(TDirectory* dir) {
  //BOOST_FOREACH(FunctorHandle* handle,meths) {
  std::cout << "Writing kinematic plots to " << dir->GetPath() << std::endl;
  for (unsigned int i = 0 ; i < meths.size() ; ++i) {
    //std::cout << "method " << i << std::endl;
    //std::cout << "is called " << meths[i]->name() << std::endl;
    //dir->WriteTObject((const TObject*)(handle->hist));
    if (0==dir->WriteTObject((const TObject*)(meths[i]->histo())))
      std::cerr << "trouble writing " << meths[i]->histo()->GetName() << std::endl;
    else
      std::cout << "wrote " << meths[i]->histo()->GetName() << std::endl;
  }
  return;
}
