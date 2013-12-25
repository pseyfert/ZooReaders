#ifndef FUNCTIONS_H
#define FUNCTIONS_H 1
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
/** @file functions.h
 *  various small functions which are used in ZooThings and possibly by more than one executable
 *
 *  @author Paul Seyfert
 *  @date   2011-11-15
 */

#include <iostream>
#include <fstream>
//#include <boost/shared_ptr.hpp>

#include <TFile.h>
#include <TRandom.h>
#include <TTree.h>
#include <TClassTable.h>
#include <TH1F.h>
#include <TH2.h>
#include <TSystem.h>
#include <TPaletteAxis.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TNtuple.h>
#include <TMath.h>
#include <string>
#include <vector>
//#include <boost/foreach.hpp>
#include <Math/GenVector/Plane3D.h>

#include "../Zoo/Zoo.h"


typedef ROOT::Math::SVector<double, 7> Vec7;
typedef ROOT::Math::SVector<double, 3> Vec3;


typedef
ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<Double32_t>,ROOT::Math::DefaultCoordinateSystemTag>
dispvec;                                                                                                                                                                                                       
typedef                                                                                                                                                                                                        
ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<Double32_t>,ROOT::Math::DefaultCoordinateSystemTag>
posvec;

typedef
ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<Double32_t>,ROOT::Math::DefaultCoordinateSystemTag>
dispvec;                                                                                                                   
typedef                                                                                                         
ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<Double32_t>,ROOT::Math::DefaultCoordinateSystemTag>
posvec;


bool passesGeoRefString(const ZooP*, const ZooEv*);
float PIDMVA(const ZooP* p) ; 
float noptPIDMVA(const ZooP* p) ;
float GeoMVA(const ZooP* p) ;
class ZooFunctor;
float singlePID_p0(const ZooP* p) ;
float singlePID_p1(const ZooP* p) ;
float singlePID_p2(const ZooP* p) ;
bool usethis(const ZooP*, const ZooEv*);

int L0TIS(const ZooP* p);
int L0TOS(const ZooP* p);
int Hlt1TIS(const ZooP* p);
int Hlt1TOS(const ZooP* p);
int Hlt2TIS(const ZooP* p);
int Hlt2TOS(const ZooP* p);


float  richK(const ZooP* p) ;
float richMU(const ZooP* p) ;

