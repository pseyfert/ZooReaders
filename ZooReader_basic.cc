#include "Zoo/Zoo.h"
#include "TChain.h"
#include "helpers/treefunctions.h"
#include "helpers/logstream.h"

std::string branchname = "b";

int main(int argc, char** argv) {
  ZooEv* pev = 0;
  ZooObjRefArray<ZooP> *parts = 0;
  if (argc==1) {
    std::cout << "USAGE " << argv[0] << " <zoontuple(s)> " << std::endl;
    return 1;
  }
  TChain* chain(new TChain("Forest"));
  for (int i = 1 ; i < argc ; ++i) {
    chain->Add(argv[i]);
  }
  chain->GetEntries();
  chain->SetBranchAddress("Event",&pev);
  int branchstatus = chain->SetBranchAddress(branchname.c_str(),parts);
  if (TTree::kMissingBranch == branchstatus || 0u == branchname.length()) {
    std::vector<std::string> branchnames = getBranchNames((TTree*)chain);
    logstreams::error << "desired branch is not available!" << std::endl;
    logstreams::warn << "available branches are:" << std::endl;
    for (unsigned b = 0u ; b < branchnames.size() ; ++b) {
      logstreams::warn << "  * " << branchnames[b] << std::endl;
    }
    return 2;
  } else {
    logstreams::debug << "branchstatus is " << branchstatus << std::endl;
  }
  return 0;
}
