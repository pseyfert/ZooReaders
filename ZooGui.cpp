#include "ZooGui.h"

void ZooGui::hardcopy() {
  if (can) {
    std::string name(Form("/tmp/%ld.eps",time(NULL)));
    can->Print(name.c_str());
    std::string name2(Form("/tmp/%s_%s.eps",branchname.c_str(),methodname(chosenmethod).c_str()));
    can->Print(name2.c_str());
    char buf[256];
    sprintf(buf, "gv %s &",name.c_str());
    system(buf);
  }
}
ZooGui::~ZooGui() {}
void ZooGui::plot() {
  //  if (!strcmp("",branchname.c_str())) return;
  if (can) delete can;
  /*TStyle* stylebuffer = gStyle;
    gROOT->SetStyle("lhcbStyle");
    gROOT->ForceStyle();*/
  TVirtualPad* padbuffer=NULL;
  // this does not work if gPad has not been set before… root is strange
  //  if (gPad) padbuffer = gPad;
  can = new TCanvas();
  ZooObjRefArray<ZooP>* mybranch = 0;
  ZooEv* pev = 0;
  kette->ResetBranchAddresses();  // this line is somehow important when branches are switched
  kette->SetBranchAddress(branchname.c_str(),&mybranch);
  kette->SetBranchAddress("Event",&pev);
  progress->Reset();
  TFile* ff = new TFile("/tmp/buffer.root","recreate");
  ff->cd();
  ntup = new TNtuple("quickview","Quickview of ZooNtuple",Form("%s:mass",methodname(chosenmethod).c_str()));
  unsigned int j = 0 ;
  unsigned int eintraege = kette->GetEntries();
  maxnumber = maxentries->GetIntNumber();
  if (maxnumber && maxnumber < eintraege) eintraege = maxnumber;
  progress->SetRange(0.,eintraege);
  //  std::cout << "start to range" << std::endl;
  for (unsigned int i = 0 ; i < eintraege ; ++i) {
    //    kette->LoadTree(i);
    kette->GetEntry(i);
    if (!(i%10))
      progress->SetPosition((float)i);
    //if (progress->GetPosition() != (float)i) std::cout << "HEY";
    for (unsigned int m = 0 ; m < mybranch->size() ; ++m) {
      //      ZooP* candidate = const_cast<ZooP*> ((*mybranch)[m]);
      //      std::cout << "nasty point around" << std::endl;
      const ZooP* candidate = (*mybranch)[m];
      float b = method(candidate,pev);
      if (b==b && fabs(b)<=std::numeric_limits<float>::infinity())
        ++j;
      //      std::cout << "fill" << std::endl;
      ntup->Fill(b,mass(candidate));//measuredMass());
      //      ntup->Fill((float)((*mybranch)[m]->momentum().P()));
    }
  }
  std::cout << j << std::endl;
  //ntup->Draw(Form("%s:mass",methodname(chosenmethod).c_str()),"mass>0","COLZ");
  ntup->Draw(Form("%s",methodname(chosenmethod).c_str()));
  if (padbuffer) padbuffer->cd();
  //gStyle=stylebuffer;
}

void ZooGui::LoadFile(const char* filename) {
  if (kette) delete kette;
  kette = NULL;
  mfilename = std::string(filename);
  if (file) {
    file->Close();
    delete file;
  }
  if (fMainFrame) fMainFrame->RemoveFrame(branchcombo);
  if (branchcombo) delete branchcombo;
  branchcombo = NULL;
  //  if (enterfilename) delete enterfilename;
  //  enterfilename = NULL;
  file = new TFile(filename);
  build();
  kette = new TChain("Forest");
  kette->Add(mfilename.c_str());
  maxentries->SetLimitValues(1,kette->GetEntries());
  maxentries->SetIntNumber(kette->GetEntries());
}