float richPI(const ZooP* p) ;
float ecalMU(const ZooP* p) ;
float hcalMU(const ZooP* p) ;
int BCK(const ZooP* p) ;
int eventNR(const ZooP* part, const ZooEv* pev) ;
float dira(const ZooP* p) ;
int recpid(const ZooP* p) ;
std::vector<std::string> GetBranchNames(TTree* tree) ;
float combine(const ZooP* p, const ZooP* p2) ;
float mumumass1(const ZooP* p) ;
float mumumass2(const ZooP* p) ;
float mumumass3(const ZooP* p) ;
float m12(const ZooP*) ;
float smm12(const ZooP*,const ZooEv*) ;
float m23(const ZooP*) ;
float smm23(const ZooP*,const ZooEv*) ;
float phiip(const ZooP* p, zooLorentzVector momentum2) ;
float phiMCmass(const ZooP* p);
float phiMCdaughtersmass(const ZooP* p);
float phiip_p0p2(const ZooP* p) ;
float phiip_p1p2(const ZooP* p) ;
float mass_phihypo(const ZooP* p);
float mass_Kpipi(const ZooP* p) ;
float mass_KKpi1(const ZooP* p) ;
float mass_KKpi2(const ZooP* p) ;
float mass_3pi(const ZooP* p) ;
float mass_k0k2(const ZooP* p) ;
float mass_k1k2(const ZooP* p) ;
float minpt(const ZooP* p) ;
bool ghostless(const ZooP* p) ;
typedef ROOT::Math::PxPyPzEVector     zooLorentzVector;
float doca(const ZooP* p, unsigned int one, unsigned int two) ;
float docaone(const ZooP* p) ;
float docatwo(const ZooP* p) ;
float docathree(const ZooP* p) ;
float chi1(const ZooP* p) ;
float chi2(const ZooP* p) ;
float chi3(const ZooP* p) ;
float matchchi(const ZooP* p) ;
int clean(const ZooP* p) ;
int jongeo(const ZooP* p) ;
int jonpid(const ZooP* p) ;
int common(const ZooP* p) ;
const ZooMCP* commonlow(const ZooP* p) ;
int commprong(const ZooP* p,int& code) ;
float trutau(const ZooP* p) ;
float peaking(const ZooP* p) ;
float trudsetamunu(const ZooP* p) ;
float trudsphipi(const ZooP* p) ;
int printtree(const ZooMCP* p,const ZooP* reco) ;
int commontwo(const ZooP* p) ;
int printtree(const ZooP* p) ;
int printroot2(const ZooMCP* p) ;
int printroot2(const ZooP* p) ;
int printroot(const ZooMCP* p) ;
int printroot(const ZooP* p) ;
int themother(const ZooP* p) ;
int missid(const ZooP* p) ;
float fisolationa(const ZooP* part) ;
float fisolationb(const ZooP* part) ;
float fisolationc(const ZooP* part) ;
int isolationa(const ZooP* part) ;
int isolationb(const ZooP* part) ;
int isolationc(const ZooP* part) ;
float isolationd(const ZooP* part) ;
float isolatione(const ZooP* part) ;
float isolationf(const ZooP* part) ;
float isosumm(const ZooP* part) ;
float CDF1(const ZooP* part) ;
float CDF2(const ZooP* part) ;
float CDF3(const ZooP* part) ;
float pt(const ZooP* p) ;
float pt(const ZooMCP* p) ;
float momentum(const ZooP* p) ;
float eta(const ZooP* p) ;
float phi(const ZooP* p) ;
float mupi(const ZooP* p) ;
float minmupi(const ZooP* p) ;
float pip(const ZooP* p) ;
float ppi(const ZooP* p);
float pk(const ZooP* p);
float mupiby15(const ZooP* p) ;
float muk(const ZooP* p) ;
float minmuk(const ZooP* p) ;
float kpi(const ZooP* p) ;
bool S15(const ZooP* p) ;
bool S17(const ZooP* p) ;
TString bckconvert(int i) ;
float diratrafo(const ZooP* p) ;
float acosdira(const ZooP* p) ;
float diratrafo2(const ZooP* p) ;
float cttrafo(const ZooP* p) ;
bool maxS15S17_tau(const ZooP* p) ;
bool maxS15S17_Ds(const ZooP* p) ;
float s15s17(const ZooP* p) ;
bool selected(const ZooP* p) ;
float mommass(const ZooP* p) ;
float measmass(const ZooP* p) ;
float mcmass(const ZooP* p) ;
float dauipone(const ZooP* p) ;
float dauiptwo(const ZooP* p) ;
float dauipthree(const ZooP* p) ;
float mindauips(const ZooP* p) ;
float d1ips(const ZooP* p) ;
float d2ips(const ZooP* p) ;
float d3ips(const ZooP* p) ;
float trackchi(const ZooP* p) ;
int tauproduction(const ZooP* p) ;
int tauproduction(const ZooMCP* mcp) ;
float productionweight(const ZooP* p) ;
float productionweighterror(const ZooP* p) ;
float productionweightMC11(const ZooP* p) ;
float productionweighterrorMC11(const ZooP* p) ;
float productionweightMC11v3(const ZooP* p) ;
float productionweighterrorMC11v3(const ZooP* p) ;
float productionweightMC11v2(const ZooP* p) ;
float productionweighterrorMC11v2(const ZooP* p) ;
int combineproduction(const ZooP* p) ;
int combineproduction(const ZooMCP* mcp) ;
int combineproduction(int fromB, int fromDp, int fromDs, int baryonic);
int baryonic(const ZooMCP* mcp);
int fromB(const ZooP* p);
int fromB(const ZooMCP* mcp);
int fromDp(const ZooP* p);
int fromDp(const ZooMCP* mcp);
int fromD0(const ZooMCP* mcp);
int D0PID(const ZooMCP* mcp);
int fromDs(const ZooP* p);
int fromDs(const ZooMCP* mcp);
int tauorder(const ZooP* part) ;
int isolationg(const ZooP* part) ;
int isolationh(const ZooP* part) ;
int isolationi(const ZooP* part) ;
int isolationj(const ZooP* part) ;
int isolationk(const ZooP* part) ;
int isolationl(const ZooP* part) ;
int chargeorder(const ZooP* part) ;
float bpvipchi2(const ZooP* p) ;
float bpvltime225(const ZooP* p) ;
float bpvltime(const ZooP* p) ;
float cloned(const ZooP* pa) ;
float cloned1(const ZooP* pa) ;
float cloned2(const ZooP* pa) ;
float cloned3(const ZooP* pa) ;
float minbias(const ZooP* part, const ZooEv* pev) ;
float theL0Dec(const ZooP* part, const ZooEv* pev) ;
float Hlt1Dec(const ZooP* part, const ZooEv* pev) ;
float L0Hlt1DecMu(const ZooP* part, const ZooEv* pev) ;
float Hlt2DecSingleMu(const ZooP* part, const ZooEv* pev) ;
float Hlt2DecMu(const ZooP* part, const ZooEv* pev) ;
float Hlt2DecDhhh(const ZooP* part, const ZooEv* pev) ;
float Hlt2Dec(const ZooP* part, const ZooEv* pev) ;
float unscaledHlt2Dec(const ZooP* , const ZooEv* pev) ;
float Hlt2DecOld(const ZooP* part, const ZooEv* pev) ;
float ANNghost(const ZooP* part);
float TRACKghost(const ZooP* part);
float ANNpion(const ZooP* part);
float ANNmuon(const ZooP* part);
float ANNkaon(const ZooP* part);
float ANNproton(const ZooP* part);
float ANNelectron(const ZooP* part);
float Tchi(const ZooP* );
float Vchi(const ZooP* );
float Tdof(const ZooP* );
float Vdof(const ZooP* );
float ncandcommonfake(const ZooP* );

