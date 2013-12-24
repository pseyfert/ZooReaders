#include "Zoo/Zoo.h"
#include "TChain.h"
#include "helpers/treefunctions.h"
#include "helpers/logstream.h"
#include "helpers/option.h"

std::string branchname = "b";
options opts(true);

int main(int argc, char** argv) {
  ZooEv* pev = 0;
  ZooObjRefArray<ZooP> *parts = 0;

  opts.enter_helpmessage("are input file names");
  opts.push_back<std::string>(new option<std::string>('b',"b",&branchname))->enter_helpmessage("your decay name");

  if (argc==1) {
    opts.help();
    return 1;
  }
  opts.parse(argc,argv);


  TChain* chain(new TChain("Forest"));
  for (auto file : opts.rest()) {
    chain->Add(file.c_str());
  }
  chain->GetEntries();
  chain->SetBranchAddress("Event",&pev);
  int branchstatus = chain->SetBranchAddress(branchname.c_str(),&parts);
  if (TTree::kMissingBranch == branchstatus || 0u == branchname.length()) {
    std::vector<std::string> branchnames = getBranchNames((TTree*)chain);
    logstreams::error << "desired branch is not available!" << std::endl;
    logstreams::warn << "available branches are:" << std::endl;
    for (auto name : branchnames) {
      logstreams::warn << "  * " << name << std::endl;
    }
    return 2;
  } else {
    logstreams::debug << "branchstatus is " << branchstatus << std::endl;
  }
  return 0;
}