std::vector<std::string> ZooGui::GetBranchNames() {
  std::vector<std::string> zweignamen;
  zweignamen.clear();
  if (!file) return zweignamen;
  TTree* tree = NULL;
  file->GetObject("Forest",tree);
  if (!tree) return zweignamen;
  TObjArray* zweige = tree->GetListOfBranches();
  for ( int ind = 0 ; ind < zweige->GetEntries() ; ++ind) {
    // that is ugly. But still it works. I hope your branch never starts with 'Zoo'.
    if ((zweige->At(ind)->GetName())[0]=='Z'&&(zweige->At(ind)->GetName())[1]=='o'&&(zweige->At(ind)->GetName())[2]=='o')
      continue;
    if (!(strcmp(zweige->At(ind)->GetName(),"Event"))) continue;
    zweignamen.push_back(std::string(zweige->At(ind)->GetName()));
  }
  return zweignamen;
}

void ZooGui::buildCombo() {
  printf("buildCombo() called\n");
  if (branchcombo) delete branchcombo;
  branchcombo = new TGComboBox(fMainFrame);
  std::vector<std::string> zweignamen = GetBranchNames();
  for (unsigned int ind = 0 ; ind < zweignamen.size() ; ++ind) {
    branchcombo->AddEntry(zweignamen[ind].c_str(),ind);
  }
  branchcombo->Resize(102,22);
  branchcombo->Select(-1);
  fMainFrame->AddFrame(branchcombo, new TGLayoutHints());//kLHintsLeft | kLHintsTop,2,2,2,2));
  branchcombo->MoveResize(112,40,152,22);
  branchcombo->Connect(branchcombo,"Selected(const char*)","ZooGui",this,"setbranch(const char*)");

  methodcombo = new TGComboBox(fMainFrame);
  for (int i = 0 ; i < max_methods() ; ++i) {
    methodcombo->AddEntry(methodname(i).c_str(),i);
  }
  methodcombo->Resize(102,22);
  methodcombo->Select(-1);
  fMainFrame->AddFrame(methodcombo, new TGLayoutHints());
  methodcombo->MoveResize(112,62,152,22);
  methodcombo->Connect(methodcombo,"Selected(int)","ZooGui",this,"choose(int)");
  printf("buildCombo() processed\n");
}

void ZooGui::choose(int c) {chosenmethod=c;}

/* //did not work
   void ZooGui::SetMaxEntries(int themax) {
   maxnumber = themax;
   std::cout << "max changed" << std::endl;
   }*/