float ismuon(const ZooP* p) ;                 
int istruemuon(const ZooP* p) ;
int mcpid(const ZooP* p) ;
int mcpid(const ZooMCP* p) ;
float float_mcpid(const ZooP* p) ;
float float_mcpid(const ZooMCP* p) ;
float float_ndau(const ZooMCP* p) ;
//float float_nchargeddau(const ZooMCP* p) ;
float float_mother_mcpid(const ZooP* p) ;
float float_mother_ndau(const ZooP* p) ;
//float float_mother_nchargeddau(const ZooP* p) ;
float float_mother_mother_mcpid(const ZooP* p) ;
float float_mother_mother_mother_mcpid(const ZooP* p) ;
float float_mother_mother_mother_mother_mcpid(const ZooP* p) ;

float mass_pi0m1m2  (const ZooP* p) ;
float mass_m0pi1m2  (const ZooP* p) ;
float mass_m0m1pi2  (const ZooP* p) ;
float mass_pi0pi1m2 (const ZooP* p) ;
float mass_pi0m1pi2 (const ZooP* p) ;
float mass_m0pi1pi2 (const ZooP* p) ;
float mass_pi0pi1pi2(const ZooP* p) ;
float mass_k0m1m2   (const ZooP* p) ;
float mass_m0k1m2   (const ZooP* p) ;
float mass_m0m1k2   (const ZooP* p) ;
float mass_k0k1m2   (const ZooP* p) ;
float mass_k0m1k2   (const ZooP* p) ;
float mass_m0k1k2   (const ZooP* p) ;
float mass_k0k1k2   (const ZooP* p) ;
float mass_k0pi1pi2 (const ZooP* p) ;
float mass_pi0k1pi2 (const ZooP* p) ;
float mass_pi0pi1k2 (const ZooP* p) ;
float mass_pi0k1k2  (const ZooP* p) ;
float mass_k0pi1k2  (const ZooP* p) ;
float mass_k0k1pi2  (const ZooP* p) ;
float mass_m0pi1k2  (const ZooP* p) ;
float mass_m0k1pi2  (const ZooP* p) ;
float mass_pi0m1k2  (const ZooP* p) ;
float mass_pi0k1m2  (const ZooP* p) ;
float mass_k0pi1m2  (const ZooP* p) ;
float mass_k0m1pi2  (const ZooP* p) ;






int joncode(const ZooMCP* mcp) ;

float expectedTT(const ZooP* p) ;

float observedTT(const ZooP* p) ;

float expectedT(const ZooP* p) ;

float expectedV(const ZooP* p) ;

float observedT(const ZooP* p) ;

