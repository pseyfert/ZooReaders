/** @class ZooReader
 *  @brief base class for my ZooNtuple reading helpers
 *
 * Usage: inherit from ZooReader and place the job-specific code
 * in initialize(), finalize() and ProcessOneEvent()
 * alternatively you can also override ProcessAllEvents()
 * in you job, instantiate you class and call
 * AddFile(std::string filename) to read files (use argv[i] for command line setting)
 * choose the branch (SetBranchName(std::string) or AskForBranch() or through the command line)
 * initialize(), ProcessAllEvents() and finalize()
 *
 * for examples see ZooReader.cc
 *
 *  @author Paul Seyfert
 *  @date   2011-12-03
 */
#ifndef ZOOREADER_H
#define ZOOREADER_H 1
#include <vector>
#include <string>
#include <TChain.h>


#ifndef __CINT__
// this include alone is not all you need to use ZooGui with ACLiC.
#include "../Zoo/Zoo.h"
#include "ZooFunctor.h"
#include "functions.h"
#include "likely.h"
#endif

class ZooReader {
 private:
  Long64_t m_entry;
 //typedef TTree::SetBranchAddressStatus branchstatus;
 protected:
  unsigned int m_evtmax;
  ///@todo simplify most simple class by moving this to derived class?
  std::vector<ZooFunctor*> meths;
 public:
  Long64_t                 whichEntry() {return m_entry;}
 typedef std::vector<std::string> stringvector;
  virtual void             initialize();
  virtual void             finalize();
  /// @todo this went to helper package
  std::vector<std::string> GetBranchNames();
  /// @todo revisit LoadFile and AddFile splitting
  /// loads a single zoontuple file. Discards previously read files
  virtual int              LoadFile(TString filename);
  /// add another file to the current chain
  virtual int              AddFile(TString filename);
  /// print all decay branches from the current chain
  virtual stringvector     PrintBranches();

  /// ask user in command line which branch to process
  ///@todo return type?
  ///@todo move to derived class
  #ifdef FIXEDTTREE
  virtual TTree::SetBranchAddressStatus     AskForBranch();
  virtual TTree::SetBranchAddressStatus     SetBranchName(std::string);
  #endif
  #ifndef FIXEDTTREE
  virtual int     AskForBranch();
  virtual int     SetBranchName(std::string);
  #endif

  virtual void             SetOutputFilename(std::string);

  ///@todo why is this not purely abstract
  /// dummy to be replaced by the user. return 0 if everything went fine. encode errors binary.
  virtual int              ProcessOneEvent() {
    return 0;
  }
  /// eventwrapper to let the user concentrate on the event itself. collects errorcodes
  virtual int              ProcessAllEvents() {
    int collectederrorcode = 0;
    if (!m_tree) return 99;
    unsigned int max = m_tree->GetEntries();
    if (m_evtmax>0)
      max = std::min(m_evtmax,max);
    std::cout << "Requested " << max << " events" << std::endl;
    unsigned int steps = 20;
    unsigned int percentage = 100/steps;
    unsigned int stepwidth = max/steps;
    if (max<steps) stepwidth=1u;
    for ( m_entry = 0 ; m_entry < max ; ++m_entry) {
      if (0ul == (m_entry % stepwidth)) {
        unsigned int fraction = m_entry/stepwidth * percentage;
        std::cout << "Reading event " << m_entry << "\t(" << fraction << "%)" << std::endl;
      }
      m_tree->GetEntry(m_entry);
      collectederrorcode |= ProcessOneEvent();
    }
    return collectederrorcode;
  }

  ZooReader() ;
  virtual ~ZooReader() ;
 protected:
  TChain*                  m_tree;
  ///@todo these three go to derived classes
  TFile*                   m_outfile;
  ZooObjRefArray<ZooP>*    m_branch;
  ZooEv*                   m_pev;
 public:
  TString                  m_branchname;
};
#endif