void ZooGui::build() {
  std::cout << "called build()"<< std::endl;
  if (maxentries) fMainFrame->RemoveFrame(maxentries);
  if (maxentries) delete maxentries;
  //if (maxentries) maxentries->Disconnect(maxentries,"ValueSet(Long_t)",this,"SetMaxEntries(int)");
  //if (maxentries) maxentries->Disconnect(maxentries,"ValueChanged(Long_t)",this,"SetMaxEntries(int)");
  maxentries = NULL;
  if (doit && fMainFrame) fMainFrame->RemoveFrame(doit);
  if (doit) doit->Disconnect(doit,"Clicked()",this,"plot()");
  if (doit) delete doit;
  doit = NULL;
  if (persistify && fMainFrame) fMainFrame->RemoveFrame(persistify);
  if (persistify) persistify->Disconnect(persistify,"Clicked()",this,"hardcopy()");
  if (persistify) delete persistify;
  persistify = NULL;
  if (methodcombo && fMainFrame) fMainFrame->RemoveFrame(methodcombo);
  if (methodcombo) methodcombo->Disconnect(methodcombo,"Selected(int)",this,"choose(int)");
  if (methodcombo) delete methodcombo;
  methodcombo = NULL;
  if (enterfilename) enterfilename->Disconnect(enterfilename,"ReturnPressed()",this,"LoadFile()");
  if (enterfilename && fMainFrame) fMainFrame->RemoveFrame(enterfilename);
  if (enterfilename) {delete enterfilename; enterfilename = NULL;}
  if (branchcombo) {delete branchcombo; branchcombo = NULL;}
  if (fMainFrame) delete fMainFrame;
  std::cout << "deleted main frame" << std::endl;
  fMainFrame = NULL;
  fMainFrame = new TGMainFrame();
  fMainFrame->SetLayoutBroken(kTRUE);
  fMainFrame->SetWindowName("ZooGUI. Powered by Physikalisches Institut at Heidelberg University.");
  if (!enterfilename) enterfilename = new TGTextEntry(fMainFrame);
  enterfilename->SetMaxLength(4096);
  enterfilename->SetAlignment(kTextLeft);
  if (mfilename.length()>2)
    enterfilename->SetText(mfilename.c_str());
  else
    enterfilename->SetText("enter filename");
  enterfilename->Resize(93,enterfilename->GetDefaultHeight());
  fMainFrame->AddFrame(enterfilename, new TGLayoutHints());
  enterfilename->MoveResize(10,10,93,22);
  //  enterfilename->Connect(enterfilename,"TextChanged(const char*)","ZooGui",this,"SetFilename(const char *)");
  enterfilename->Connect(enterfilename,"ReturnPressed()","ZooGui",this,"LoadFile()");
  std::cout << "text field exists, now fill comboboxes" << std::endl;
  buildCombo();
  std::cout << "okay, turn to the button" << std::endl;
  doit = new TGTextButton(fMainFrame,"DRAW");
  doit->SetTextJustify(36);
  doit->SetMargins(0,0,0,0);
  doit->SetWrapLength(-1);
  doit->Resize(92,24);
  fMainFrame->AddFrame(doit, new TGLayoutHints());
  doit->MoveResize(8,96,92,24);
  doit->Connect(doit,"Clicked()","ZooGui",this,"plot()");

  persistify = new TGTextButton(fMainFrame,"hardcopy");
  persistify->SetTextJustify(36);
  persistify->SetMargins(0,0,0,0);
  persistify->SetWrapLength(-1);
  persistify->Resize(92,24);
  fMainFrame->AddFrame(persistify, new TGLayoutHints());
  persistify->MoveResize(8,120,92,24);
  persistify->Connect(persistify,"Clicked()","ZooGui",this,"hardcopy()");


  progress = new TGHProgressBar(fMainFrame,TGProgressBar::kFancy,300);
  progress->TGHProgressBar::ShowPosition();
  fMainFrame->AddFrame(progress, new TGLayoutHints());
  progress->MoveResize(105,96,150,24);

  maxentries = new TGNumberEntry(fMainFrame,0,5,-1,TGNumberEntry::kNESInteger,TGNumberEntry::kNEANonNegative,TGNumberEntry::kNELLimitMinMax,0,1);
  //maxentries->Connect(maxentries,"ValueSet(Long_t)","ZooGui",this,"SetMaxEntries(int)");
  //maxentries->Connect(maxentries,"ValueChanged(Long_t)","ZooGui",this,"SetMaxEntries(int)");
  fMainFrame->AddFrame(maxentries, new TGLayoutHints());
  maxentries->MoveResize(260,96,90,24);

  fMainFrame->SetMWMHints(1,1,0);//kMWMDecorAll,  kMWMFuncAll, kMWMInputModeless);
  fMainFrame->MapSubwindows();

  fMainFrame->Resize(fMainFrame->GetDefaultSize());
  fMainFrame->MapWindow();
  fMainFrame->Resize(526,156);
  printf("end of build()\n");
}

int main(int argc, char** argv)
{ 
  std::string fname(argv[1]);
  // define if we want to run interactively
  TApplication* theApp = new TApplication("App", &argc, argv); 

  // Krempel
  ZooGui gui;
  gui.LoadFile(fname.c_str());

  theApp->Run();
}

ClassImp(ZooGui);
