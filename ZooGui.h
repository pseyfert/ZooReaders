/** @class ZooGui ZooGui.h
 *  graphical user interface to take a quick look at Zoontuples
 *
 * @author Paul Seyfert
 * @date   2011-10-28
 */

#ifndef ZOOGUI_H
#define ZOOGUI_H 1

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
#include "funs.h"
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
//#include "/home/pseyfert/phd/dropbox/rd/2011-09-12/select.h"
//#include "lhcbstylemod.h"
class ZooP;
class ZooEv;
class ZooFunctor;

// for demonstration purpose
class ZooGui : public TQObject {
  public:
    virtual ~ZooGui ();
    ZooGui()      {
#ifdef __CINT__
      gSystem->Load("Zoo/libZooROOT.so");
      gSystem->Load("ZooFunctors/libZooFunctorROOT.so");
#endif
      file = NULL; kette = NULL; branchcombo = NULL; fMainFrame = NULL;can = NULL;enterfilename=NULL;doit=NULL;methodcombo=NULL;maxentries=NULL;
      persistify=NULL;
      mfilename=std::string("");maxnumber=0;

      meths.push_back(new ZooPointer(&somept,"somept"));
meths.push_back(new ZooPointer(&TRACKghost,"ghostprobability"));
meths.push_back(new ZooChild(new ZooPointer(&TRACKghost,"ghostprobability"),0));
meths.push_back(new ZooPointer(&trackchi2,"trackchi2"));

      meths.push_back((new ZooParticleInfoFunctor(666)));
  meths.push_back(new ZooPointer(&bpvipchi2,"BPVIPCHI2"));
  meths.push_back(new ZooPointer(&bpvltime,"BPVLTIME225"));
  meths.push_back((new ZooPointer(&dira,"dira")));

  //
  //

  meths.push_back((new ZooChild(new ZooPointer(&ismuon,"isMuon"),0)));
  meths.push_back((new ZooChild(new ZooPointer(&ismuon,"isMuon"),1)));
  meths.push_back((new ZooChild(new ZooPointer(&ismuon,"isMuon"),2)));

  meths.push_back(new ZooPointer(&L0TIS,"L0TIS"));
  meths.push_back(new ZooPointer(&L0TOS,"L0TOS"));
  meths.push_back(new ZooPointer(&Hlt1TIS,"Hlt1TIS"));
  meths.push_back(new ZooPointer(&Hlt1TOS,"Hlt1TOS"));
  meths.push_back(new ZooPointer(&Hlt2TIS,"Hlt2TIS"));
  meths.push_back(new ZooPointer(&Hlt2TOS,"Hlt2TOS"));

  meths.push_back((new ZooEventPointer(&eventNR,"eventNumber")));

      meths.push_back(new ZooEventPointer(&ZooEv::polarity,"magnet"));
      meths.push_back(new ZooChild(new ZooCloneDist(),1));
      //meths.push_back(new ZooChild(new ZooSingleMuon(),0));
      meths.push_back(new ZooChild(new ZooCloneDist(),0));
      meths.push_back(new ZooChild(new ZooCloneDist(),2));
      meths.push_back(new ZooPointer(&geo,"GL"));
      meths.push_back(new ZooPointer(&pid,"PID"));
      meths.push_back(new ZooMass());
      meths.push_back(new ZooPointer(&mctruth,"mctruth"));
      meths.push_back(new ZooEventPointer(&nmcp,"MCP"));
      meths.push_back(new ZooOLeg(new ZooPointer(&andy,"andreas")));
      meths.push_back(new ZooPointer(&etatest,"etatest"));
      meths.push_back(new ZooPointer(&mumumass1,"MuMuMass1"));
      meths.push_back(new ZooPointer(&minips,"minmuIPs"));
      meths.push_back(new ZooPointer(&mumumass2,"MuMuMass2"));
      meths.push_back(new ZooPointer(&mumumass3,"MuMuMass3"));
      meths.push_back(new ZooPointer(&maxpt,"maxpt"));
      meths.push_back(new ZooChild((ZooFunctor*)new ZooRealTrackChi(),1));
      meths.push_back(((ZooFunctor*)new ZooRealTrackChi()));
      meths.push_back(new ZooChild(new ZooInfoFunctor(10004),1));
      meths.push_back(new ZooChild(new ZooInfoFunctor(10005),1));
      meths.push_back(new ZooChild(new ZooInfoFunctor(10006),1));
      meths.push_back(new ZooChild(new ZooInfoFunctor(10007),1));
      meths.push_back(new ZooChild(new ZooInfoFunctor(10020),1));
      meths.push_back(new ZooChild(new ZooInfoFunctor(10021),1));
      meths.push_back(new ZooChild(new ZooInfoFunctor(10022),1));
      meths.push_back(new ZooChild(new ZooParticleInfoFunctor(10030),1));
      meths.push_back((new ZooParticleInfoFunctor(10030)));
      meths.push_back((new ZooParticleInfoFunctor(10031)));
      meths.push_back((new ZooParticleInfoFunctor(2)));
      meths.push_back((new ZooParticleInfoFunctor(1)));
      meths.push_back(new ZooChild(new ZooParticleInfoFunctor(10031),1));
      meths.push_back(new ZooBestSelect(new ZooLeg(new ZooLinkkey())));
      meths.push_back(new ZooBestSelect(new ZooLeg(new ZooLinkfraction(0.75))));
      meths.push_back((new ZooLeg(new ZooLinkfraction(0.75))));
      meths.push_back((new ZooLeg(new ZooLinkfraction(0.20))));
      meths.push_back(new ZooLeg(new ZooPointer(&expectedT,"Texpectation")));
      meths.push_back(new ZooLeg(new ZooPointer(&expectedTT,"TTexpectation")));
      meths.push_back(new ZooLeg(new ZooBestLink(new ZooRealTrackChi(),0.75)));

      meths.push_back(new UpdatedMass());
      meths.push_back(new ZooBestSelect(new ZooMass()));
      meths.push_back(new NewIPCut());
      meths.push_back(new ZooBestSelect(new ZooOLeg(new ZooIPSig())));
      meths.push_back(new ZooBestSelect(new ZooMichel()));
      meths.push_back(new ZooBestSelect(new Zoo4040Mass()));
      meths.push_back(new ZooBestSelect(new ZooUnconfirmed()));
      meths.push_back(new ZooLeg(new ZooTISTOS()));
      meths.push_back(new ZooOLeg(new ZooTISTOS()));
      meths.push_back(new ZooAssoc());
      meths.push_back(new ZooMomentum());
      meths.push_back(new ZooIP());
      meths.push_back(new ZooIPSig());
      meths.push_back(new ZooVertChi());
      meths.push_back(new ZooTrackFive());
      meths.push_back(new ZooInfoFunctor(4040));
      meths.push_back(new ZooInfoFunctor(4445));
      meths.push_back(new ZooInfoFunctor(4446));
      meths.push_back(new ZooInfoFunctor(4447));
      meths.push_back(new ZooInfoFunctor(4448));
      meths.push_back(new ZooLeg(new ZooIP()));
      meths.push_back(new ZooLeg(new ZooInfoFunctor(4445)));
      meths.push_back(new ZooLeg(new ZooInfoFunctor(4446)));
      meths.push_back(new ZooLeg(new ZooInfoFunctor(4447)));
      meths.push_back(new ZooLeg(new ZooInfoFunctor(4448)));
      meths.push_back(new ZooLeg(new ZooInfoFunctor(4040)));
      meths.push_back(new ZooLeg(new ZooInfoFunctor(4001)));
      meths.push_back(new ZooConfirmedVeloCalo(new ZooMass()));
      meths.push_back(new ZooLeg(new ZooMass()));
      meths.push_back(new ZooLeg(new ZooMomentum()));
      meths.push_back(new ZooLeg(new ZooIP()));
      meths.push_back(new ZooNPV());
      meths.push_back(new ZooLeg(new ZooPt()));
      meths.push_back(new ZooLeg(new ZooEta()));
      meths.push_back(new ZooLeg(new ZooPhi()));
      meths.push_back(new ZooOldCut(new ZooLeg(new ZooPt())));
      meths.push_back(new ZooOldCut(new ZooLeg(new ZooEta())));
      meths.push_back(new ZooOldCut(new ZooLeg(new ZooPhi())));
      meths.push_back(new ZooOldCut(new ZooBestSelect(new ZooMichel())));
      meths.push_back(new ZooOTHits());
      // to make a more failsafe no-need-to-change-the-code demonstration of the ZooPointer see the following lines.
      // They do not work in the interpreted mode (=CINT .L ZooGui.cpp) but in the compiled mode (=ACLiC .L ZooGui.cpp+)
      // There are different constructors of ZooPointer, either with a float valued ZooP method:
      //       typedef float (ZooP::*func)()const;
      // i.e. it must be something you call
      //       const ZooP* particle;
      //       particle->demo();
      // or it is a float valued function with a const ZooP* as argument:
      //       typedef float (*gfunc)(const ZooP*);
      // i.e. something which is:
      //       float demo(const ZooP*) ;
#ifndef __CINT__
      meths.push_back(new ZooPointer(&trackchi2,"thetrackschi2perndf"));
      meths.push_back(new ZooPointer(&ZooP::ct,"LifeTime"));
      meths.push_back(new ZooPointer(&flight,"FlightDistance"));
      meths.push_back(new ZooEventPointer(&ZooEv::trackmult,"trackmult"));
      meths.push_back(new ZooEventPointer(&spam,"TriggerWord"));
      //		 meths.push_back(new ZooEventPointer(&hlt2triggeredmass,"hlt2triggered"));
      //		 meths.push_back(new ZooEventPointer(&hlt2triggeredmass,"triggered"));
      //		 meths.push_back(new ZooEventPointer(&nottriggeredmass,"untriggered"));
      meths.push_back(new ZooEventPointer(&runnumber,"runnumber"));
#endif
      meths.push_back(new Zoo4040Mass());
      meths.push_back(new ZooUnconfirmed());
      meths.push_back(new Zoo4448());
      meths.push_back(new ZooMichel());
      //applystyle();
#ifndef __CINT__
      std::cout << "not in cint" << std::endl;
#endif
#ifdef __MAKECINT__
      std::cout << "in makecint" << std::endl;
#endif
      generalmass = new ZooMass();//ZooBestSelect(new ZooMichel());
    }
    TFile*        file;              // the file which is opened
    std::string   branchname;        // store the branch to be used
    std::string   mfilename;         // store filename
    TChain*       kette;             // the TChain which is read to be filled into the histogram
  public:
    std::vector<ZooFunctor*> meths;
    /* obsolete. were used for other implementation
       private:
       float         momentum(const ZooP* part) {return part->momentum().P();}
       float         ip(const ZooP* part) {return part->ip();}*/
  private:
    ZooFunctor*   generalmass;
    float         mass(const ZooP* part) {return generalmass->eval(part);}
    std::vector<std::string>   GetBranchNames();
  public:
    void          build();
    void          LoadFile() {std::string feld(enterfilename->GetText());std::cout << "now I am gonna load a file: " << enterfilename->GetText() << "\t" << feld << std::endl;
      LoadFile(feld.c_str());
      printf("end of LoadFile()\n");
    }
    void          LoadFile(const char* filename);
    void          buildCombo();
    void          sayhello() {std::cout << "Hello there!\n";} // Hello World!
    void          plot();
    void          hardcopy();
    TNtuple*      ntup;
    void          setbranch(const char* text) {std::string s(text);branchname = s ;std::cout << "changed branch to " << s <<std::endl;}
  private:
    TCanvas*      can;               // canvas which is used for plotting
    TGTextEntry*  enterfilename;     // text field of filename
    TGComboBox*   branchcombo;       // combobox of branchnames
    TGMainFrame*  fMainFrame;        // the ZooGui control window
    TGComboBox*   methodcombo;       // combobox of plottable functions
    TGTextButton* doit;              // DRAW button
    TGTextButton* persistify;        // hardcopy
    TGHProgressBar* progress;         // progressbar
    TGNumberEntry*  maxentries;         // how many entries to process
    int           chosenmethod;      // buffer of the chosen function
    unsigned int  maxnumber;
    //  void          SetMaxEntries(int);
    int           max_methods() {
      return meths.size();}// for loops over all functions. implemented as method to change implementation (has changed often).
    float         method(const ZooP* part) {
      return meths[chosenmethod]->eval(part);
    }
    float		method(const ZooP* part, const ZooEv* event) {
      return meths[chosenmethod]->eval(part,event);
    }
    std::string   methodname(int i) {
      return meths[i]->name;
    }
  public:
    void          choose(int);
    ClassDef(ZooGui,1);
};


#endif
