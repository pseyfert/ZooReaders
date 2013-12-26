#include "Zoo/Zoo.h"
#include "TChain.h"
#include "helpers/treefunctions.h"
#include "helpers/logstream.h"
#include "helpers/option.h"
#include "helpers/glob.h"

std::string branchname = "b";

int main(int argc, char** argv) {
  ZooEv* pev = 0;
  ZooObjRefArray<ZooP> *parts = 0;

  glob_options.enter_helpmessage("are input file names");
  glob_options.push_back<std::string>(new option<std::string>('b',"b",&branchname))->enter_helpmessage("your decay name");
  glob_options.set_needsOverflow(true);

  {
    int parsererror = glob_options.parse(argc,argv);
    if (parsererror>1) {
      glob_options.help();
      glob_options.show_settings();
    }
    if (parsererror) {
      return parsererror;
    }
  }


  TChain* chain(new TChain("Forest"));
  for (auto file : glob_options.rest()) {
    chain->Add(file.c_str());
  }
  chain->GetEntries();
  chain->SetBranchAddress("Event",&pev);
  int branchstatus = chain->SetBranchAddress((dynamic_cast<option<std::string>*>(glob_options.get_option('b')))->value().c_str(),&parts);
  if (TTree::kMissingBranch == branchstatus || 0u == (dynamic_cast<option<std::string>*>(glob_options.get_option('b')))->value().length()) {
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
