#include "treefunctions.h"
#include "TTree.h"
#include "logstream.h"

std::vector<std::string> getBranchNames(TTree* tree) {
  std::vector<std::string> zweignamen;
  zweignamen.clear();
  if (NULL==tree) return zweignamen;
  TObjArray* zweige = tree->GetListOfBranches();
  logstreams::debug << "REMEMBER: AUTOMATIC BRANCHDETECTION FAILS IF YOUR DECAY IS CALLED \"Zoo...\"" << std::endl;
  for ( int ind = 0 ; ind < zweige->GetEntries() ; ++ind) {
    if ((zweige->At(ind)->GetName())[0]=='Z'&&(zweige->At(ind)->GetName())[1]=='o'&&(zweige->At(ind)->GetName())[2]=='o')
      continue;
    if (!(strcmp(zweige->At(ind)->GetName(),"Event"))) continue;
    zweignamen.push_back(std::string(zweige->At(ind)->GetName()));
  }
  return zweignamen;

}
