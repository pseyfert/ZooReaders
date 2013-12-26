#include "ZooReader.h"

void ZooReader::initialize() {
  if (!m_tree) {
    std::cerr << "initialization must be done after the file(s) is/are loaded" << std::endl;
    return;
  }
  m_tree->SetBranchAddress("Event",&m_pev);
  if (m_branchname.Length())
    m_tree->SetBranchAddress(m_branchname, &m_branch);
  else
    std::cout << "No branchname set. assume this is not needed" << std::endl;
  return ;
}

void ZooReader::finalize() {
  return;
}

std::vector<std::string> ZooReader::GetBranchNames() {
  std::vector<std::string> zweignamen;
  zweignamen.clear();
  if (!m_tree) return zweignamen;
  TObjArray* zweige = m_tree->GetListOfBranches();
  std::cout << "REMEMBER: THIS AUTOMATIC BRANCHDETECTION FAILS IF YOUR DECAY IS CALLED \"Zoo...\"" << std::endl;
  for ( int ind = 0 ; ind < zweige->GetEntries() ; ++ind) {
    if ((zweige->At(ind)->GetName())[0]=='Z'&&(zweige->At(ind)->GetName())[1]=='o'&&(zweige->At(ind)->GetName())[2]=='o')
      continue;
    if (!(strcmp(zweige->At(ind)->GetName(),"Event"))) continue;
    zweignamen.push_back(std::string(zweige->At(ind)->GetName()));
  }
  return zweignamen;
}

int ZooReader::AddFile(TString filename) {
/// error codes
/// ===========
/// 0 : everything fine
/// 2 : AddFile method returned an error

  if (!m_tree) {
    //std::cerr << "NO TREE LOADED!" << std::endl;
    m_tree = new TChain("Forest");
  }
  if (1==m_tree->AddFile(filename))
    return 0;
  else {
    std::cerr << "error returned from ROOT" << std::endl;
    return 2;
  }
}

int ZooReader::LoadFile(TString filename) {
/// error codes
/// ===========
/// 0 : everything fine
/// 2 : AddFile method returned an error


  if (m_tree) delete m_tree;
  m_tree = NULL;
  m_tree = new TChain("Forest");
  return AddFile(filename);
}

std::vector<std::string> ZooReader::PrintBranches() {
  std::vector<std::string> branchnames = GetBranchNames();
  for (unsigned i = 0 ; i < branchnames.size() ; ++i)
    std::cout << "\t" << i << ")\t" << branchnames[i] << std::endl;
  return branchnames;
}

#ifdef FIXEDTTREE
TTree::SetBranchAddressStatus ZooReader::AskForBranch() {
#endif
#ifndef FIXEDTTREE
int ZooReader::AskForBranch() {
#endif
  std::vector<std::string> branchnames = PrintBranches();
  if (branchnames.size() > 1 && m_branchname.Length() == 0) {
    std::cout << std::endl << "Which branch do you want to process?\n";
    unsigned int choice;
    /// @todo: check user choice's sanity
    std::cin >> choice;
    if (choice < branchnames.size())
      return SetBranchName(branchnames[choice]);
  } else if (branchnames.size() == 1) {
    std::cout << "only one branch. Taking the obvious choice: " << branchnames[0] << std::endl;
    return SetBranchName(branchnames[0]);
  }
#ifdef FIXEDTTREE
  return TTree::kMissingBranch;
#endif
#ifndef FIXEDTTREE
  return 0;
#endif
}

void ZooReader::SetOutputFilename(std::string name) {
  if (m_outfile && m_outfile->IsOpen()) m_outfile->Close();
  if (m_outfile) delete m_outfile;
  m_outfile = new TFile(name.c_str(),"recreate");
}

ZooReader::ZooReader() : m_tree(0), m_branch(0), m_pev(0){
  m_evtmax = 0;
  m_outfile = NULL;
}

ZooReader::~ZooReader() {
  std::cout << "ZooReader::~ZooReader()" << std::endl;
  if (m_outfile) {
    if (m_outfile->IsOpen()) m_outfile->Close();
    delete m_outfile;
    //m_outfile->Close();
  }
  for (unsigned int k = 0 ; k < meths.size() ; ++k)
    delete meths[k];
  meths.clear();
}

#ifdef FIXEDTTREE
TTree::SetBranchAddressStatus ZooReader::SetBranchName(std::string branchname) {
#endif
#ifndef FIXEDTTREE
int ZooReader::SetBranchName(std::string branchname) {
#endif
  m_branchname = branchname;
  return m_tree->SetBranchAddress(branchname.c_str(),&m_branch);
}
