#include "TROOT.h"
#include "Zoo/Zoo.h"
#include <TApplication.h>
#include <TFile.h>
#include <iostream>
#include <TChain.h>
#include <TGComboBox.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGFrame.h>
#include <TGProgressBar.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TSystem.h>
//#ifndef __MAKECINT__
//#ifdef __CINT__
//gSystem->Load("Zoo/libZooROOT.so");
//gSystem->Load("ZooFunctors/libZooFunctorROOT.so");
//#endif

#ifndef __CINT__
// this include alone is not all you need to use ZooGui with ACLiC.
#include "Zoo/Zoo.h"
#include "ZooFunctors/ZooFunctor.h"
#include "ZooFunctors/functions.h"
#endif
#include <TPad.h>
#include <TStyle.h>
#include <TNtuple.h>
#include <fstream>
#include <utility>
#include <vector>
#include <limits>
#include "ZooFunctors/comb.h"

float flight(const ZooP* particle) ;

float nmcp(const ZooP*, const ZooEv* e) ;

float etatest(const ZooP* p) ;

float andy(const ZooP* zp) ;


float somept(const ZooP* p) ;

float maxpt(const ZooP* p) ;

float mctruth(const ZooP* p) ;

float minips(const ZooP* p) ;
float mumumass1(const ZooP* p) ;

float mumumass2(const ZooP* p) ;

float mumumass3(const ZooP* p) ;
float trackchi2(const ZooP* p) ;

float spam(const ZooP*, const ZooEv* pev) ;
float runnumber(const ZooP* , const ZooEv* pev) ;


float geo(const ZooP* p) ;
float pid(const ZooP* p) ;