float observedIT(const ZooP* p) ;
float observedOT(const ZooP* p) ;

float expectedIT(const ZooP* p) ;
float expectedOT(const ZooP* p) ;


float observedITids(const ZooP* p) ;
float observedOTids(const ZooP* p) ;
float observedTTids(const ZooP* p) ;
float observedV(const ZooP* p) ;


float trchi(const ZooP* p) ;
float trdof(const ZooP* p) ;

float ghostcat(const ZooP* p);
float tracklikelihood(const ZooP* p);
float polarisationangle(const ZooP* p) ;

float OThits(const ZooP* , const ZooEv* ev);
float IThits(const ZooP* , const ZooEv* ev);
float TThits(const ZooP* , const ZooEv* ev);
float VELOhits(const ZooP* , const ZooEv* ev);

float trackhistory(const ZooP*);
float tracktype(const ZooP*);

float OTbaddrifttime(const ZooP*);
float OToutliers(const ZooP*);

float largestMClifetime(const ZooP*);
double MClifetime(const ZooMCP* mcp);
float Y_atT(const ZooP* p) ;
float X_atT(const ZooP* p) ;
float pmumu_is_OS(const ZooP* p);

float mumumass1_smeared(const ZooP* p , const ZooEv* ev) ;
float mumumass2_smeared(const ZooP* p , const ZooEv* ev) ;
float mumumass3_smeared(const ZooP* p , const ZooEv* ev) ;
float trimumass(const ZooP* , const ZooEv*);

float mumumass1_true(const ZooP* p , const ZooEv* ev) ;
float mumumass2_true(const ZooP* p , const ZooEv* ev) ;
float mumumass3_true(const ZooP* p , const ZooEv* ev) ;


float mumumass1_truer(const ZooP* p , const ZooEv* ev) ;
float mumumass2_truer(const ZooP* p , const ZooEv* ev) ;
float mumumass3_truer(const ZooP* p , const ZooEv* ev) ;

float muon_opening_angle(const ZooP*);
float fcharge(const ZooP*);

float IPz(const ZooP*);
zooLorentzVector truermomentum(const ZooP* p, const ZooEv* ev) ;
zooLorentzVector truemomentum(const ZooP* p, const ZooEv* ev) ;

std::vector<const ZooP*> stables(const ZooP*) ;

int n_stables(const ZooP*);

template <class kk>
float gammaVLLLL(const kk* p) ;
template <class kk>
float gammaVLLRR(const kk* p) ;
template <class kk>
float gammaRAD(const kk* p) ;
template <class kk>
float interferenceVLLLL_RAD(const kk* p) ;
template <class kk>
float interferenceVLLRR_RAD(const kk* p) ;

template <class kk>
float norm_gammaVLLLL(const kk* p) ;
template <class kk>
float norm_gammaVLLRR(const kk* p) ;
template <class kk>
float norm_gammaRAD(const kk* p) ;
template <class kk>
float norm_interferenceVLLLL_RAD(const kk* p) ;
template <class kk>
float norm_interferenceVLLRR_RAD(const kk* p) ;


float norm_mc_gammaVLLLL(const ZooP* p) ;
float norm_mc_gammaVLLRR(const ZooP* p) ;
float norm_mc_gammaRAD(const ZooP* p) ;
float norm_mc_interferenceVLLLL_RAD(const ZooP* p) ;
float norm_mc_interferenceVLLRR_RAD(const ZooP* p) ;


int key(const ZooMCP* mcp) ;

int p0_mother_key(const ZooP* p) ;
int p0_mother_mother_key(const ZooP* p) ;
int p0_mother_mother_mother_key(const ZooP* p) ;
int p0_mother_mother_mother_mother_key(const ZooP* p) ;

int p1_mother_key(const ZooP* p) ;
int p1_mother_mother_key(const ZooP* p) ;
int p1_mother_mother_mother_key(const ZooP* p) ;
int p1_mother_mother_mother_mother_key(const ZooP* p) ;

int p2_mother_key(const ZooP* p) ;
int p2_mother_mother_key(const ZooP* p) ;
int p2_mother_mother_mother_key(const ZooP* p) ;
int p2_mother_mother_mother_mother_key(const ZooP* p) ;

float mother_pt(const ZooP* p);



#endif
