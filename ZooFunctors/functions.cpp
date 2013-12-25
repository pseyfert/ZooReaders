/** @file functions.cpp
 *  various small functions which are used in ZooThings and possibly by more than one executable
 *
 *  @author Paul Seyfert
 *  @date   2011-11-15
 */

#include "functions.h"
#include "ZooFunctor.h"
#include "comb.h"
#include <TLorentzVector.h>
#include "Math/GenVector/PxPyPzM4D.h"
#include "Math/GenVector/LorentzVector.h"
#include "gsl/gsl_linalg.h"
#include "gsl/gsl_blas.h"
#include "TRandom3.h"
#include <stack>

float PIDMVA(const ZooP* p) {
  if (p->ParticleInfo()->exists(2)) return p->ParticleInfo()->value(2);
  return 0.;
}
float noptPIDMVA(const ZooP* p) {
  if (p->ParticleInfo()->exists(20)) return p->ParticleInfo()->value(20);
  return 0.;
}
float GeoMVA(const ZooP* p) {
  if (p->ParticleInfo()->exists(1)) return p->ParticleInfo()->value(1);
  return 0.;
}
float singlePID_p0(const ZooP* p) {
  if (p->ParticleInfo()->exists(3)) return p->ParticleInfo()->value(3);
  return 0.;
}
float singlePID_p1(const ZooP* p) {
  if (p->ParticleInfo()->exists(4)) return p->ParticleInfo()->value(4);
  return 0.;
}
float singlePID_p2(const ZooP* p) {
  if (p->ParticleInfo()->exists(5)) return p->ParticleInfo()->value(5);
  return 0.;
}

float ismuon(const ZooP* p) {
  if( p->DLL()->isMuon()) return 1.;
  return 0.;
}
bool passesGeoRefString(const ZooP* p, const ZooEv* ev) {
  return clean(p)&&Hlt1Dec(p,ev)&&Hlt2Dec(p,ev)&&theL0Dec(p,ev)&&(mumumass3(p)>250.)&&(fabs(mumumass1(p)-1020.4)>20.)&&(fabs(mumumass2(p)-1020.4)>20.);
}

bool usethis(const ZooP* p, const ZooEv*) {
//return true;
  if (abs(p->pid())==4211) return false;
  if (abs(p->pid())==4122) return false;
  if (abs(p->pid())!=15) return true;
  return true;
  if (abs(p->measuredMass() - 1777.5) < 30.) return false;
  //////////if (fabs(mumumass1(p)-1020)<20) return false;
  //////////if (fabs(mumumass2(p)-1020)<20) return false;
  //////////if (fabs(mumumass3(p))<250) return false;
  //////////if (Hlt1Dec(p,pev) && Hlt2Dec(p,pev) && theL0Dec(p,pev))
  //////////  return clean(p);
  //////////return false;
}

float  richK(const ZooP* p) {
  if (p->ParticleInfo()->exists(10203))
  return p->ParticleInfo()->value(10203);
  return 0;
}
float richMU(const ZooP* p) {
  if (p->ParticleInfo()->exists(10201))
  return p->ParticleInfo()->value(10201);
  return 0;
}

float richPI(const ZooP* p) {
  if (p->ParticleInfo()->exists(10202))
  return p->ParticleInfo()->value(10202);
  return 0;
}

float ecalMU(const ZooP* p) {
  if (p->ParticleInfo()->exists(10465))
  return p->ParticleInfo()->value(10465);
  return 0;
}

float hcalMU(const ZooP* p) {
  if (p->ParticleInfo()->exists(10464))
  return p->ParticleInfo()->value(10464);
  return 0;
}


int BCK(const ZooP* p) {
  int retval = 0;
  //  if (!ghostless(p)) return retval;
  const ZooMCP* mo1 = NULL;;
  const ZooP* buffer = (*p)[0];
  mo1 = buffer->assocMCP();
  const ZooMCP* mo2 = NULL;
  buffer = (*p)[1];
  mo2 = buffer->assocMCP();
  const ZooMCP* mo3 = NULL;
  buffer = (*p)[2];
  mo3 = buffer->assocMCP();

  if (mo1) {
    if (abs(mo1->pid()) == 13) retval += 0x1;
    else {
      if (abs(mo1->pid()) == 211) retval += 0x10;
      else {
        if (abs(mo1->pid()) == 321) retval += 0x100;
        else {
          if (abs(mo1->pid()) == 2212) retval += 0x100000;
          else retval += 0x10000;
        }
      }
    }
  } else retval+= 0x1000;

  if (mo2) {
    if (abs(mo2->pid()) == 13) retval += 0x1;
    else {
      if (abs(mo2->pid()) == 211) retval += 0x10;
      else {
        if (abs(mo2->pid()) == 321) retval += 0x100;
        else {
          if (abs(mo2->pid()) == 2212) retval += 0x100000;
          else retval += 0x10000;
        }
      }
    }
  } else retval+= 0x1000;


  if (mo3) {
    if (abs(mo3->pid()) == 13) retval += 0x1;
    else {
      if (abs(mo3->pid()) == 211) retval += 0x10;
      else {
        if (abs(mo3->pid()) == 321) retval += 0x100;
        else {
          if (abs(mo3->pid()) == 2212) retval += 0x100000;
          else retval += 0x10000;
        }
      }
    }
  } else retval+= 0x1000;


  return retval;

}

int eventNR(const ZooP*, const ZooEv* pev) {
  return pev->eventNumber();
}

float dira(const ZooP* p) {
  const zooLorentzVector pmo(p->momentum());
  ROOT::Math::Plane3D::Point pv = p->pv();
  return (p->particle().pos() - pv).Dot(pmo.Vect()) /
    (pmo.P() * (p->particle().pos() - pv).R());

}
int recpid(const ZooP* p) {
  return p->pid();
}

typedef
ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<Double32_t>,ROOT::Math::DefaultCoordinateSystemTag>
dispvec;                                                                                                                                                                                                       
typedef                                                                                                                                                                                                        
ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<Double32_t>,ROOT::Math::DefaultCoordinateSystemTag>
posvec;


std::vector<std::string> GetBranchNames(TTree* tree) {
  std::vector<std::string> zweignamen;
  zweignamen.clear();
  if (!tree) return zweignamen;
  TObjArray* zweige = tree->GetListOfBranches();
  std::cout << "REMEMBER: THIS AUTOMATIC BRANCHDETECTION FAILS IF YOUR DECAY IS CALLED \"Zoo...\"" << std::endl;
  for ( int ind = 0 ; ind < zweige->GetEntries() ; ++ind) {
    // that is ugly. But still it works. I hope your branch never starts with 'Zoo'.
    if ((zweige->At(ind)->GetName())[0]=='Z'&&(zweige->At(ind)->GetName())[1]=='o'&&(zweige->At(ind)->GetName())[2]=='o')
      continue;
    if (!(strcmp(zweige->At(ind)->GetName(),"Event"))) continue;
    zweignamen.push_back(std::string(zweige->At(ind)->GetName()));
  }
  return zweignamen;
}

float pmumu_is_OS(const ZooP* p) {
  int is_pmumu = 0;  /// status code: 0 = undefined, 2 = bad, 1 = okay
  unsigned mu1 = 99u;
  unsigned mu2 = 99u;
  for (unsigned d = 0 ; d < p->NCh() ; ++d) {
    if (13==abs(p->Ch(d)->pid())) {
      if (99u==mu1) {
        mu1=d;
      } else {
        if (99u==mu2) {
          mu2=d;
          is_pmumu = 1;
        } else {
          is_pmumu = 2;
        }
      }
    }
  }
  if (1 == is_pmumu) {
    return (float)(p->Ch(mu1)->charge() * p->Ch(mu2)->charge());
  }
  return 0.;
}

//float combine(const ZooP* p, const ZooP* p2) {
//  zooLorentzVector mom = p->momentum();
//  mom+=p2->momentum();
//  return mom.M();
//}

float smm12(const ZooP* p, const ZooEv* ev) {
  float m =  (mumumass3_smeared(p,ev));
  return m*m;
}
float smm23(const ZooP* p,const ZooEv* ev) {
  float m =  (mumumass2_smeared(p,ev));
  return m*m;
}
float m12(const ZooP* p) {
  float m =  (mumumass3(p));
  return m*m;
}
float m23(const ZooP* p) {
  float m =  (mumumass2(p));
  return m*m;
}
float mumumass1(const ZooP* p) {
  return combine(p->Ch(0),p->Ch(2));
}

float mumumass2(const ZooP* p) {
  return combine(p->Ch(1),p->Ch(2));
}
float mumumass3(const ZooP* p) {
  return combine(p->Ch(0),p->Ch(1));
}

float mass_k0m2(const ZooP* p) {

  unsigned int tmp_particle_0 = 0;
  //		float p0_mass = 105.66;
  //		float p0_mass = 139.57;
  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  float p1_mass = 105.66;
  //		float p1_mass = 139.57;
  //		float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_k1m2(const ZooP* p) {

  unsigned int tmp_particle_0 = 1;
  //		float p0_mass = 105.66;
  //		float p0_mass = 139.57;
  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  float p1_mass = 105.66;
  //		float p1_mass = 139.57;
  //		float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_m0k2(const ZooP* p) {

  unsigned int tmp_particle_0 = 0;
  float p0_mass = 105.66;
  //	float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  //	float p1_mass = 139.57;
  float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_m1k2(const ZooP* p) {

  unsigned int tmp_particle_0 = 1;
  float p0_mass = 105.66;
  //	float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  //	float p1_mass = 139.57;
  float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_k0k2(const ZooP* p) {

  unsigned int tmp_particle_0 = 0;
  //	float p0_mass = 105.66;
  //	float p0_mass = 139.57;
  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  //	float p1_mass = 139.57;
  float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_k1k2(const ZooP* p) {

  unsigned int tmp_particle_0 = 1;
  //	float p0_mass = 105.66;
  //	float p0_mass = 139.57;
  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  //	float p1_mass = 139.57;
  float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_Kpipi(const ZooP* p) {
  unsigned int tmp_particle_0 = 0;
  //		float p0_mass = 105.66;
  float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 1;
  //    float p1_mass = 105.66;
  float p1_mass = 139.57;
  //		float p1_mass = 493.68;

  unsigned int tmp_particle_2 = 2;
  //    float p2_mass = 105.66;
  //		float p2_mass = 139.57;
  float p2_mass = 493.68;


  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(tmp_particle_2)->momentum()).px(),
      (p->Ch(tmp_particle_2)->momentum()).py(),
      (p->Ch(tmp_particle_2)->momentum()).pz(),
      (p->Ch(tmp_particle_2)->momentum()).E()
      );


  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);
  tmp_p2.SetPtEtaPhiM(tmp_p2.Pt(),tmp_p2.Eta(),tmp_p2.Phi(),p2_mass);

  return (tmp_p0 + tmp_p1 + tmp_p2).M();

}
float mass_KKpi1(const ZooP* p) {
  unsigned int tmp_particle_0 = 0;
  //		float p0_mass = 105.66;
  //    float p0_mass = 139.57;
  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 1;
  //    float p1_mass = 105.66;
  float p1_mass = 139.57;
  //		float p1_mass = 493.68;

  unsigned int tmp_particle_2 = 2;
  //    float p2_mass = 105.66;
  //		float p2_mass = 139.57;
  float p2_mass = 493.68;


  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(tmp_particle_2)->momentum()).px(),
      (p->Ch(tmp_particle_2)->momentum()).py(),
      (p->Ch(tmp_particle_2)->momentum()).pz(),
      (p->Ch(tmp_particle_2)->momentum()).E()
      );


  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);
  tmp_p2.SetPtEtaPhiM(tmp_p2.Pt(),tmp_p2.Eta(),tmp_p2.Phi(),p2_mass);

  return (tmp_p0 + tmp_p1 + tmp_p2).M();

}
float mass_KKpi2(const ZooP* p) {
  unsigned int tmp_particle_0 = 0;
  //		float p0_mass = 105.66;
  float p0_mass = 139.57;
  //  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 1;
  //    float p1_mass = 105.66;
  //   float p1_mass = 139.57;
  float p1_mass = 493.68;

  unsigned int tmp_particle_2 = 2;
  //    float p2_mass = 105.66;
  //		float p2_mass = 139.57;
  float p2_mass = 493.68;


  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(tmp_particle_2)->momentum()).px(),
      (p->Ch(tmp_particle_2)->momentum()).py(),
      (p->Ch(tmp_particle_2)->momentum()).pz(),
      (p->Ch(tmp_particle_2)->momentum()).E()
      );


  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);
  tmp_p2.SetPtEtaPhiM(tmp_p2.Pt(),tmp_p2.Eta(),tmp_p2.Phi(),p2_mass);

  return (tmp_p0 + tmp_p1 + tmp_p2).M();

}
float mass_3pi(const ZooP* p) {
  unsigned int tmp_particle_0 = 0;
  //		float p0_mass = 105.66;
  float p0_mass = 139.57;
  //  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 1;
  //    float p1_mass = 105.66;
  float p1_mass = 139.57;
  ///float p1_mass = 493.68;

  unsigned int tmp_particle_2 = 2;
  //    float p2_mass = 105.66;
  float p2_mass = 139.57;
  //float p2_mass = 493.68;


  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(tmp_particle_2)->momentum()).px(),
      (p->Ch(tmp_particle_2)->momentum()).py(),
      (p->Ch(tmp_particle_2)->momentum()).pz(),
      (p->Ch(tmp_particle_2)->momentum()).E()
      );


  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);
  tmp_p2.SetPtEtaPhiM(tmp_p2.Pt(),tmp_p2.Eta(),tmp_p2.Phi(),p2_mass);

  return (tmp_p0 + tmp_p1 + tmp_p2).M();

}

float mass_pi0m2(const ZooP* p) {

  unsigned int tmp_particle_0 = 0;
  //		float p0_mass = 105.66;
  float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  float p1_mass = 105.66;
  //		float p1_mass = 139.57;
  //		float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_pi1m2(const ZooP* p) {

  unsigned int tmp_particle_0 = 1;
  //		float p0_mass = 105.66;
  float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  float p1_mass = 105.66;
  //		float p1_mass = 139.57;
  //		float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_m0pi2(const ZooP* p) {

  unsigned int tmp_particle_0 = 0;
  float p0_mass = 105.66;
  //	float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  float p1_mass = 139.57;
  //	float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_m1pi2(const ZooP* p) {

  unsigned int tmp_particle_0 = 1;
  float p0_mass = 105.66;
  //	float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  float p1_mass = 139.57;
  //	float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_pi0pi2(const ZooP* p) {

  unsigned int tmp_particle_0 = 0;
  //	float p0_mass = 105.66;
  float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  float p1_mass = 139.57;
  //	float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_pi1pi2(const ZooP* p) {

  unsigned int tmp_particle_0 = 1;
  //	float p0_mass = 105.66;
  float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  float p1_mass = 139.57;
  //	float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_pi0k2(const ZooP* p) {

  unsigned int tmp_particle_0 = 0;
  //		float p0_mass = 105.66;
  float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  //		float p1_mass = 139.57;
  float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_pi1k2(const ZooP* p) {

  unsigned int tmp_particle_0 = 1;
  //		float p0_mass = 105.66;
  float p0_mass = 139.57;
  //	float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  //		float p1_mass = 139.57;
  float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_k0pi2(const ZooP* p) {

  unsigned int tmp_particle_0 = 0;
  //	float p0_mass = 105.66;
  //	float p0_mass = 139.57;
  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  float p1_mass = 139.57;
  //	float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_k1pi2(const ZooP* p) {

  unsigned int tmp_particle_0 = 1;
  //	float p0_mass = 105.66;
  //	float p0_mass = 139.57;
  float p0_mass = 493.68;

  unsigned int tmp_particle_1 = 2;
  //	float p1_mass = 105.66;
  float p1_mass = 139.57;
  //	float p1_mass = 493.68;

  TLorentzVector tmp_p0;

  tmp_p0.SetPxPyPzE(
      (p->Ch(tmp_particle_0)->momentum()).px(),
      (p->Ch(tmp_particle_0)->momentum()).py(),
      (p->Ch(tmp_particle_0)->momentum()).pz(),
      (p->Ch(tmp_particle_0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(tmp_particle_1)->momentum()).px(),
      (p->Ch(tmp_particle_1)->momentum()).py(),
      (p->Ch(tmp_particle_1)->momentum()).pz(),
      (p->Ch(tmp_particle_1)->momentum()).E()
      );

  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  return (tmp_p0 + tmp_p1).M();
}

float mass_p0p1p2(const ZooP* p, TString p0_id, TString p1_id, TString p2_id ) {

  //set masses
  float p0_mass = -1 ;
  float p1_mass = -1 ;
  float p2_mass = -1 ;

  if (!strcmp(p0_id,"mu")){
    p0_mass =  105.66;
  }
  else if (!strcmp(p0_id,"pi") ){
    p0_mass = 139.57;
  }
  else if (!strcmp(p0_id,"k")){
    p0_mass = 493.68;
  }

  if (!strcmp(p1_id,"mu")){
    p1_mass =  105.66;
  }
  else if (!strcmp(p1_id,"pi") ){
    p1_mass = 139.57;
  }
  else if (!strcmp(p1_id,"k")){
    p1_mass = 493.68;
  }

  if (!strcmp(p2_id,"mu")){
    p2_mass =  105.66;
  }
  else if (!strcmp(p2_id,"pi") ){
    p2_mass = 139.57;
  }
  else if (!strcmp(p2_id,"k")){
    p2_mass = 493.68;
  }

  if(p0_mass == -1 || p1_mass == -1 || p2_mass == -1 ){
    return -1;
  }

  TLorentzVector tmp_p0;
  tmp_p0.SetPxPyPzE(
      (p->Ch(0)->momentum()).px(),
      (p->Ch(0)->momentum()).py(),
      (p->Ch(0)->momentum()).pz(),
      (p->Ch(0)->momentum()).E()
      );

  //set new mass
  tmp_p0.SetPtEtaPhiM(tmp_p0.Pt(),tmp_p0.Eta(),tmp_p0.Phi(),p0_mass);

  TLorentzVector tmp_p1;
  tmp_p1.SetPxPyPzE(
      (p->Ch(1)->momentum()).px(),
      (p->Ch(1)->momentum()).py(),
      (p->Ch(1)->momentum()).pz(),
      (p->Ch(1)->momentum()).E()
      );
  //set new mass
  tmp_p1.SetPtEtaPhiM(tmp_p1.Pt(),tmp_p1.Eta(),tmp_p1.Phi(),p1_mass);

  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(2)->momentum()).px(),
      (p->Ch(2)->momentum()).py(),
      (p->Ch(2)->momentum()).pz(),
      (p->Ch(2)->momentum()).E()
      );

  //set new mass
  tmp_p2.SetPtEtaPhiM(tmp_p2.Pt(),tmp_p2.Eta(),tmp_p2.Phi(),p2_mass);

  return (tmp_p0 + tmp_p1 + tmp_p2).M();
}

float mass_pi0m1m2(const ZooP* p){
  return mass_p0p1p2(p,"pi","mu","mu");
}
float mass_m0pi1m2(const ZooP* p){
  return mass_p0p1p2(p,"mu","pi","mu");
}
float mass_m0m1pi2(const ZooP* p){
  return mass_p0p1p2(p,"mu","mu","pi");
}
float mass_pi0pi1m2(const ZooP* p){
  return mass_p0p1p2(p,"pi","pi","mu");
}
float mass_pi0m1pi2(const ZooP* p){
  return mass_p0p1p2(p,"pi","mu","pi");
}
float mass_m0pi1pi2(const ZooP* p){
  return mass_p0p1p2(p,"mu","pi","pi");
}
float mass_pi0pi1pi2(const ZooP* p){
  return mass_p0p1p2(p,"pi","pi","pi");
}
float mass_k0m1m2(const ZooP* p){
  return mass_p0p1p2(p,"k","mu","mu");
}
float mass_m0k1m2(const ZooP* p){
  return mass_p0p1p2(p,"mu","k","mu");
}
float mass_m0m1k2(const ZooP* p){
  return mass_p0p1p2(p,"mu","mu","k");
}
float mass_k0k1m2(const ZooP* p){
  return mass_p0p1p2(p,"k","k","mu");
}
float mass_k0m1k2(const ZooP* p){
  return mass_p0p1p2(p,"k","mu","k");
}
float mass_m0k1k2(const ZooP* p){
  return mass_p0p1p2(p,"mu","k","k");
}
float mass_k0k1k2(const ZooP* p){
  return mass_p0p1p2(p,"k","k","k");
}
float mass_k0pi1pi2(const ZooP* p){
  return mass_p0p1p2(p,"k","pi","pi");
}
float mass_pi0k1pi2(const ZooP* p){
  return mass_p0p1p2(p,"pi","k","pi");
}
float mass_pi0pi1k2(const ZooP* p){
  return mass_p0p1p2(p,"pi","pi","k");
}
float mass_pi0k1k2(const ZooP* p){
  return mass_p0p1p2(p,"pi","k","k");
}
float mass_k0pi1k2(const ZooP* p){
  return mass_p0p1p2(p,"k","pi","k");
}
float mass_k0k1pi2(const ZooP* p){
  return mass_p0p1p2(p,"k","k","pi");
}
float mass_m0pi1k2(const ZooP* p){
  return mass_p0p1p2(p,"mu","pi","k");
}
float mass_m0k1pi2(const ZooP* p){
  return mass_p0p1p2(p,"mu","k","pi");
}
float mass_pi0m1k2(const ZooP* p){
  return mass_p0p1p2(p,"pi","mu","k");
}
float mass_pi0k1m2(const ZooP* p){
  return mass_p0p1p2(p,"pi","k","mu");
}
float mass_k0pi1m2(const ZooP* p){
  return mass_p0p1p2(p,"k","pi","mu");
}
float mass_k0m1pi2(const ZooP* p){
  return mass_p0p1p2(p,"k","mu","pi");
}
float dphi_max(const ZooP* p){
  TLorentzVector tmp_tlv[3];
  for (uint k=0; k<3; k++){
    tmp_tlv[k].SetPxPyPzE(
        (p->Ch(k)->momentum()).px(),
        (p->Ch(k)->momentum()).py(),
        (p->Ch(k)->momentum()).pz(),
        (p->Ch(k)->momentum()).E()
        );
  }

  //find largest dphi:

  double tmp_dphi_max = tmp_tlv[0].DeltaPhi(tmp_tlv[1]);
  for (uint k=0; k<3; k++){
    for (uint kk=k+1; kk<3; kk++){
      if(fabs(tmp_tlv[k].DeltaPhi(tmp_tlv[kk])) > fabs(tmp_dphi_max)){
        tmp_dphi_max = tmp_tlv[k].DeltaPhi(tmp_tlv[kk]);
      }
    }
  }

  return tmp_dphi_max;
}
float dphi_min(const ZooP* p){
  TLorentzVector tmp_tlv[3];
  for (uint k=0; k<3; k++){
    tmp_tlv[k].SetPxPyPzE(
        (p->Ch(k)->momentum()).px(),
        (p->Ch(k)->momentum()).py(),
        (p->Ch(k)->momentum()).pz(),
        (p->Ch(k)->momentum()).E()
        );
  }

  //find smallest dphi:

  double tmp_dphi_min = tmp_tlv[0].DeltaPhi(tmp_tlv[1]);
  for (uint k=0; k<3; k++){
    for (uint kk=k+1; kk<3; kk++){
      if(fabs(tmp_tlv[k].DeltaPhi(tmp_tlv[kk])) < fabs(tmp_dphi_min)){
        tmp_dphi_min = tmp_tlv[k].DeltaPhi(tmp_tlv[kk]);
      }
    }
  }

  return tmp_dphi_min;
}

float dphi_p0p2(const ZooP* p){
  TLorentzVector tmp_p0;
  tmp_p0.SetPxPyPzE(
      (p->Ch(0)->momentum()).px(),
      (p->Ch(0)->momentum()).py(),
      (p->Ch(0)->momentum()).pz(),
      (p->Ch(0)->momentum()).E()
      );
  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(2)->momentum()).px(),
      (p->Ch(2)->momentum()).py(),
      (p->Ch(2)->momentum()).pz(),
      (p->Ch(2)->momentum()).E()
      );
  return tmp_p0.DeltaPhi(tmp_p2);
}
float dphi_p1p2(const ZooP* p){
  TLorentzVector tmp_p0;
  tmp_p0.SetPxPyPzE(
      (p->Ch(1)->momentum()).px(),
      (p->Ch(1)->momentum()).py(),
      (p->Ch(1)->momentum()).pz(),
      (p->Ch(1)->momentum()).E()
      );
  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(2)->momentum()).px(),
      (p->Ch(2)->momentum()).py(),
      (p->Ch(2)->momentum()).pz(),
      (p->Ch(2)->momentum()).E()
      );
  return tmp_p0.DeltaPhi(tmp_p2);
}
float dphi_p0p1(const ZooP* p){
  TLorentzVector tmp_p0;
  tmp_p0.SetPxPyPzE(
      (p->Ch(0)->momentum()).px(),
      (p->Ch(0)->momentum()).py(),
      (p->Ch(0)->momentum()).pz(),
      (p->Ch(0)->momentum()).E()
      );
  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(1)->momentum()).px(),
      (p->Ch(1)->momentum()).py(),
      (p->Ch(1)->momentum()).pz(),
      (p->Ch(1)->momentum()).E()
      );
  return tmp_p0.DeltaPhi(tmp_p2);
}
float dr_p0p1(const ZooP* p){
  TLorentzVector tmp_p0;
  tmp_p0.SetPxPyPzE(
      (p->Ch(0)->momentum()).px(),
      (p->Ch(0)->momentum()).py(),
      (p->Ch(0)->momentum()).pz(),
      (p->Ch(0)->momentum()).E()
      );
  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(1)->momentum()).px(),
      (p->Ch(1)->momentum()).py(),
      (p->Ch(1)->momentum()).pz(),
      (p->Ch(1)->momentum()).E()
      );
  return tmp_p0.DeltaR(tmp_p2);
}
float dr_p0p2(const ZooP* p){
  TLorentzVector tmp_p0;
  tmp_p0.SetPxPyPzE(
      (p->Ch(0)->momentum()).px(),
      (p->Ch(0)->momentum()).py(),
      (p->Ch(0)->momentum()).pz(),
      (p->Ch(0)->momentum()).E()
      );
  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(2)->momentum()).px(),
      (p->Ch(2)->momentum()).py(),
      (p->Ch(2)->momentum()).pz(),
      (p->Ch(2)->momentum()).E()
      );
  return tmp_p0.DeltaR(tmp_p2);
}
float dr_p1p2(const ZooP* p){
  TLorentzVector tmp_p0;
  tmp_p0.SetPxPyPzE(
      (p->Ch(1)->momentum()).px(),
      (p->Ch(1)->momentum()).py(),
      (p->Ch(1)->momentum()).pz(),
      (p->Ch(1)->momentum()).E()
      );
  TLorentzVector tmp_p2;
  tmp_p2.SetPxPyPzE(
      (p->Ch(2)->momentum()).px(),
      (p->Ch(2)->momentum()).py(),
      (p->Ch(2)->momentum()).pz(),
      (p->Ch(2)->momentum()).E()
      );
  return tmp_p0.DeltaR(tmp_p2);
}
float phiip(const ZooP* p, zooLorentzVector momentum2) {
  ROOT::Math::XYZPoint dv = p->decayVertex();
  ROOT::Math::XYZPoint pv = ROOT::Math::XYZPoint(p->pv().pos()[0],p->pv().pos()[1],p->pv().pos()[2]);
  dispvec fl = dv-pv;
  ROOT::Math::XYZPoint flight = ROOT::Math::XYZPoint(fl.x(),fl.y(),fl.z());
  ROOT::Math::XYZPoint norm = (flight).Cross(momentum2.Vect());
  float dist = norm.R() / momentum2.Vect().R();
  return dist;
}
float mass_phihypo(const ZooP* p){
  zooLorentzVector mom;
  if (abs(p->Ch(0)->pid())==13)
    mom+=p->Ch(0)->momentum();
  if (abs(p->Ch(1)->pid())==13)
    mom+=p->Ch(1)->momentum();
  if (abs(p->Ch(2)->pid())==13)
    mom+=p->Ch(2)->momentum();
  TLorentzVector mom_copy;
  mom_copy.SetPtEtaPhiM(mom.Pt(),mom.Eta(),mom.Phi(),1020.);
  zooLorentzVector mom2(mom_copy.X(),mom_copy.Y(),mom_copy.Z(),mom_copy.E());
  if (abs(p->Ch(0)->pid())!=13)
    mom2+=p->Ch(0)->momentum();
  if (abs(p->Ch(1)->pid())!=13)
    mom2+=p->Ch(1)->momentum();
  if (abs(p->Ch(2)->pid())!=13)
    mom2+=p->Ch(2)->momentum();
  return mom2.M();
}
  

float phiip_p0p2(const ZooP* p) {
  zooLorentzVector mom = p->Ch(0)->momentum();
  mom += p->Ch(2)->momentum();
  return phiip(p,mom);
}

float phiip_p1p2(const ZooP* p) {
  zooLorentzVector mom = p->Ch(1)->momentum();
  mom += p->Ch(2)->momentum();
  return phiip(p,mom);
}



float minpt(const ZooP* p) {
  float ret = 999999999;
  for (unsigned int i = 0 ; i < p->NCh() ; ++i)
    ret = std::min(ret,(float)p->Ch(i)->momentum().Rho());
  return ret;
}
bool ghostless(const ZooP* p) {
  if (p->Ch(0)->assocMCP())
    if (p->Ch(1)->assocMCP())
      if (p->Ch(2)->assocMCP())
        return true;
  return false;
}
typedef
ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<Double32_t>,ROOT::Math::DefaultCoordinateSystemTag>
dispvec;                                                                                                                   
typedef                                                                                                         
ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<Double32_t>,ROOT::Math::DefaultCoordinateSystemTag>
posvec;

typedef ROOT::Math::PxPyPzEVector     zooLorentzVector;
float doca(const ZooP* p, unsigned int one, unsigned int two) {
  if(p->NCh()<std::max(one,two)) return -1;
  const zooLorentzVector pmo_one((*p)[one]->momentum());
  const zooLorentzVector pmo_two((*p)[two]->momentum());
  dispvec dir_one,dir_two;
  dir_one.SetXYZ(pmo_one.x(),pmo_one.y(),pmo_one.z());
  dir_two.SetXYZ(pmo_two.x(),pmo_two.y(),pmo_two.z());

  dispvec cross = dir_one.Cross(dir_two);
  float norm = sqrt(cross.Mag2());
  cross/=norm;
  posvec tmppoint1 = (*p)[one]->particle().pos();
  posvec tmppoint2 = (*p)[two]->particle().pos();
  dispvec tmpvec = tmppoint1-tmppoint2;
  float signeddoca = tmpvec.Dot(cross);
  return fabs(signeddoca);


  return -1;
}



float docaone(const ZooP* p) {
  return doca(p,0,1);
}
float docatwo(const ZooP* p) {
  return doca(p,1,2);
}
float docathree(const ZooP* p) {
  return doca(p,0,2);
}


float chi1(const ZooP* p) {
  bool debug = false;
  if (debug) {
    std::cout << p->NCh() << " children. Hope this is okay." << std::endl;
    if (p->NCh() < 1) return std::numeric_limits<float>::signaling_NaN();
    if (p->Ch(0)->TrInfo() == NULL) {
      std::cout << " No TRACK info " << std::endl;
      return std::numeric_limits<float>::signaling_NaN();
    }
  }
  return p->Ch(0)->TrInfo()->chi2()/p->Ch(0)->TrInfo()->ndf();
}
float chi2(const ZooP* p) {
  return p->Ch(1)->TrInfo()->chi2()/p->Ch(1)->TrInfo()->ndf();
}
float chi3(const ZooP* p) {
  return p->Ch(2)->TrInfo()->chi2()/p->Ch(2)->TrInfo()->ndf();
}

float matchchi(const ZooP* p) {
  if (p->TrExtraInfo() && p->TrExtraInfo()->exists(21)) return p->TrExtraInfo()->value(21);
  return -99.;
}

int clean(const ZooP* p) {
  if (NULL==p) {std::cout << "YOU FOOL" << std::endl;}
  bool one   = (p->ct()<0.025);
  bool two   = (p->ct()>-0.01);
  bool three = (dira(p)>0.99);
  return one && two && three;
  return (p->ct()<0.025)&&(p->ct()>-0.01)&&(dira(p)>0.99);
}

int jongeo(const ZooP* p) {
  return (p->chi2()<12)&&(docaone(p)<0.15)&&(docatwo(p)<0.15)&&(docathree(p)<0.15)&&(dira(p)>0.9999);
}

int jonpid(const ZooP* p) {
  return (p->Ch(0)->DLL()->DLLmu()>1.5)&&(p->Ch(1)->DLL()->DLLmu()>1.5)&&(p->Ch(2)->DLL()->DLLmu()>1.5)
    &&(p->Ch(0)->DLL()->DLLmu() - p->Ch(0)->DLL()->DLLk()>20)&&(p->Ch(1)->DLL()->DLLmu() - p->Ch(1)->DLL()->DLLk()>20)&&(p->Ch(2)->DLL()->DLLmu() - p->Ch(2)->DLL()->DLLk()>20);
}

int numberofghosts(const ZooP* p) {

  int nghosts=0;

  for (uint k=0; k<p->NCh(); k++){
    if ( !(*p)[k]->assocMCP() ){
      nghosts++;
    }
  }

  return nghosts;
}

int commontwo(const ZooP* p) {

  if ( numberofghosts(p) == 3 ) return 0;

  if ( numberofghosts(p) == 2 ) {
    for (uint k=0; k<p->NCh(); k++){
      if ( !(*p)[k]->assocMCP() ) return k;
      return k;
    }
  }

  //if there is only one ghost return the two others
  const ZooMCP* mo1;
  const ZooP* buffer = (*p)[0];
  if ( buffer->assocMCP() ){
    mo1 = buffer->assocMCP();
  }
  else {
    mo1 = NULL;
  }

  const ZooMCP* mo2;
  buffer = (*p)[1];
  if ( buffer->assocMCP() ){
    mo2 = buffer->assocMCP();
  }
  else {
    mo2 = NULL;
  }

  const ZooMCP* mo3;
  buffer = (*p)[2];
  if ( buffer->assocMCP() ){
    mo3 = buffer->assocMCP();
  }
  else {
    mo3 = NULL;
  }


  for (int i = 0 ; i < 100 ; ++i) {
    if (mo1 == NULL) continue;
    if (mo1->NMo() > 0) {mo1 = mo1->Mo(0); --i;}
    else i=1000;
  }

  for (int i = 0 ; i < 100 ; ++i) {
    if (mo2 == NULL) continue;
    if (mo2->NMo() > 0) {mo2 = mo2->Mo(0); --i;}
    else i=1000;
  }
  for (int i = 0 ; i < 100 ; ++i) {
    if (mo3 == NULL) continue;
    if (mo3->NMo() > 0) {mo3 = mo3->Mo(0); --i;}
    else i=1000;
  }


  if (mo1 == mo2) return 12;
  else if (mo2 == mo3) return 23;
  else if (mo1 == mo3) return 13;
  else {
    if (mo1 == NULL) return 123;
    if (mo2 == NULL) return 213;
    if (mo3 == NULL) return 312;
  }

  return -1;
}
int common(const ZooP* p) {
  if (!ghostless(p)) return 0;
  const ZooMCP* mo1;
  const ZooP* buffer = (*p)[0];
  mo1 = buffer->assocMCP();
  const ZooMCP* mo2;
  buffer = (*p)[1];
  mo2 = buffer->assocMCP();
  const ZooMCP* mo3;
  buffer = (*p)[2];
  mo3 = buffer->assocMCP();
  for (int i = 0 ; i < 100 ; ++i) {
    if (mo1->NMo() > 0) {mo1 = mo1->Mo(0); --i;}
    else i=1000;
  }
  for (int i = 0 ; i < 100 ; ++i) {
    if (mo2->NMo() > 0) {mo2 = mo2->Mo(0); --i;}
    else i=1000;
  } 
  for (int i = 0 ; i < 100 ; ++i) {
    if (mo3->NMo() > 0) {mo3 = mo3->Mo(0); --i;}
    else i=1000;
  }
  if (mo1 == mo2) {
    if (mo2 == mo3)
      return 1;
    else
      return 2;
  }
  if (mo2 == mo3) {
    return 2;}
  if (mo1 == mo3) {
    return 4;}
  return 3;
}
const ZooMCP* commonlow(const ZooP* p) {
  if (common(p) != 1) return NULL;
  std::vector<const ZooMCP*> one,two,three;
  const ZooMCP* mo1;
  const ZooP* buffer = (*p)[0];
  mo1 = buffer->assocMCP();
  const ZooMCP* mo2;
  buffer = (*p)[1];
  mo2 = buffer->assocMCP();
  const ZooMCP* mo3;
  buffer = (*p)[2];
  mo3 = buffer->assocMCP();
  for (int i = 0 ; i < 100 ; ++i) {
    one.push_back(mo1);
    if (mo1->NMo() > 0) {mo1 = mo1->Mo(0); --i;}
    else i=1000;
  }
  for (int i = 0 ; i < 100 ; ++i) {
    for (unsigned int j = 0 ; j < one.size() ; ++j)
      if (one[j]==mo2)
        two.push_back(mo2);
    if (mo2->NMo() > 0) {mo2 = mo2->Mo(0); --i;}
    else i=1000;
  }
  for (int i = 0 ; i < 100 ; ++i) {
    for (unsigned int j = 0 ; j < two.size() ; ++j)
      if (mo3==two[j])
        three.push_back(mo3);
    if (mo3->NMo() > 0) {mo3 = mo3->Mo(0); --i;}
    else i=1000;
  }
  return three[0];

}

int commprong(const ZooP* p,int& code) {
  code = 0;
  if (common(p) != 1) return 0;
  std::vector<const ZooMCP*> one,onetwo,onethree;
  const ZooMCP* mo1;
  const ZooP* buffer = (*p)[0];
  mo1 = buffer->assocMCP();
  const ZooMCP* mo2;
  buffer = (*p)[1];
  mo2 = buffer->assocMCP();
  const ZooMCP* mo3;
  buffer = (*p)[2];
  mo3 = buffer->assocMCP();
  for (int i = 0 ; i < 100 ; ++i) {
    one.push_back(mo1);
    if (mo1->NMo() > 0) {mo1 = mo1->Mo(0); --i;}
    else i=1000;
  }
  for (int i = 0 ; i < 100 ; ++i) {
    for (unsigned int j = 0 ; j < one.size() ; ++j)
      if (one[j]==mo2)
        onetwo.push_back(mo2);
    if (mo2->NMo() > 0) {mo2 = mo2->Mo(0); --i;}
    else i=1000;
  }
  for (int i = 0 ; i < 100 ; ++i) {
    for (unsigned int j = 0 ; j < one.size() ; ++j)
      if (one[j]==mo3)
        onethree.push_back(mo3);
    if (mo3->NMo() > 0) {mo3 = mo3->Mo(0); --i;}
    else i=1000;
  }
  if (onetwo[0]!=commonlow(p)) {code = 12;return onetwo[0]->pid();}
  if (onethree[0]!=commonlow(p)) {code = 13;return onethree[0]->pid();}

  one.clear();onetwo.clear();onethree.clear();

  buffer = (*p)[1];
  mo2 = buffer->assocMCP();
  buffer = (*p)[2];
  mo3 = buffer->assocMCP();
  for (int i = 0 ; i < 100 ; ++i) {
    one.push_back(mo2);
    if (mo2->NMo() > 0) {mo2 = mo2->Mo(0); --i;}
    else i=1000;
  }
  for (int i = 0 ; i < 100 ; ++i) {
    for (unsigned int j = 0 ; j < one.size() ; ++j)
      if (one[j]==mo3)
        onetwo.push_back(mo3);
    if (mo3->NMo() > 0) {mo3 = mo3->Mo(0); --i;}
    else i=1000;
  }
  code = 23;
  return onetwo[0]->pid();


}


float trutau(const ZooMCP* p) {
  if (3==p->NCh() && 15==abs(p->pid())) return 1.;
  return 0.;
}

float trutau(const ZooP* p) {
  if (!(p->assocMCP())) {
    //    std::cout << "this is not associated" << std::endl;
    for (unsigned short i = 0 ; i < p->NCh() ; ++i) {
      //      if (p->Ch(i)->assocMCP())
      //        std::cout << p->Ch(i)->assocMCP()->pid() << std::endl;
      //      else
      //        std::cout << "not assoc" << std::endl;
    }
    return 0;
  }
  if (15==abs(p->assocMCP()->pid())) {
    if (3!=p->NCh()) return 3;
//    if (3!=p->assocMCP()->NCh()) {
//      for (unsigned short i = 0 ; i < p->assocMCP()->NCh() ; ++i ) {
//        if (13!=abs(p->assocMCP()->Ch(i)->pid())
//            && 22 !=abs(p->assocMCP()->Ch(i)->pid())) return 6;
//      }
//    }
    std::vector<const ZooMCP*> found;
    for (unsigned short i = 0 ; i < p->NCh() ; ++i) {
      if (!p->Ch(i)->assocMCP()) return 4;
      if (abs(p->Ch(i)->pid()) != abs(p->Ch(i)->assocMCP()->pid())) return 5;
      found.push_back(p->Ch(i)->assocMCP());
      
    }
    for (unsigned short i = 0 ; i < p->assocMCP()->NCh() ; ++i) {
      if (22==abs(p->assocMCP()->Ch(i)->pid())) continue;
      bool good = false;
      for (unsigned short j = 0 ; j < found.size() ; ++j) {
        if (found[j] == p->assocMCP()->Ch(i)) good = true;
      }
      if (!good) return 6;
    }
  }
  return abs(p->assocMCP()->pid());
}

int printtree(const ZooMCP* p, const ZooP* reco) {
  if (abs(p->pid())>99999) return 0;
  //  if (p->momentum().P() < 10 ) return 0;

  bool is_reco = false;
  for (uint k=0; k<reco->NCh(); k++){
    if (reco->Ch(k)->assocMCP() == p){
      is_reco = true;
    }
  }
  if (is_reco){
    printf("%d!",p->pid());
    //fileprint(Form("%d!",p->pid()));
  }
  else{
    printf("%d",p->pid());
    //fileprint( Form("%d",p->pid()));
  }
  if (p->NCh()>0) {
    printf("<%d> -> (",p->NCh());
    //fileprint( Form("<%d> -> (",p->NCh()));
    for (unsigned short a = 0 ; a < p->NCh() ; ++a) {
      printtree(p->Ch(a),reco);
    }
    //fileprint( ")");
    printf(")");
  } else {
    printf(" ");
    //fileprint( " ");
  }
  //fflush(graphviz_file);
  return 0;
}

int printtree(const ZooP* p) {
  const ZooMCP* mo1;
  const ZooMCP* mo2;
  const ZooP* buffer = NULL;
  const ZooP* buffer2 = NULL;

  if (2==common(p)) {
    buffer = (*p)[0];
    buffer2 = (*p)[2];
  }
  else if (3==common(p)) {
    buffer = (*p)[1];
    buffer2 = (*p)[0];
  }
  else if (4==common(p)) {
    buffer = (*p)[0];
    buffer2 = (*p)[1];
  }
  else if (1==common(p)){
    buffer = (*p)[1];
    buffer2 =  NULL;
  }
  else {
    if(commontwo(p) == 0 ){
      printf("\n 0!|0!|0!");
      return 1;
    }
    if( commontwo(p) == 1  ) {
      buffer = (*p)[0];
      buffer2 =  NULL;
    }
    if( commontwo(p) == 2  ) {
      buffer = (*p)[1];
      buffer2 =  NULL;
    }
    if( commontwo(p) == 3  ) {
      buffer = (*p)[2];
      buffer2 =  NULL;
    }
    if( commontwo(p) == 12  ) {
      buffer = (*p)[0];
      buffer2 =  NULL;
    }
    if( commontwo(p) == 13  ) {
      buffer = (*p)[0];
      buffer2 =  NULL;
    }
    if( commontwo(p) == 23  ) {
      buffer = (*p)[1];
      buffer2 =  NULL;
    }
    if( commontwo(p) == 123  ) {
      buffer = (*p)[1];
      buffer2 =(*p)[2] ;
    }
    if( commontwo(p) == 213  ) {
      buffer = (*p)[0];
      buffer2 =(*p)[2] ;
    }
    if( commontwo(p) == 312  ) {
      buffer = (*p)[0];
      buffer2 =(*p)[1] ;
    }
  }
  mo1 = buffer->assocMCP();
  if (NULL == mo1) return 0;
  for (int i = 0 ; i < 100 ; ++i) {
    if (mo1->NMo() > 0) {mo1 = mo1->Mo(0); --i;}
    else i=1000;
  }
  printf("\n");
  //fileprint( "\n");
  //fflush(graphviz_file);
  printtree(mo1, p);

  if (buffer2 == NULL) return 0;
  mo2 = buffer2->assocMCP();
  if (NULL == mo2) return 0;
  for (int i = 0 ; i < 100 ; ++i) {
    if (mo2->NMo() > 0) {mo2 = mo2->Mo(0); --i;}
    else i=1000;
  }
  printf("|");
  //fileprint( "|");
  //fflush(graphviz_file);
  printtree(mo2, p);

  //fflush(graphviz_file);
  return 1;
}

int printroot2(const ZooMCP* p) {
  if (NULL==p) return 1;
  printf("%d(%d)", p->pid(),p->NCh());
  if (p->NMo()>0) {
    printf(" <- ");
    printroot2(p->Mo(0));
  }
  return 0;
}

int printroot2(const ZooP* p) {
  //if (!ghostless(p)) return 1;
  printf("\nP1\t");
  printroot2(p->Ch(0)->assocMCP());
  printf("\nP2\t");
  printroot2(p->Ch(1)->assocMCP());
  printf("\nP3\t");
  printroot2(p->Ch(2)->assocMCP());
  printf("\n\n");
  return 0;
}


int printroot(const ZooMCP* p) {
  printf("%d", p->pid());
  if (p->NMo()>0) {
    printf(" <- ");
    printroot(p->Mo(0));
  }
  return 0;
}

int printroot(const ZooP* p) {
  printf("\n");
  printroot(p->Ch(0)->assocMCP());
  printf("\n");
  printroot(p->Ch(1)->assocMCP());
  printf("\n");
  printroot(p->Ch(2)->assocMCP());
  printf("\n\n");
  return 0;
}
int themother(const ZooP* p) {
  const ZooMCP* mo1;
  const ZooP* buffer = (*p)[0];
  mo1 = buffer->assocMCP();
  if (NULL == mo1) return 0;
  for (int i = 0 ; i < 100 ; ++i) {
    if (mo1->NMo() > 0) {mo1 = mo1->Mo(0); --i;}
    else i=1000;
  }
  return mo1->pid();
}

int missid(const ZooP* p) {
  int retval = 0;
  //  if (!ghostless(p)) return retval;
  const ZooMCP* mo1 = NULL;;
  const ZooP* buffer = (*p)[0];
  mo1 = buffer->assocMCP();
  const ZooMCP* mo2 = NULL;
  buffer = (*p)[1];
  mo2 = buffer->assocMCP();
  const ZooMCP* mo3 = NULL;
  buffer = (*p)[2];
  mo3 = buffer->assocMCP();

  if (mo1) {
    if (abs(mo1->pid()) == 13) retval += 0x1;
    else {
      if (abs(mo1->pid()) == 211) retval += 0x10;
      else {
        if (abs(mo1->pid()) == 321) retval += 0x100;
        else retval += 0x10000;
      }
    }
  } else retval+= 0x1000;

  if (mo2) {
    if (abs(mo2->pid()) == 13) retval += 0x1;
    else {
      if (abs(mo2->pid()) == 211) retval += 0x10;
      else {
        if (abs(mo2->pid()) == 321) retval += 0x100;
        else retval += 0x10000;
      }
    }
  } else retval+= 0x1000;


  if (mo3) {
    if (abs(mo3->pid()) == 13) retval += 0x1;
    else {
      if (abs(mo3->pid()) == 211) retval += 0x10;
      else {
        if (abs(mo3->pid()) == 321) retval += 0x100;
        else retval += 0x10000;
      }
    }
  } else retval+= 0x1000;


  return retval;

}

float fisolationa(const ZooP* part) {
  float val = part->Ch(0)->TrExtraInfo()->value(10004);
  return val;
}
float fisolationb(const ZooP* part) {
  float val = part->Ch(1)->TrExtraInfo()->value(10004);
  return val;
}
float fisolationc(const ZooP* part) {
  float val = part->Ch(2)->TrExtraInfo()->value(10004);
  return val;
}
int isolationa(const ZooP* part) {
  float val = part->Ch(0)->TrExtraInfo()->value(10004);
  return (int)val;
}
int isolationb(const ZooP* part) {
  float val = part->Ch(1)->TrExtraInfo()->value(10004);
  return (int)val;
}
int isolationc(const ZooP* part) {
  float val = part->Ch(2)->TrExtraInfo()->value(10004);
  return (int)val;
}
float isolationd(const ZooP* part) {
  float val = part->Ch(0)->TrExtraInfo()->value(10005);
  return val;
}
float isolatione(const ZooP* part) {
  float val = part->Ch(1)->TrExtraInfo()->value(10005);
  return val;
}
float isolationf(const ZooP* part) {
  float val = part->Ch(2)->TrExtraInfo()->value(10005);
  return val;
}
float isosumm(const ZooP* part) {
  return isolationd(part)+isolatione(part)+isolationf(part);
}
float CDF1(const ZooP* part) {
  return part->Ch(1)->TrExtraInfo()->value(10020);
}
float CDF2(const ZooP* part) {
  return part->Ch(1)->TrExtraInfo()->value(10021);
}
float CDF3(const ZooP* part) {
  return part->Ch(1)->TrExtraInfo()->value(10022);
}
float pt(const ZooMCP* p) {
  return p->momentum().pt();
}
float pt(const ZooP* p) {
  return p->momentum().pt();
}
float momentum(const ZooP* p) {
  return p->momentum().R();
}
float eta(const ZooP* p) {
  return p->momentum().Eta();
}
float phi(const ZooP* p) {
  return p->momentum().Phi();
}
float mupi(const ZooP* p) {
  return p->DLL()->DLLmu();
}
float pip(const ZooP* p) {
  return - p->DLL()->DLLp();
}
float pk(const ZooP* p) {
  return p->DLL()->DLLp() - p->DLL()->DLLk();
}
float ppi(const ZooP* p) {
  return p->DLL()->DLLp();
}
float mupiby15(const ZooP* p) {
  return (p->DLL()->DLLmu())/15.;
}
float muk(const ZooP* p) {
  return p->DLL()->DLLmu() - p->DLL()->DLLk();
}
float minmuk(const ZooP* p) {
  float ret = std::numeric_limits<float>::infinity();
  for (unsigned d = 0 ; d < p->NCh() ; ++d) {
    if (13 == abs(p->Ch(d)->pid()))
      ret = std::min(ret,muk(p->Ch(d)));
  }
  return ret;
}
float minmupi(const ZooP* p) {
  float ret = std::numeric_limits<float>::infinity();
  for (unsigned d = 0 ; d < p->NCh() ; ++d) {
    if (13 == abs(p->Ch(d)->pid()))
      ret = std::min(ret,mupi(p->Ch(d)));
  }
  return ret;
}



float kpi(const ZooP* p) {
  return p->DLL()->DLLk();
}
bool S15(const ZooP* p) {//return true;
  //  std::cout << std::max((*p)[0]->momentum().Rho(), std::max( (*p)[1]->momentum().Rho(), (*p)[2]->momentum().Rho())) << std::endl;
  if (std::max((*p)[0]->momentum().Rho(), std::max( (*p)[1]->momentum().Rho(), (*p)[2]->momentum().Rho())) < 1000) return false;
  if (fabs(p->measuredMass()-1777)>400)return false;
  if (p->Ch(0)->TrInfo()->chi2()/p->Ch(0)->TrInfo()->ndf() > 5) return false;
  if (p->Ch(1)->TrInfo()->chi2()/p->Ch(1)->TrInfo()->ndf() > 5) return false;
  if (p->Ch(2)->TrInfo()->chi2()/p->Ch(2)->TrInfo()->ndf() > 5) return false;
  //if (p->Ch(0)->ipSig() >36) return false;
  //if (p->Ch(2)->ipSig() >36) return false;
  //if (p->Ch(1)->ipSig() >36) return false;
  if (p->decayVertexPacked().chi2() >25 ) return false;
  //if (p->ct() <70) return false;
  return true;
}

bool S17(const ZooP* p) {//return true;
  if (fabs(p->measuredMass()-1777)>400)return false;
  if (p->Ch(0)->TrInfo()->chi2()/p->Ch(0)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(1)->TrInfo()->chi2()/p->Ch(1)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(2)->TrInfo()->chi2()/p->Ch(2)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(0)->ipSig() <3) return false;
  if (p->Ch(2)->ipSig() <3) return false;
  if (p->Ch(1)->ipSig() <3) return false;
  if (p->decayVertexPacked().chi2() >15 ) return false;
  if (p->ct() <0.00038) return false;
  if (p->ipSig() > 15 ) return false;
  return true;
}
TString bckconvert(int i) {
  int muons = i&0xF;
  int pion = (i&0xF0)>>4;
  int kaons = (i&0xF00)>>8;
  int ghosts = (i&0xF000)>>12;
  int rest = (i&0xF0000)>>16;
  TString buffer="";
  for (int j = 0 ; j < muons ; ++j) buffer+="#mu";
  for (int j = 0 ; j < pion ; ++j) buffer+="#pi";
  for (int j = 0 ; j < kaons ; ++j) buffer+="K";
  for (int j = 0 ; j < rest ; ++j) buffer+="X";
  //for (int j = 0 ; j < ghosts ; ++j) buffer+="(ghost)";
  if (ghosts>0&&ghosts<3) buffer+="+";
  if (ghosts==1) buffer+=Form("%dghost",ghosts);
  if (ghosts>1) buffer+=Form("%dghosts",ghosts);
  return buffer;

}

float diratrafo(const ZooP* p) {
  return TMath::Log(1+1./(1.-dira(p)));

}

float diratrafo2(const ZooP* p) {
  return TMath::Log(1+1./(1.-dira(p)));

}

float acosdira(const ZooP* p) {
  return acos(dira(p));
}

float cttrafo(const ZooP* p) {
  return TMath::Exp(-1000*(p->ct()));
}
bool maxS15S17_tau(const ZooP* p) {
  if (std::max((*p)[0]->momentum().Rho(), std::max( (*p)[1]->momentum().Rho(), (*p)[2]->momentum().Rho())) < 1000) return false;
  if (std::min((*p)[0]->momentum().Rho(), std::min( (*p)[1]->momentum().Rho(), (*p)[2]->momentum().Rho())) < 300) return false;
  if (fabs(p->measuredMass()-1777)>400)return false;
  if (p->Ch(0)->TrInfo()->chi2()/p->Ch(0)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(1)->TrInfo()->chi2()/p->Ch(1)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(2)->TrInfo()->chi2()/p->Ch(2)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(0)->ipSig() <3) return false;
  if (p->Ch(2)->ipSig() <3) return false;
  if (p->Ch(1)->ipSig() <3) return false;
  if (p->decayVertexPacked().chi2() >15 ) return false;
  if ((p->ct()*299792.4) < 100 ) return false;
  if (p->ipSig() > 15 ) return false;
  return true;
}

bool maxS15S17_Ds(const ZooP* p) {
  if (std::max((*p)[0]->momentum().Rho(), std::max( (*p)[1]->momentum().Rho(), (*p)[2]->momentum().Rho())) < 2000) return false;
  if (std::min((*p)[0]->momentum().Rho(), std::min( (*p)[1]->momentum().Rho(), (*p)[2]->momentum().Rho())) < 300) return false;
  if (fabs(p->measuredMass()-1968.47)>250)return false;
  if (p->Ch(0)->TrInfo()->chi2()/p->Ch(0)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(1)->TrInfo()->chi2()/p->Ch(1)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(2)->TrInfo()->chi2()/p->Ch(2)->TrInfo()->ndf() > 4) return false;
  if (p->Ch(0)->ipSig() <3.464) return false;
  if (p->Ch(2)->ipSig() <3.464) return false;
  if (p->Ch(1)->ipSig() <3.464) return false;
  if (p->decayVertexPacked().chi2() >15 ) return false;
  if ((p->ct()*299792.4) < 100 ) return false;
  if (p->ipSig() > 3 ) return false;
  if(fabs(1020-combine(p->Ch(0),p->Ch(1))) > 50) return false;  
  return true;
}


float s15s17(const ZooP* p) {
  if (abs(p->pid())==15) {
    if (maxS15S17_tau(p)) return 1.;
    return 0.;
  } else {
    if (maxS15S17_Ds(p)) return 1.;
    return 0.;
  }
}

bool selected(const ZooP* p) {
  if (p->momentum().pt() < 1000) return false;
  return true;
}
float mommass(const ZooP* p) {
  return p->momentum().M();
}
float mcmass(const ZooP* p) {
  if (p->assocMCP()) return p->assocMCP()->momentum().M();
  else return -898;
}
float measmass(const ZooP* p) {
  return p->measuredMass();
}
float dauipone(const ZooP* p) {
  const ZooP* dau = (*p)[0];
  return dau->ip();
}
float dauiptwo(const ZooP* p) {
  const ZooP* dau = (*p)[1];
  return dau->ip();
}
float dauipthree(const ZooP* p) {
  const ZooP* dau = (*p)[2];
  return dau->ip();
}
float d1ips(const ZooP* p) {
  return (*p)[0]->ipSig();
}
float d2ips(const ZooP* p) {
  return (*p)[1]->ipSig();
}
float d3ips(const ZooP* p) {
  return (*p)[2]->ipSig();
}
float mindauips(const ZooP* p) {
  return std::min(d3ips(p),std::min(d1ips(p),d2ips(p)));
}

float trackchi(const ZooP* p) {
  return p->TrInfo()->chi2()/p->TrInfo()->ndf();
}

int tauproduction(const ZooP* p) {
  if (!(p->assocMCP())) return 0;
  const ZooMCP* mcp = p->assocMCP();
  return tauproduction(mcp);
}

int tauproduction(const ZooMCP* mcp) {
  //if (15!=trutau(p)) return 0;

  if (0==mcp->NMo()) return 0x1;
  int retval = 0;
  if (4==(abs(mcp->Mo(0)->pid())/100)%10) retval+= 0x10;
  if (5==(abs(mcp->Mo(0)->pid())/100)%10) retval+= 0x20;
  if (3==(abs(mcp->Mo(0)->pid())/10)%10) retval+= 0x40;
  if (0==mcp->Mo(0)->NMo()) return retval;
  if (4==(abs(mcp->Mo(0)->Mo(0)->pid())/100)%10) retval+= 0x100;
  if (5==(abs(mcp->Mo(0)->Mo(0)->pid())/100)%10) retval+= 0x200;
  if (3==(abs(mcp->Mo(0)->Mo(0)->pid())/10)%10) retval+= 0x400;
  if (0==mcp->Mo(0)->Mo(0)->NMo()) return retval;
  if (4==(abs(mcp->Mo(0)->Mo(0)->Mo(0)->pid())/100)%10) retval+= 0x1000;
  if (5==(abs(mcp->Mo(0)->Mo(0)->Mo(0)->pid())/100)%10) retval+= 0x2000;
  if (3==(abs(mcp->Mo(0)->Mo(0)->Mo(0)->pid())/10)%10) retval+= 0x4000;
  return retval;
}

int combineproduction(const ZooP* p) {
  if (!p->assocMCP()) return -99;
  return combineproduction(p->assocMCP());
}

int combineproduction(const ZooMCP* mcp) {
  return combineproduction(fromB(mcp),fromDp(mcp),fromDs(mcp),baryonic(mcp));
}

int combineproduction(int fromB, int fromDp, int fromDs, int baryonic){
  return (fromB<<2)|(fromDp<<1)|(fromDs<<0)|(baryonic<<3);
}

int baryonic(const ZooMCP* mcp) {
  if (abs(mcp->pid()) > 999 && abs(mcp->pid()) <10000) return 1;
  const ZooMCP* mo = mcp;
  while(mo && mo->NMo() && mo->Mo(0)) {
    mo = mo->Mo(0);
    if (abs(mo->pid()) > 999 && abs(mo->pid()) <10000) return 1;
  }
  return 0;
}

int fromB(const ZooP* p) {
  if (!(p->assocMCP())) return -99;
  if (abs(p->pid()) != abs(p->assocMCP()->pid())) std::cout << "You might be doing something wrong: p->pid() != mcp->pid() !" << std::endl; 
  return fromB(p->assocMCP());
}

int fromDs(const ZooP* p) {
  if (!(p->assocMCP())) return -99;
  if (abs(p->pid()) != abs(p->assocMCP()->pid())) std::cout << "You might be doing something wrong: p->pid() != mcp->pid() !" << std::endl; 
  return fromDs(p->assocMCP());
}

int fromDp(const ZooP* p) {
  if (!(p->assocMCP())) return -99;
  if (abs(p->pid()) != abs(p->assocMCP()->pid())) std::cout << "You might be doing something wrong: p->pid() != mcp->pid() !" << std::endl; 
  return fromDp(p->assocMCP());
}

int fromB(const ZooMCP* mcp) {
  if (abs(mcp->pid())==15) {
    int fromB = 0;
    const ZooMCP* mo = mcp;
    while (mo->NMo() && mo->Mo(0)) {
      mo = mo->Mo(0);
      if (5==(abs(mo->pid())/100)%10) return 1;
      /*if (4==(abs(mo->pid())/100)%10) {
        if (!(3==(abs(mo->pid())/10)%10))
          fromDp=1;
      }*/
    }
    return fromB;
  }
  if (abs(mcp->pid())==431) {
    int fromB = 0;
    const ZooMCP* mo = mcp;
    while (mo->NMo() && mo->Mo(0)) {
      mo = mo->Mo(0);
      if (5==(abs(mo->pid())/100)%10) return 1;
    }
    return fromB;
  }
  return -90;
}

int fromDp(const ZooMCP* mcp) {
  if (abs(mcp->pid())==15) {
    int fromDp = 0;
    const ZooMCP* mo = mcp;
    while (mo->NMo() && mo->Mo(0)) {
      mo = mo->Mo(0);
      //if (5==(abs(mo->pid())/100)%10) fromB = 1;
      if (4==(abs(mo->pid())/100)%10) {
        if (!(3==(abs(mo->pid())/10)%10))
          if (!(abs(mo->pid())<10000 && abs(mo->pid()>999))) // make sure it's no baryon
            return 1;//fromDp=true;
      }
    }
    return fromDp;
  }
  if (abs(mcp->pid())==431) return 0;
  return -90;
}
int fromD0(const ZooMCP* mcp) {
  if (abs(mcp->pid())==15) {
    int fromD0 = 0;
    const ZooMCP* mo = mcp;
    while (mo->NMo() && mo->Mo(0)) {
      mo = mo->Mo(0);
      //if (5==(abs(mo->pid())/100)%10) fromB = 1;
      if (4==(abs(mo->pid())/100)%10) {
        if ((2==(abs(mo->pid())/10)%10))
          if (!(abs(mo->pid())<10000 && abs(mo->pid()>999))) { // make sure it's no baryon
            if (abs(mo->pid()) == 421) return 1;
            else return 2;
          }
      }
    }
    return fromD0;
  }
  if (abs(mcp->pid())==431) return 0;
  return -90;
}
int D0PID(const ZooMCP* mcp) {
  if (abs(mcp->pid())==15) {
    int fromD0 = 0;
    const ZooMCP* mo = mcp;
    while (mo->NMo() && mo->Mo(0)) {
      mo = mo->Mo(0);
      //if (5==(abs(mo->pid())/100)%10) fromB = 1;
      if (4==(abs(mo->pid())/100)%10) {
        if ((2==(abs(mo->pid())/10)%10))
          if (!(abs(mo->pid())<10000 && abs(mo->pid()>999))) { // make sure it's no baryon
            return mo->pid();
          }
      }
    }
    return fromD0;
  }
  if (abs(mcp->pid())==431) return 0;
  return -90;
}

int fromDs(const ZooMCP* mcp) {
  if (abs(mcp->pid())==15) {
    int fromDs = 0;
    const ZooMCP* mo = mcp;
    while (mo->NMo() && mo->Mo(0)) {
      mo = mo->Mo(0);
    //  if (5==(abs(mo->pid())/100)%10) fromB = 1;
      if (4==(abs(mo->pid())/100)%10) {
        if (3==(abs(mo->pid())/10)%10)
          if (!(abs(mo->pid())<10000 && abs(mo->pid()>999))) // make sure it's no baryon
            return 1;//fromDs=true;
    //    else
    //      fromDp=1;
      }
    }
    return fromDs;
  }
  if (abs(mcp->pid())==431) return 0;
  return -90;
}

float productionweighterrorMC11v3(const ZooP* p) {
  if (!(p->assocMCP())) return 1.;
  const ZooMCP* mcp = p->assocMCP();
  if (abs(p->pid())==15) {
    if ( fromB(mcp) && !fromDs(mcp) && !fromDp(mcp)) return 0.08;
    if (!fromB(mcp) &&  fromDs(mcp)) return 0.1;
    if (!fromB(mcp) &&  fromDp(mcp)) return 1.7;
    if ( fromB(mcp) &&  fromDs(mcp)) return 0.3;
    if ( fromB(mcp) &&  fromDp(mcp)) return 0.8;
    if (baryonic(mcp)) return 0.08;
    return 0.;
  }
  if (abs(p->pid())==431) {

    if (fromB(mcp)) return 0.3;
    return 0.10;
  }
  return 0.;
}
float productionweightMC11v3(const ZooP* p) {
  if (!(p->assocMCP())) return 1.;
  const ZooMCP* mcp = p->assocMCP();
  if (abs(p->pid())==15) {
    if ( fromB(mcp) && !fromDs(mcp) && !fromDp(mcp)) return 0.43;
    if (!fromB(mcp) &&  fromDs(mcp)) return 1.4;
    if (!fromB(mcp) &&  fromDp(mcp)) return 1.7;
    if ( fromB(mcp) &&  fromDs(mcp)) return 1.3;
    if ( fromB(mcp) &&  fromDp(mcp)) return 0.9;
    if (baryonic(mcp)) return 0.43;
    return 1.;
  }
  if (abs(p->pid())==431) {

    if (fromB(mcp)) return 1.3;
    return 1.40;
  }
  return 1.;
}
float productionweighterrorMC11v2(const ZooP* p) {
  if (!(p->assocMCP())) return 1.;
  const ZooMCP* mcp = p->assocMCP();
  if (abs(p->pid())==15) {
    if ( fromB(mcp) && !fromDs(mcp) && !fromDp(mcp)) return 0.09;
    if (!fromB(mcp) &&  fromDs(mcp)) return 0.1;
    if (!fromB(mcp) &&  fromDp(mcp)) return 1.7;
    if ( fromB(mcp) &&  fromDs(mcp)) return 0.3;
    if ( fromB(mcp) &&  fromDp(mcp)) return 0.9;
    if (baryonic(mcp)) return 0.09;
    return 0.;
  }
  if (abs(p->pid())==431) {

    if (fromB(mcp)) return 0.3;
    return 0.10;
  }
  return 0.;
}
float productionweightMC11v2(const ZooP* p) {
  if (!(p->assocMCP())) return 1.;
  const ZooMCP* mcp = p->assocMCP();
  if (abs(p->pid())==15) {
    if ( fromB(mcp) && !fromDs(mcp) && !fromDp(mcp)) return 0.43;
    if (!fromB(mcp) &&  fromDs(mcp)) return 1.4;
    if (!fromB(mcp) &&  fromDp(mcp)) return 1.7;
    if ( fromB(mcp) &&  fromDs(mcp)) return 1.3;
    if ( fromB(mcp) &&  fromDp(mcp)) return 1.4;
    if (baryonic(mcp)) return 0.43;
    return 1.;
  }
  if (abs(p->pid())==431) {

    if (fromB(mcp)) return 1.3;
    return 1.40;
  }
  return 1.;
}
float productionweighterrorMC11(const ZooP* p) {
  if (!(p->assocMCP())) return 1.;
  const ZooMCP* mcp = p->assocMCP();
  if (abs(p->pid())==15) {
    if ( fromB(mcp) && !fromDs(mcp) && !fromDp(mcp)) return 0.08;
    if (!fromB(mcp) &&  fromDs(mcp)) return 0.1;
    if (!fromB(mcp) &&  fromDp(mcp)) return 1.8;
    if ( fromB(mcp) &&  fromDs(mcp)) return 0.3;
    if ( fromB(mcp) &&  fromDp(mcp)) return 1.3;
    if (baryonic(mcp)) return 0.08;
    return 0.;
  }
  if (abs(p->pid())==431) {

    if (fromB(mcp)) return 0.2;
    return 0.08;
  }
  return 0.;
}
float productionweightMC11(const ZooP* p) {
  if (!(p->assocMCP())) return 1.;
  const ZooMCP* mcp = p->assocMCP();
  if (abs(p->pid())==15) {
    if ( fromB(mcp) && !fromDs(mcp) && !fromDp(mcp)) return 0.43;
    if (!fromB(mcp) &&  fromDs(mcp)) return 1.4;
    if (!fromB(mcp) &&  fromDp(mcp)) return 1.9;
    if ( fromB(mcp) &&  fromDs(mcp)) return 1.3;
    if ( fromB(mcp) &&  fromDp(mcp)) return 1.3;
    if (baryonic(mcp)) return 0.43;
    return 1.;
  }
  if (abs(p->pid())==431) {

    if (fromB(mcp)) return 1.1;
    return 1.08;
  }
  return 1.;
}
float productionweighterror(const ZooP* p) {
  if (!(p->assocMCP())) return 1.;
  const ZooMCP* mcp = p->assocMCP();
  if (abs(p->pid())==15) {
    if ( fromB(mcp) && !fromDs(mcp) && !fromDp(mcp)) return 0.1;
    if (!fromB(mcp) &&  fromDs(mcp)) return 0.08;
    if (!fromB(mcp) &&  fromDp(mcp)) return 2.2;
    if ( fromB(mcp) &&  fromDs(mcp)) return 0.2;
    if ( fromB(mcp) &&  fromDp(mcp)) return 0.1;
    if (baryonic(mcp)) return 0.1;
    return 0.;
  }
  if (abs(p->pid())==431) {

    if (fromB(mcp)) return 0.2;
    return 0.08;
  }
  return 0.;
}
float productionweight(const ZooP* p) {
  if (!(p->assocMCP())) return 1.;
  const ZooMCP* mcp = p->assocMCP();
  if (abs(p->pid())==15) {
    if ( fromB(mcp) && !fromDs(mcp) && !fromDp(mcp)) return 0.7;
    if (!fromB(mcp) &&  fromDs(mcp)) return 1.08;
    if (!fromB(mcp) &&  fromDp(mcp)) return 2.3;
    if ( fromB(mcp) &&  fromDs(mcp)) return 1.1;
    if ( fromB(mcp) &&  fromDp(mcp)) return 1.5;
    if (baryonic(mcp)) return 0.7;
    return 1.;
  }
  if (abs(p->pid())==431) {

    if (fromB(mcp)) return 1.1;
    return 1.08;
  }
  return 1.;
}
int tauorder(const ZooP* part) {
  const ZooP* one = (*part)[0];
  const ZooP* two = (*part)[1];
  const ZooP* three = (*part)[2];
  if (one->momentum().Rho() > two->momentum().Rho()) // 1,2,4
    if (one->momentum().Rho() > three->momentum().Rho()) // 1,4
      if (two->momentum().Rho() > three->momentum().Rho()) // 1
        return 1;
      else
        return 4;
    else
      return 2;
  else // 3,5,6
    if (one->momentum().Rho() > three->momentum().Rho()) // 5
      return 5;
    else
      if (two->momentum().Rho() > three->momentum().Rho())
        return 3;
      else
        return 6;
  return 0;
}

int isolationg(const ZooP* part) {
  float val = part->Ch(0)->TrExtraInfo()->value(10006); 
  /*int retval = (int)val;
    int ex =  part->Ch(0)->TrExtraInfo()->exists(10006);
    printf("%d\t%f\t%f\t%d\n",ex,val,(double)val,retval);*/
  return (int)val;
}
int isolationh(const ZooP* part) {
  float val = part->Ch(1)->TrExtraInfo()->value(10006); 
  return (int)val;
}
int isolationi(const ZooP* part) {
  float val = part->Ch(2)->TrExtraInfo()->value(10006); 
  return (int)val;
}
int isolationj(const ZooP* part) {
  float val = part->Ch(0)->TrExtraInfo()->value(10007); 
  return (int)val;
}
int isolationk(const ZooP* part) {
  float val = part->Ch(1)->TrExtraInfo()->value(10007); 
  return (int)val;
}
int isolationl(const ZooP* part) {
  float val = part->Ch(2)->TrExtraInfo()->value(10007); 
  return (int)val;
}

int chargeorder(const ZooP* part) {
  const ZooP* one = (*part)[0];
  const ZooP* two = (*part)[1];
  const ZooP* three = (*part)[2];
  int retval =0;
  if (one->charge()>0) retval+=1;
  if (two->charge()>0) retval+=2;
  if (three->charge()>0) retval+=4;
  return retval;
}

float bpvipchi2(const ZooP* p) {
  return p->ParticleInfo()->value(10030);
}
float bpvltime225(const ZooP* p) {
  return p->ParticleInfo()->value(10031);
}
float bpvltime(const ZooP* p) {
  return p->ParticleInfo()->value(10032);
}
float cloned(const ZooP* p) {
  int m_num = 101;
  if (p->TrExtraInfo()) {
    if (p->TrExtraInfo()->exists(m_num)) {
      return p->TrExtraInfo()->value(m_num);
    } else {
      //std::cout << "this info doesn't exist" << std::endl;
    }
  } else {
    std::cout << "no extra info" << std::endl;
  }
  return 5000.;//std::numeric_limits<float>::quiet_NaN();

}

float cloned1(const ZooP* pa) {
  const ZooP* p = pa->Ch(0);
  int m_num = 101;
  if (p->TrExtraInfo()) {
    if (p->TrExtraInfo()->exists(m_num)) {
      return p->TrExtraInfo()->value(m_num);
    } else {
      //std::cout << "this info doesn't exist" << std::endl;
    }
  } else {
    std::cout << "no extra info" << std::endl;
  }
  return 5000.;//std::numeric_limits<float>::quiet_NaN();

}

float cloned2(const ZooP* pa) {
  const ZooP* p = pa->Ch(1);
  int m_num = 101;
  if (p->TrExtraInfo()) {
    if (p->TrExtraInfo()->exists(m_num)) {
      return p->TrExtraInfo()->value(m_num);
    } else {
      //std::cout << "this info doesn't exist" << std::endl;
    }
  } else {
    std::cout << "no extra info" << std::endl;
  }
  return 5000.;//std::numeric_limits<float>::quiet_NaN();

}

float cloned3(const ZooP* pa) {
  const ZooP* p = pa->Ch(2);
  int m_num = 101;
  if (p->TrExtraInfo()) {
    if (p->TrExtraInfo()->exists(m_num)) {
      return p->TrExtraInfo()->value(m_num);
    } else {
      //std::cout << "this info doesn't exist" << std::endl;
    }
  } else {
    std::cout << "no extra info" << std::endl;
  }
  return 5000.;//std::numeric_limits<float>::quiet_NaN();

}

float unscaledHlt2Dec(const ZooP* , const ZooEv* pev) {
  return (float)(int)(pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuonHighPT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiElectronHighMass >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TFBc2JpsiMuX >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiElectronB >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TFBc2JpsiMuXSignal >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo2BodySimple >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2B2HHPi0_Merged >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo3BodySimple >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2HHH >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo4BodySimple >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo2BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo3BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonB >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo4BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu2BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetached >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu3BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetachedHeavy >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu4BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetachedJPsi >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE2BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TriMuonDetached >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE3BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TriMuonTau >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE4BodyBBDT >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHHH >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2IncPhi >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHKsLL >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPi >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KK >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KPi >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0Pi >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0K >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bs2PhiGamma >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bd2KstGamma >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD2HMuMu >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmRareDecayD02MuMu >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HHMuMu >()
      || pev->getTriggerDec<TriggerDecisions::Hlt2B2HH>()
      );
}


float theL0Dec(const ZooP*, const ZooEv* pev) {
  return (float)(int) (pev->getTriggerDec<TriggerDecisions::L0Muon>() 
      || pev->getTriggerDec<TriggerDecisions::L0DiMuon>() 
      || pev->getTriggerDec<TriggerDecisions::L0Electron>() 
      || pev->getTriggerDec<TriggerDecisions::L0Photon>() 
      || pev->getTriggerDec<TriggerDecisions::L0Hadron>() 
      );
}

float minbias(const ZooP*, const ZooEv* pev) {
  return (float)(int) (
      (
      pev->getTriggerDec<TriggerDecisions::Hlt1MBNoBias>() 
   || pev->getTriggerDec<TriggerDecisions::Hlt1MBMicroBiasVelo>() 
   || pev->getTriggerDec<TriggerDecisions::Hlt1MBMicroBiasVeloRateLimited>() 
   || pev->getTriggerDec<TriggerDecisions::Hlt1MBMicroBiasTStation>() 
   || pev->getTriggerDec<TriggerDecisions::Hlt1MBMicroBiasTStationRateLimited>() 
      ) &&
      (
      pev->getTriggerDec<TriggerDecisions::Hlt2PassThrough>() 
   || pev->getTriggerDec<TriggerDecisions::Hlt2Forward>() 
      )
      );
}
float Hlt1Dec(const ZooP* , const ZooEv* pev) {
  return (float)(int) (pev->getTriggerDec<TriggerDecisions::Hlt1DiMuonLowMass>()
      ||pev->getTriggerDec<TriggerDecisions::Hlt1DiMuonHighMass>() 
      ||pev->getTriggerDec<TriggerDecisions::Hlt1SingleMuonHighPT>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt1TrackAllL0>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt1TrackPhoton>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt1TrackMuon>() );
}

float L0Hlt1DecMu(const ZooP* , const ZooEv* pev) {
  return (float)(int) (pev->getTriggerDec<TriggerDecisions::L0Muon>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt1TrackMuon>() );
}

float Hlt2DecSingleMu(const ZooP* , const ZooEv* pev) {
  return (float)(int) (pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuon>() );
}

float Hlt2DecMu(const ZooP* , const ZooEv* pev) {
  return (float)(int) (pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuon>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuonHighPT>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonJPsi>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonJPsiHighPT>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetached>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetachedHeavy>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetachedJPsi>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2TriMuonDetached>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2TriMuonTau>() 
      );
}

float Hlt2DecDhhh(const ZooP* , const ZooEv* pev) {
  return (float)(int) (pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHHH>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHHHWideMass>()
      );
}

float Hlt2Dec(const ZooP* , const ZooEv* pev) {
  return (float)(int) (pev->getTriggerDec<TriggerDecisions::Hlt2SingleElectronTFLowPt>()    
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleElectronTFHighPt>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiElectronHighMass>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiElectronB>()              
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo2BodySimple>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo3BodySimple>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo4BodySimple>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo2BodyBBDT>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo3BodyBBDT>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo4BodyBBDT>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu2BodyBBDT>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu3BodyBBDT>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu4BodyBBDT>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE2BodyBBDT>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE3BodyBBDT>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE4BodyBBDT>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2IncPhi>()                  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHKsLL>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2Dst2PiD02PiPi>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2Dst2PiD02MuMu>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2Dst2PiD02KMu>()            
      || pev->getTriggerDec<TriggerDecisions::Hlt2Dst2PiD02KPi>()            
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02KK>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02KKWideMass>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02KPi>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02KPiWideMass>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02PiPi>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02PiPiWideMass>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bs2PhiGamma>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bd2KstGamma>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0Pi>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0K>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmRareDecayD02MuMu>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2B2HH>()                 
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuon>()     
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuonHighPT>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuonLowPT>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2TFBc2JpsiMuX>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2TFBc2JpsiMuXSignal>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2B2HHPi0_Merged>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2HHH>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2HHHWideMass>()
		       //      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonJPsi>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonJPsiHighPT>()    
		       //      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonPsi2S>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonB>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonZ>()          
		       //      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDY1>()        
		       //      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDY2>()        
		       //      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDY3>()        
		       //      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDY4>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetached>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetachedHeavy>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetachedJPsi>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2TriMuonDetached>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2TriMuonTau>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHHH>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHHHWideMass>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2B2HHLTUnbiased>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPi>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPiWideMass>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KK>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KKWideMass>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KPi>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KPiWideMass>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0Pi>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0K>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02KMuNuWS>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02PiMuNuWS>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02KMuNu>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02PiMuNu>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD2HMuMu>()              
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD2HMuMuWideMass>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HHMuMu>()            
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HHMuMuWideMass>()   
      );
}

float Hlt2DecOld(const ZooP* , const ZooEv* pev) {
  return (float)(int) (pev->getTriggerDec<TriggerDecisions::Hlt2SingleElectronTFLowPt>()    
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleElectronTFHighPt>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiElectronHighMass>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiElectronB>()              
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo2BodySimple>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo3BodySimple>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo4BodySimple>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo2BodyBBDT>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo3BodyBBDT>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2Topo4BodyBBDT>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu2BodyBBDT>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu3BodyBBDT>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoMu4BodyBBDT>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE2BodyBBDT>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE3BodyBBDT>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2TopoE4BodyBBDT>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2IncPhi>()                  
      || pev->getTriggerDec<TriggerDecisions::Hlt2IncPhiSidebands>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHKsLL>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2Dst2PiD02PiPi>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2Dst2PiD02MuMu>()           
      || pev->getTriggerDec<TriggerDecisions::Hlt2Dst2PiD02KMu>()            
      || pev->getTriggerDec<TriggerDecisions::Hlt2Dst2PiD02KPi>()            
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02KK>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02KKWideMass>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02KPi>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02KPiWideMass>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02PiPi>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02PiPiWideMass>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bs2PhiGamma>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bs2PhiGammaWideBMass>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bd2KstGamma>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bd2KstGammaWideKMass>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2Bd2KstGammaWideBMass>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0Pi>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0K>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmRareDecayD02MuMu>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2B2HH>()                 
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuon>()     
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuonHighPT>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuonLowPT>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2TFBc2JpsiMuX>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2TFBc2JpsiMuXSignal>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2B2HHPi0_Merged>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2HHH>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2HHHWideMass>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonJPsi>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonJPsiHighPT>()    
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonPsi2S>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonB>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonZ>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDY1>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDY2>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDY3>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDY4>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetached>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetachedHeavy>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2DiMuonDetachedJPsi>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2TriMuonDetached>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2TriMuonTau>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHHH>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HHHHWideMass>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2B2HHLTUnbiased>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPi>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPiWideMass>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KK>()          
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KKWideMass>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KPi>()         
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD02HH_D02KPiWideMass>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0Pi>()       
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0K>()        
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02KMuNuWS>() 
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02PiMuNuWS>()
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02KMuNu>()   
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02PiMuNu>()  
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD2HMuMu>()              
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD2HMuMuWideMass>()      
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HHMuMu>()            
      || pev->getTriggerDec<TriggerDecisions::Hlt2CharmSemilepD02HHMuMuWideMass>()   
      );
}

int istruemuon(const ZooP* p) {
  return (int)(bool)((p->assocMCP())&&(13==abs(p->assocMCP()->pid())));
}

int mcpid(const ZooP* p) {
  if (NULL==p) return 0;
  if (!(p->assocMCP())) return 0;
  return (p->assocMCP()->pid());
  }

float float_ndau(const ZooMCP* p) {
  if (NULL==p) return -1.;
  return (float)(int) p->NCh();
}
float float_mcpid(const ZooP* p) {
  if (NULL==p) return 0.;
  if (!(p->assocMCP())) return 0.;
  return (float)(int)(p->assocMCP()->pid());
  }

int mcpid(const ZooMCP* p) {
  if (NULL==p) return 0.;
  return (int)(p->pid());
  }
float float_mcpid(const ZooMCP* p) {
  if (NULL==p) return 0.;
  return (float)(int)(p->pid());
  }

const ZooMCP* mother(const ZooMCP* p) {
  if (NULL==p) return NULL;
  if (0==p->NMo()) return NULL;
  return p->Mo(0);
}

const ZooMCP* mother(const ZooP* p) {
  if (NULL==p) return NULL;
  if (NULL==p->assocMCP()) return NULL;
  if (0==p->assocMCP()->NMo()) return NULL;
  return p->assocMCP()->Mo(0);
}


float float_mother_ndau(const ZooP* p) {
  return float_ndau(mother(p));
}

//float  float_nchargeddau(const ZooMCP* p) {
//  if (NULL==p) return -1.;
//  int i = 0;
//  for (int k = 0 ; k < p->NCh() ; ++k) {
//    if (p->Ch(k)->charge()) ++i;
//  }
//  return (float)(int)i;
//}
//
//float float_mother_nchargeddau(const ZooP* p) {
//  return float_nchargeddau(mother(p));
//}
//

float float_mother_mcpid(const ZooP* p) {
  return float_mcpid(mother(p));
}
float mother_pt(const ZooP* p) {
  if (mother(p)) return pt(mother(p));
  return -100.;
}
float float_mother_mother_mcpid(const ZooP* p) {
  return float_mcpid(mother(mother(p)));
}
float float_mother_mother_mother_mcpid(const ZooP* p) {
  return float_mcpid(mother(mother(mother(p))));
}
float float_mother_mother_mother_mother_mcpid(const ZooP* p) {
  return float_mcpid(mother(mother(mother(mother(p)))));
}

int key(const ZooMCP* mcp) {
  long long buffer = reinterpret_cast<int>(mcp);
  return buffer;
}

int p0_mother_key(const ZooP* p) {
  return key(mother(p->Ch(0)));
}
int p0_mother_mother_key(const ZooP* p) {
  return key(mother(mother(p->Ch(0))));
}
int p0_mother_mother_mother_key(const ZooP* p) {
  return key(mother(mother(mother(p->Ch(0)))));
}
int p0_mother_mother_mother_mother_key(const ZooP* p) {
  return key(mother(mother(mother(mother(p->Ch(0))))));
}

int p1_mother_key(const ZooP* p) {
  return key(mother(p->Ch(1)));
}
int p1_mother_mother_key(const ZooP* p) {
  return key(mother(mother(p->Ch(1))));
}
int p1_mother_mother_mother_key(const ZooP* p) {
  return key(mother(mother(mother(p->Ch(1)))));
}
int p1_mother_mother_mother_mother_key(const ZooP* p) {
  return key(mother(mother(mother(mother(p->Ch(1))))));
}


int p2_mother_key(const ZooP* p) {
  return key(mother(p->Ch(2)));
}
int p2_mother_mother_key(const ZooP* p) {
  return key(mother(mother(p->Ch(2))));
}
int p2_mother_mother_mother_key(const ZooP* p) {
  return key(mother(mother(mother(p->Ch(2)))));
}
int p2_mother_mother_mother_mother_key(const ZooP* p) {
  return key(mother(mother(mother(mother(p->Ch(2))))));
}




int L0TIS(const ZooP* p) {
  const ZooTrigger* pev = p->TrigDec();
  if (!pev) {std::cerr << "no trigger for this particle" << std::endl; return -1;}
  if (!pev) {std::cerr << "no trigger for this particle" << std::endl; return -1;}

  return (float)(int) (pev->getTIS<TriggerDecisions::L0Muon>() 
      || pev->getTIS<TriggerDecisions::L0DiMuon>() 
      || pev->getTIS<TriggerDecisions::L0Electron>() 
      || pev->getTIS<TriggerDecisions::L0Photon>() 
      || pev->getTIS<TriggerDecisions::L0Hadron>() 
      );
}
int L0TOS(const ZooP* p) {
  const ZooTrigger* pev = p->TrigDec();
  if (!pev) {std::cerr << "no trigger for this particle" << std::endl; return -1;}
  if (!pev) {std::cerr << "no trigger for this particle" << std::endl; return -1;}

  return (float)(int) (pev->getTOS<TriggerDecisions::L0Muon>() 
      || pev->getTOS<TriggerDecisions::L0DiMuon>() 
      || pev->getTOS<TriggerDecisions::L0Electron>() 
      || pev->getTOS<TriggerDecisions::L0Photon>() 
      || pev->getTOS<TriggerDecisions::L0Hadron>() 
      );
}
int Hlt1TIS(const ZooP* p) {
  const ZooTrigger* pev = p->TrigDec();
  if (!pev) std::cerr << "no trigger for this particle" << std::endl;
  if (!pev) {std::cerr << "no trigger for this particle" << std::endl; return -1;}

  return (float)(int) (pev->getTIS<TriggerDecisions::Hlt1DiMuonLowMass>()
      ||pev->getTIS<TriggerDecisions::Hlt1DiMuonHighMass>() 
      ||pev->getTIS<TriggerDecisions::Hlt1SingleMuonHighPT>() 
      ||pev->getTIS<TriggerDecisions::Hlt1TrackAllL0>() 
      ||pev->getTIS<TriggerDecisions::Hlt1TrackPhoton>() 
      ||pev->getTIS<TriggerDecisions::Hlt1TrackMuon>() );
}
int Hlt1TOS(const ZooP* p) {
  const ZooTrigger* pev = p->TrigDec();
  if (!pev) std::cerr << "no trigger for this particle" << std::endl;
  if (!pev) {std::cerr << "no trigger for this particle" << std::endl; return -1;}

  return (float)(int) (pev->getTOS<TriggerDecisions::Hlt1DiMuonLowMass>()
      ||pev->getTOS<TriggerDecisions::Hlt1DiMuonHighMass>() 
      ||pev->getTOS<TriggerDecisions::Hlt1SingleMuonHighPT>() 
      ||pev->getTOS<TriggerDecisions::Hlt1TrackAllL0>() 
      ||pev->getTOS<TriggerDecisions::Hlt1TrackPhoton>() 
      ||pev->getTOS<TriggerDecisions::Hlt1TrackMuon>() );
}
int Hlt2TIS(const ZooP* p) {
  const ZooTrigger* pev = p->TrigDec();
  if (!pev) std::cerr << "no trigger for this particle" << std::endl;
  if (!pev) {std::cerr << "no trigger for this particle" << std::endl; return -1;}

  return (float)(int) (pev->getTIS<TriggerDecisions::Hlt2SingleElectronTFLowPt>()    
      || pev->getTIS<TriggerDecisions::Hlt2SingleElectronTFHighPt>()   
      || pev->getTIS<TriggerDecisions::Hlt2DiElectronHighMass>()       
      || pev->getTIS<TriggerDecisions::Hlt2DiElectronB>()              
      || pev->getTIS<TriggerDecisions::Hlt2Topo2BodySimple>()         
      || pev->getTIS<TriggerDecisions::Hlt2Topo3BodySimple>()         
      || pev->getTIS<TriggerDecisions::Hlt2Topo4BodySimple>()         
      || pev->getTIS<TriggerDecisions::Hlt2Topo2BodyBBDT>()           
      || pev->getTIS<TriggerDecisions::Hlt2Topo3BodyBBDT>()           
      || pev->getTIS<TriggerDecisions::Hlt2Topo4BodyBBDT>()           
      || pev->getTIS<TriggerDecisions::Hlt2TopoMu2BodyBBDT>()         
      || pev->getTIS<TriggerDecisions::Hlt2TopoMu3BodyBBDT>()         
      || pev->getTIS<TriggerDecisions::Hlt2TopoMu4BodyBBDT>()         
      || pev->getTIS<TriggerDecisions::Hlt2TopoE2BodyBBDT>()          
      || pev->getTIS<TriggerDecisions::Hlt2TopoE3BodyBBDT>()          
      || pev->getTIS<TriggerDecisions::Hlt2TopoE4BodyBBDT>()          
      || pev->getTIS<TriggerDecisions::Hlt2IncPhi>()                  
      || pev->getTIS<TriggerDecisions::Hlt2IncPhiSidebands>()         
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HHKsLL>()       
      || pev->getTIS<TriggerDecisions::Hlt2Dst2PiD02PiPi>()           
      || pev->getTIS<TriggerDecisions::Hlt2Dst2PiD02MuMu>()           
      || pev->getTIS<TriggerDecisions::Hlt2Dst2PiD02KMu>()            
      || pev->getTIS<TriggerDecisions::Hlt2Dst2PiD02KPi>()            
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02KK>()          
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02KKWideMass>()  
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02KPi>()         
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02KPiWideMass>() 
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02PiPi>()        
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02PiPiWideMass>()  
      || pev->getTIS<TriggerDecisions::Hlt2Bs2PhiGamma>()      
      || pev->getTIS<TriggerDecisions::Hlt2Bs2PhiGammaWideBMass>()
      || pev->getTIS<TriggerDecisions::Hlt2Bd2KstGamma>()         
      || pev->getTIS<TriggerDecisions::Hlt2Bd2KstGammaWideKMass>() 
      || pev->getTIS<TriggerDecisions::Hlt2Bd2KstGammaWideBMass>() 
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD2KS0Pi>()      
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD2KS0K>()       
      || pev->getTIS<TriggerDecisions::Hlt2CharmRareDecayD02MuMu>()
      || pev->getTIS<TriggerDecisions::Hlt2B2HH>()                 
      || pev->getTIS<TriggerDecisions::Hlt2SingleMuon>()     
      || pev->getTIS<TriggerDecisions::Hlt2SingleMuonHighPT>() 
      || pev->getTIS<TriggerDecisions::Hlt2SingleMuonLowPT>()  
      || pev->getTIS<TriggerDecisions::Hlt2TFBc2JpsiMuX>()      
      || pev->getTIS<TriggerDecisions::Hlt2TFBc2JpsiMuXSignal>()
      || pev->getTIS<TriggerDecisions::Hlt2B2HHPi0_Merged>()      
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD2HHH>()       
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD2HHHWideMass>()
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonJPsi>()          
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonJPsiHighPT>()    
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonPsi2S>()       
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonB>()          
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonZ>()          
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonDY1>()        
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonDY2>()        
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonDY3>()        
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonDY4>()        
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonDetached>()   
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonDetachedHeavy>()
      || pev->getTIS<TriggerDecisions::Hlt2DiMuonDetachedJPsi>() 
      || pev->getTIS<TriggerDecisions::Hlt2TriMuonDetached>()   
      || pev->getTIS<TriggerDecisions::Hlt2TriMuonTau>()        
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HHHH>()   
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HHHHWideMass>()
      || pev->getTIS<TriggerDecisions::Hlt2B2HHLTUnbiased>()         
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPi>()  
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPiWideMass>()
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HH_D02KK>()          
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HH_D02KKWideMass>()  
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HH_D02KPi>()         
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD02HH_D02KPiWideMass>() 
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0Pi>()       
      || pev->getTIS<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0K>()        
      || pev->getTIS<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02KMuNuWS>() 
      || pev->getTIS<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02PiMuNuWS>()
      || pev->getTIS<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02KMuNu>()   
      || pev->getTIS<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02PiMuNu>()  
      || pev->getTIS<TriggerDecisions::Hlt2CharmSemilepD2HMuMu>()              
      || pev->getTIS<TriggerDecisions::Hlt2CharmSemilepD2HMuMuWideMass>()      
      || pev->getTIS<TriggerDecisions::Hlt2CharmSemilepD02HHMuMu>()            
      || pev->getTIS<TriggerDecisions::Hlt2CharmSemilepD02HHMuMuWideMass>()   
      );
}
int Hlt2TOS(const ZooP* p) {
  const ZooTrigger* pev = p->TrigDec();
  if (!pev) std::cerr << "no trigger for this particle" << std::endl;
  if (!pev) {std::cerr << "no trigger for this particle" << std::endl; return -1;}

  return (float)(int) (pev->getTOS<TriggerDecisions::Hlt2SingleElectronTFLowPt>()    
      || pev->getTOS<TriggerDecisions::Hlt2SingleElectronTFHighPt>()   
      || pev->getTOS<TriggerDecisions::Hlt2DiElectronHighMass>()       
      || pev->getTOS<TriggerDecisions::Hlt2DiElectronB>()              
      || pev->getTOS<TriggerDecisions::Hlt2Topo2BodySimple>()         
      || pev->getTOS<TriggerDecisions::Hlt2Topo3BodySimple>()         
      || pev->getTOS<TriggerDecisions::Hlt2Topo4BodySimple>()         
      || pev->getTOS<TriggerDecisions::Hlt2Topo2BodyBBDT>()           
      || pev->getTOS<TriggerDecisions::Hlt2Topo3BodyBBDT>()           
      || pev->getTOS<TriggerDecisions::Hlt2Topo4BodyBBDT>()           
      || pev->getTOS<TriggerDecisions::Hlt2TopoMu2BodyBBDT>()         
      || pev->getTOS<TriggerDecisions::Hlt2TopoMu3BodyBBDT>()         
      || pev->getTOS<TriggerDecisions::Hlt2TopoMu4BodyBBDT>()         
      || pev->getTOS<TriggerDecisions::Hlt2TopoE2BodyBBDT>()          
      || pev->getTOS<TriggerDecisions::Hlt2TopoE3BodyBBDT>()          
      || pev->getTOS<TriggerDecisions::Hlt2TopoE4BodyBBDT>()          
      || pev->getTOS<TriggerDecisions::Hlt2IncPhi>()                  
      || pev->getTOS<TriggerDecisions::Hlt2IncPhiSidebands>()         
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HHKsLL>()       
      || pev->getTOS<TriggerDecisions::Hlt2Dst2PiD02PiPi>()           
      || pev->getTOS<TriggerDecisions::Hlt2Dst2PiD02MuMu>()           
      || pev->getTOS<TriggerDecisions::Hlt2Dst2PiD02KMu>()            
      || pev->getTOS<TriggerDecisions::Hlt2Dst2PiD02KPi>()            
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02KK>()          
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02KKWideMass>()  
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02KPi>()         
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02KPiWideMass>() 
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02PiPi>()        
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02PiPiWideMass>()  
      || pev->getTOS<TriggerDecisions::Hlt2Bs2PhiGamma>()      
      || pev->getTOS<TriggerDecisions::Hlt2Bs2PhiGammaWideBMass>()
      || pev->getTOS<TriggerDecisions::Hlt2Bd2KstGamma>()         
      || pev->getTOS<TriggerDecisions::Hlt2Bd2KstGammaWideKMass>() 
      || pev->getTOS<TriggerDecisions::Hlt2Bd2KstGammaWideBMass>() 
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD2KS0Pi>()      
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD2KS0K>()       
      || pev->getTOS<TriggerDecisions::Hlt2CharmRareDecayD02MuMu>()
      || pev->getTOS<TriggerDecisions::Hlt2B2HH>()                 
      || pev->getTOS<TriggerDecisions::Hlt2SingleMuon>()     
      || pev->getTOS<TriggerDecisions::Hlt2SingleMuonHighPT>() 
      || pev->getTOS<TriggerDecisions::Hlt2SingleMuonLowPT>()  
      || pev->getTOS<TriggerDecisions::Hlt2TFBc2JpsiMuX>()      
      || pev->getTOS<TriggerDecisions::Hlt2TFBc2JpsiMuXSignal>()
      || pev->getTOS<TriggerDecisions::Hlt2B2HHPi0_Merged>()      
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD2HHH>()       
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD2HHHWideMass>()
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonJPsi>()          
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonJPsiHighPT>()    
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonPsi2S>()       
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonB>()          
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonZ>()          
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonDY1>()        
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonDY2>()        
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonDY3>()        
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonDY4>()        
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonDetached>()   
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonDetachedHeavy>()
      || pev->getTOS<TriggerDecisions::Hlt2DiMuonDetachedJPsi>() 
      || pev->getTOS<TriggerDecisions::Hlt2TriMuonDetached>()   
      || pev->getTOS<TriggerDecisions::Hlt2TriMuonTau>()        
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HHHH>()   
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HHHHWideMass>()
      || pev->getTOS<TriggerDecisions::Hlt2B2HHLTUnbiased>()         
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPi>()  
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HH_D02PiPiWideMass>()
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HH_D02KK>()          
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HH_D02KKWideMass>()  
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HH_D02KPi>()         
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD02HH_D02KPiWideMass>() 
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0Pi>()       
      || pev->getTOS<TriggerDecisions::Hlt2CharmHadD2KS0H_D2KS0K>()        
      || pev->getTOS<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02KMuNuWS>() 
      || pev->getTOS<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02PiMuNuWS>()
      || pev->getTOS<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02KMuNu>()   
      || pev->getTOS<TriggerDecisions::Hlt2CharmSemilepD02HMuNu_D02PiMuNu>()  
      || pev->getTOS<TriggerDecisions::Hlt2CharmSemilepD2HMuMu>()              
      || pev->getTOS<TriggerDecisions::Hlt2CharmSemilepD2HMuMuWideMass>()      
      || pev->getTOS<TriggerDecisions::Hlt2CharmSemilepD02HHMuMu>()            
      || pev->getTOS<TriggerDecisions::Hlt2CharmSemilepD02HHMuMuWideMass>()   
      );
}

float ANNghost(const ZooP* p){//, const ZooEv* ) {
  int val = 10100+705;
  if (p->ParticleInfo()->exists(val)) return p->ParticleInfo()->value(val);
  return -100.;
}
float TRACKghost(const ZooP* p) {
  int val = 20004;
  /*std::cout << "oldprob = ";
  if ((p->ParticleInfo()->exists(val)))
    std::cout << p->ParticleInfo()->value(val);
  else
    std::cout << "non existant";
  std::cout  << std::endl;
  val = 10100;
  std::cout << "buffdprob = ";
  if ((p->ParticleInfo()->exists(val)))
    std::cout << p->ParticleInfo()->value(val);
  else
    std::cout << "non existant";
  std::cout  << std::endl;
  std::cout << "newprob = ";*/
  if ((p->TrInfo()))
    return p->TrInfo()->ghostProbability();
  else
    std::cout << "non existant ghost prob, try workaround";
  val = 20004;
  if (p->ParticleInfo()->exists(val) && 100.>p->ParticleInfo()->value(val)) return p->ParticleInfo()->value(val);
  val = 10100;
  if (p->ParticleInfo()->exists(val) && 100.>p->ParticleInfo()->value(val)) return p->ParticleInfo()->value(val);
  return -100.;
}
float ANNpion(const ZooP* p){//, const ZooEv* ){
  int val = 10100+702;
  if (p->ParticleInfo()->exists(val)) return p->ParticleInfo()->value(val);
  return -100.;
}
float ANNmuon(const ZooP* p){//, const ZooEv* ){
  int val = 10100+701;
  if (p->ParticleInfo()->exists(val)) return p->ParticleInfo()->value(val);
  return -100.;
}
float ANNkaon(const ZooP* p){//, const ZooEv* ){
  int val = 10100+703;
  if (p->ParticleInfo()->exists(val)) return p->ParticleInfo()->value(val);
  return -100.;
}
float ANNproton(const ZooP* p){//, const ZooEv* ){
  int val = 10100+704;
  if (p->ParticleInfo()->exists(val)) return p->ParticleInfo()->value(val);
  return -100.;
}
float ANNelectron(const ZooP* p){//, const ZooEv* ){
  int val = 10100+700;
  if (p->ParticleInfo()->exists(val)) return p->ParticleInfo()->value(val);
  return -100.;
}


float Tchi(const ZooP* p ){//, const ZooEv*) {
  int val = 19;
  if (p->TrExtraInfo() && p->TrExtraInfo()->exists(val))
    return (p->TrExtraInfo()->value(val))/(p->TrExtraInfo()->value(val+1));
  return -100.;
}
float Vchi(const ZooP* p){//, const ZooEv*){
  int val = 17;
  if (p->TrExtraInfo() && p->TrExtraInfo()->exists(val)) return p->TrExtraInfo()->value(val)/p->TrExtraInfo()->value(val+1);
  return -100.;
}

float Tdof(const ZooP* p){//, const ZooEv*) {
  int val = 19;
  if (p->TrExtraInfo() && p->TrExtraInfo()->exists(val)) return p->TrExtraInfo()->value(val+1);
  return -100.;
}
float Vdof(const ZooP* p){//, const ZooEv*){
  int val = 17;
  if (p->TrExtraInfo() && p->TrExtraInfo()->exists(val)) return p->TrExtraInfo()->value(val+1);
  return -100.;
}

int joncode(const ZooMCP* mcp) {
  if (mcp->NMo()==0) return -1;
  if (mcp->NMo()>0 && 431== abs(mcp->Mo(0)->pid())) {
    const ZooMCP* ds = mcp->Mo(0);
    if (ds->NMo()>0) {
      if (43 == (int)(abs(ds->Mo(0)->pid())/10) && abs(ds->Mo(0)->pid()) != 431)
        return 11;
      if ((int)(abs(ds->Mo(0)->pid())/100)%10 == 5)
        return 12;
    }
    return 10;
  }
  if (abs(mcp->Mo(0)->pid()) == 411) {
    const ZooMCP* dp = mcp->Mo(0);
    if (dp->NMo()==0) return 20;
    if (abs(dp->Mo(0)->pid()) == 413 ||
        abs(dp->Mo(0)->pid()) == 415) 
      return 22;
    if ((int)(abs(dp->Mo(0)->pid())/100)%10 == 5)
      return 23;
    return 20;
  }
  if ((int)(abs(mcp->Mo(0)->pid())/100)%10 == 5)
    return 30;
  if ((int)(abs(mcp->Mo(0)->pid()))/1000 == 5)
    return 40;
  return -999;
}

float expectedTT(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->expectedHitPattern()) return -10.;
  return (p->TrInfo()->expectedHitPattern()->tt()[0]
+p->TrInfo()->expectedHitPattern()->tt()[1]
+p->TrInfo()->expectedHitPattern()->tt()[2]
+p->TrInfo()->expectedHitPattern()->tt()[3]);
}

float observedTT(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->hitPattern()) return -10.;
  return (p->TrInfo()->hitPattern()->tt()[0]
+p->TrInfo()->hitPattern()->tt()[1]
+p->TrInfo()->hitPattern()->tt()[2]
+p->TrInfo()->hitPattern()->tt()[3]);
}

float expectedT(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->expectedHitPattern()) return -10.;
  std::bitset<12> dump = p->TrInfo()->expectedHitPattern()->otMono1();
  dump |= p->TrInfo()->expectedHitPattern()->otMono2();
  dump |= p->TrInfo()->expectedHitPattern()->itAC();
  dump |= p->TrInfo()->expectedHitPattern()->itTopBottom();
  //bool isit = (dump[11])||(dump[8]);
  //isit = isit && ((dump[7])||(dump[4]));
  //isit = isit && ((dump[3])||(dump[0]));
  //isit = isit && ((dump[10])||(dump[9]));
  //isit = isit && ((dump[6])||(dump[5]));
  //isit = isit && ((dump[2])||(dump[1]));
  float ret = 0.;
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  return ret;

}

float expectedV(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->expectedHitPattern()) return -10.;
  std::bitset< 23 > ar = p->TrInfo()->expectedHitPattern()->veloAR();
  std::bitset< 23 > cr = p->TrInfo()->expectedHitPattern()->veloCR();
  std::bitset< 23 > ap = p->TrInfo()->expectedHitPattern()->veloAPhi();
  std::bitset< 23 > cp = p->TrInfo()->expectedHitPattern()->veloCPhi();
  float ret = 0.;
  for (int i = 0 ; i < 23 ; ++i) {
    if (ar[i]) ret+=1.;
    if (ap[i]) ret+=1.;
    if (cr[i]) ret+=1.;
    if (cp[i]) ret+=1.;
  }
  return ret;
}

float observedT(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->hitPattern()) return -10.;
  std::bitset<12> dump = p->TrInfo()->hitPattern()->otMono1();
  dump |= p->TrInfo()->hitPattern()->otMono2();
  dump |= p->TrInfo()->hitPattern()->itAC();
  dump |= p->TrInfo()->hitPattern()->itTopBottom();
  //bool isit = (dump[11])||(dump[8]);
  //isit = isit && ((dump[7])||(dump[4]));
  //isit = isit && ((dump[3])||(dump[0]));
  //isit = isit && ((dump[10])||(dump[9]));
  //isit = isit && ((dump[6])||(dump[5]));
  //isit = isit && ((dump[2])||(dump[1]));
  float ret = 0.;
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  return ret;

}

float observedIT(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->hitPattern()) return -10.;
  std::bitset<12> dump = p->TrInfo()->hitPattern()->itAC();
  float ret = 0.;
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  dump = p->TrInfo()->hitPattern()->itTopBottom();
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  return ret;

}
float observedOT(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->hitPattern()) return -10.;
  std::bitset<12> dump = p->TrInfo()->hitPattern()->otMono1();
  float ret = 0.;
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  dump = p->TrInfo()->hitPattern()->otMono2();
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  return ret;

}

float expectedIT(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->expectedHitPattern()) return -10.;
  std::bitset<12> dump = p->TrInfo()->expectedHitPattern()->itAC();
  float ret = 0.;
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  dump = p->TrInfo()->expectedHitPattern()->itTopBottom();
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  return ret;

}
float expectedOT(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->expectedHitPattern()) return -10.;
  std::bitset<12> dump = p->TrInfo()->expectedHitPattern()->otMono1();
  float ret = 0.;
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  dump = p->TrInfo()->expectedHitPattern()->otMono2();
  for (int i = 0 ; i < 12 ; ++i)
    if (dump[i]) ret+=1.;
  return ret;

}


float observedITids(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  const ZooLHCbIDBlock* obsids = p->TrInfo()->lhcbids();
  std::vector<ZooLHCbID> obsidss = *obsids;
  float ret = 0.;
  for (std::vector<ZooLHCbID>::const_iterator it = obsidss.begin() ; obsidss.end() != it ; ++it) {
    if (it->channeltype() == ZooLHCbID::IT)
      ret+=1.;
  }
  return ret;
}
float observedOTids(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  const ZooLHCbIDBlock* obsids = p->TrInfo()->lhcbids();
  std::vector<ZooLHCbID> obsidss = *obsids;
  float ret = 0.;
  for (std::vector<ZooLHCbID>::const_iterator it = obsidss.begin() ; obsidss.end() != it ; ++it) {
    if (it->channeltype() == ZooLHCbID::OT)
      ret+=1.;
  }
  return ret;
}
float observedTTids(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  const ZooLHCbIDBlock* obsids = p->TrInfo()->lhcbids();
  std::vector<ZooLHCbID> obsidss = *obsids;
  float ret = 0.;
  for (std::vector<ZooLHCbID>::const_iterator it = obsidss.begin() ; obsidss.end() != it ; ++it) {
    if (it->channeltype() == ZooLHCbID::TT)
      ret+=1.;
  }
  return ret;
}
float observedV(const ZooP* p) {
  if (! p->TrInfo()) return -10.;
  if (! p->TrInfo()->hitPattern()) return -10.;
  std::bitset< 23 > ar = p->TrInfo()->hitPattern()->veloAR();
  std::bitset< 23 > cr = p->TrInfo()->hitPattern()->veloCR();
  std::bitset< 23 > ap = p->TrInfo()->hitPattern()->veloAPhi();
  std::bitset< 23 > cp = p->TrInfo()->hitPattern()->veloCPhi();
  float ret = 0.;
  for (int i = 0 ; i < 23 ; ++i) {
    if (ar[i]) ret+=1.;
    if (ap[i]) ret+=1.;
    if (cr[i]) ret+=1.;
    if (cp[i]) ret+=1.;
  }
  return ret;
}


float trchi(const ZooP* p) {
  return p->TrInfo()->chi2();
}
float trdof(const ZooP* p) {
  return p->TrInfo()->ndf();
}
float ghostcat(const ZooP* p) {
  if (NULL == p->GhostCat()) return -10.;
  return p->GhostCat()->ghostCategory();
}
float tracklikelihood(const ZooP* p) {
  int val = 10100-1;
  if (p->ParticleInfo()->exists(val)) return p->ParticleInfo()->value(val);
  return -100;
}

float polarisationangle(const ZooP* p) {
  TVector3 muon;
  muon.SetXYZ(p->Ch(2)->momentum().px(),
              p->Ch(2)->momentum().py(),
              p->Ch(2)->momentum().pz());
  TVector3 tau;
  tau.SetXYZ(p->momentum().px(),
             p->momentum().py(),
             p->momentum().pz());
  double denom = 2*p->momentum().Pt() * p->momentum().M();
  double numer = (tau.Cross(muon)).z();
  return numer/denom;
}


float OThits(const ZooP* , const ZooEv* ev) {
  const ZooOccupancy* oc = ev->occupancy();
  if (NULL==oc) return -99.;
  std::vector< std::vector< std::vector< std::vector< int > > > > occ;
  oc->getOccOT(occ);
  unsigned int iocc = 0;
  for (unsigned int a = 0 ; a < occ.size() ; ++a)
    for (unsigned int b = 0 ; b < occ[a].size() ; ++b)
      for (unsigned int c = 0 ; c < occ[a][b].size() ; ++c)
        for (unsigned int d = 0 ; d < occ[a][b][c].size() ; ++d)
          iocc+= occ[a][b][c][d];
  return iocc;
}

float IThits(const ZooP* , const ZooEv* ev) {
  const ZooOccupancy* oc = ev->occupancy();
  if (NULL==oc) return -99.;
  std::vector< std::vector< std::vector< std::vector< int > > > > occ;
  oc->getOccIT(occ);
  unsigned int iocc = 0;
  for (unsigned int a = 0 ; a < occ.size() ; ++a)
    for (unsigned int b = 0 ; b < occ[a].size() ; ++b)
      for (unsigned int c = 0 ; c < occ[a][b].size() ; ++c)
        for (unsigned int d = 0 ; d < occ[a][b][c].size() ; ++d)
          iocc+= occ[a][b][c][d];
  return iocc;
}

float TThits(const ZooP* , const ZooEv* ev) {
  const ZooOccupancy* oc = ev->occupancy();
  if (NULL==oc) return -99.;
  std::vector< std::vector< std::vector< std::vector< int > > > > occ;
  oc->getOccTT(occ);
  unsigned int iocc = 0;
  for (unsigned int a = 0 ; a < occ.size() ; ++a)
    for (unsigned int b = 0 ; b < occ[a].size() ; ++b)
      for (unsigned int c = 0 ; c < occ[a][b].size() ; ++c)
        for (unsigned int d = 0 ; d < occ[a][b][c].size() ; ++d)
          iocc+= occ[a][b][c][d];
  return iocc;
}

float VELOhits(const ZooP* , const ZooEv* ev) {
  const ZooOccupancy* oc = ev->occupancy();
  if (NULL==oc) return -99.;
  std::vector< std::vector< std::vector< int > > > occ;
  oc->getOccVelo(occ);
  unsigned int iocc = 0;
  for (unsigned int a = 0 ; a < occ.size() ; ++a)
    for (unsigned int b = 0 ; b < occ[a].size() ; ++b)
      for (unsigned int c = 0 ; c < occ[a][b].size() ; ++c)
          iocc+= occ[a][b][c];
  return iocc;
}


float trackhistory(const ZooP* p) {
  return p->TrInfo()->history();
}

float tracktype(const ZooP* p) {
  return p->TrInfo()->type();
}


float OTbaddrifttime(const ZooP* p) {
  return p->ParticleInfo()->value(20002);
}
float OToutliers(const ZooP* p) {
  return p->ParticleInfo()->value(20001);
}
double MClifetime(const ZooMCP* mcp){
  double distance = (mcp->prodVertex()-mcp->decayVertex()).R();
  double mass = mcp->momentum().mass();
  double momentum = mcp->momentum().P();
  const double c=299792458.; 
  return distance*mass/momentum*1.0e6/c;
}


/**
* @brief 
*
* @param p
*
* @return lifetime in ns
*/
float largestMClifetime(const ZooP* p) {
  if (!p->assocMCP()) return -10.;
  if (0==p->assocMCP()->NMo()) return -10.;
  if (!p->assocMCP()->Mo(0)) return -10.;
  const ZooMCP* mo = p->assocMCP()->Mo(0);
  double life = MClifetime(mo);
  while (mo->NMo()>0) {
    mo = mo->Mo(0);
    life = std::max(life,MClifetime(mo));
  }
  return life;
  
}

float X_atT(const ZooP* p) {
  if (NULL==p ||
      NULL==p->TrInfo() ||
      NULL==p->TrInfo()->states() ) return -99999.9;
  const ZooPackedState hcalstate = p->TrInfo()->states()->closestState(7000);
  /// this is x,y,tx,ty,q/p
  ZooPackedState::BaseVector::Vector vec = hcalstate;
  return vec[0];
}

float Y_atT(const ZooP* p) {
  if (NULL==p ||
      NULL==p->TrInfo() ||
      NULL==p->TrInfo()->states()) return -99999.9;
  const ZooPackedState hcalstate = p->TrInfo()->states()->closestState(7000);
  /// this is x,y,tx,ty,q/p
  ZooPackedState::BaseVector::Vector vec = hcalstate;
  return vec[1];
}

float g_smearingscale_qop = 1.30;
float g_smearingscale_slope = 1.00;
float g_smearingscale = 0.90;

void gslcopy(gsl_matrix* in, gsl_matrix* out) {
  for (size_t i = 0 ; i < 3 ; ++i)
    for (size_t j = 0 ; j < 3 ; ++j)
      gsl_matrix_set(out,i,j,gsl_matrix_get(in,i,j));
  return;
}

bool isnullvector(gsl_vector* vec) {
  //std::cout << gsl_vector_get(vec,0) << "\t" <<gsl_vector_get(vec,1) <<gsl_vector_get(vec,2) << std::endl;
  return (0.001>=fabs(gsl_vector_get(vec,0))&&0.001>=fabs(gsl_vector_get(vec,1))&&0.001>=fabs(gsl_vector_get(vec,2)));

}

zooLorentzVector truemomentum(const ZooP* p, const ZooEv*) {
  if (!p->assocMCP())
    return p->momentum();
  return p->assocMCP()->momentum();
}

zooLorentzVector truermomentum(const ZooP* p, const ZooEv*) {
  if (!p->assocMCP())
    return p->momentum();
  zooLorentzVector recvec = p->momentum();
  zooLorentzVector truvec = p->assocMCP()->momentum();
  zooLorentzVector diff = recvec;
  diff -= truvec;
  recvec -= 0.5 * diff;
  return recvec;
}

zooLorentzVector smearedmomentum(const ZooP* p, const ZooEv* ev) {
  static bool filecheck = false;
  static int flow = 0;
  ZooPackedParticleVectorWithError::Matrix mat = p->particle().cov();
  if (!(mat.At(0,0) == mat.At(0,0))) {
    if (UNLIKELY(flow<100)) {
      std::cerr << "smearing error 1 " << p->pid() << std::endl;
      ++flow;
    }
    return p->momentum();
  } else {
    if (UNLIKELY(!filecheck)) {
      std::cerr << "matrix has been written" << std::endl;
      filecheck = true;
    }
  }
  gsl_matrix* mymatrix = gsl_matrix_alloc(3,3);
  gsl_matrix* mymatrixcheck = gsl_matrix_alloc(3,3); //removeme
  // put covariance in gsl matrix (px,py,pz) parametrisation
  for (size_t i = 0 ; i < 3 ; ++i)
    for (size_t j = 0 ; j < 3 ; ++j) {
      gsl_matrix_set(mymatrix,i,j,mat.At(3+i,3+j));
      gsl_matrix_set(mymatrixcheck,i,j,mat.At(3+i,3+j));//removeme
    }

  double pmom[3] = {p->momentum().x(),p->momentum().y(),p->momentum().z()};
  double pp = p->momentum().R();
  double charge = p->charge();

  gsl_matrix* jacobian = gsl_matrix_alloc(3,3);
  gsl_matrix* buffer = gsl_matrix_alloc(3,3);
  gsl_matrix* jacobiancheck = gsl_matrix_alloc(3,3);//removeme
  for (size_t i = 0 ; i < 3 ; ++i) {
    for (size_t j = 0 ; j < 3 ; ++j) {
      if (j<2&&i<2) {
        gsl_matrix_set(jacobian,i,j,(i==j)?1./pmom[2]:0.);
        gsl_matrix_set(jacobiancheck,i,j,(i==j)?1./pmom[2]:0.);//removeme
      } else if (i<2) {
        gsl_matrix_set(jacobian,i,j,-pmom[i]/pmom[2]/pmom[2]);
        gsl_matrix_set(jacobiancheck,i,j,-pmom[i]/pmom[2]/pmom[2]);//removeme
      } else {
        gsl_matrix_set(jacobian,i,j,-charge*pmom[j]/pp/pp/pp);
        gsl_matrix_set(jacobiancheck,i,j,-charge*pmom[j]/pp/pp/pp);//removeme
      }
    }
  }

  // convert to (tx,ty,q/p) parametrisation
  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1., jacobian, mymatrix, 0., buffer);
  gslcopy(buffer,mymatrix);
  gsl_blas_dgemm (CblasNoTrans, CblasTrans, 1., mymatrix, jacobian, 0., buffer);
  gslcopy(buffer,mymatrix);


  // scale errors up according to guesstimate
  for (size_t i = 0 ; i < 3 ; ++i)
    for (size_t j = 0 ; j < 3 ; ++j)
      gsl_matrix_set(mymatrix,i,j,(i==2?g_smearingscale_qop:g_smearingscale_slope)*(j==2?g_smearingscale_qop:g_smearingscale_slope)*gsl_matrix_get(mymatrix,i,j));


  /// transform back




  //GSL inversion doesn't work (numerics???)
  //gsl_permutation * perm = gsl_permutation_alloc (3);
  //int s;
  //gsl_linalg_LU_decomp (jacobian, perm, &s);
  //gsl_linalg_LU_invert (jacobian, perm, jacobian);
  //gsl_permutation_free(perm);

  //hand computed inverse
  gsl_matrix_set(jacobian,0,0,pmom[2]/pp*pmom[2]/pp*pmom[2]+pmom[2]/pp*pmom[1]/pp*pmom[1]);
  gsl_matrix_set(jacobian,1,0,-pmom[0]/pp*pmom[1]/pp*pmom[2]);
  gsl_matrix_set(jacobian,2,0,-pmom[0]*pmom[2]/pp*pmom[2]/pp);
  gsl_matrix_set(jacobian,0,1,-pmom[0]/pp*pmom[1]/pp*pmom[2]);
  gsl_matrix_set(jacobian,1,1,pmom[2]/pp*pmom[2]/pp*pmom[2]+pmom[2]/pp*pmom[0]/pp*pmom[0]);
  gsl_matrix_set(jacobian,2,1,-pmom[1]*pmom[2]/pp*pmom[2]/pp);
  gsl_matrix_set(jacobian,0,2,-charge*pp*pmom[0]);
  gsl_matrix_set(jacobian,1,2,-charge*pp*pmom[1]);
  gsl_matrix_set(jacobian,2,2,-charge*pp*pmom[2]);


  gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1., jacobian, mymatrix, 0., buffer);
  gslcopy(buffer,mymatrix);
  gsl_blas_dgemm (CblasNoTrans, CblasTrans, 1., mymatrix, jacobian, 0., buffer);
  gslcopy(buffer,mymatrix);
  /// @todo: sanity check (scale to 1 and check mymatrix)
  
 // gsl_blas_dgemm (CblasNoTrans, CblasNoTrans, 1., jacobian, jacobiancheck, 0., buffer);//removeme
 // gslcopy(buffer,jacobiancheck);
 // for (size_t i = 0 ; i < 3 ; ++i){//removeme
 //   for (size_t j = 0 ; j < 3 ; ++j)//removeme
 //   {
 //       std::cout <<  gsl_matrix_get(jacobiancheck,i,j) << "\t";//removeme
 //   }
 //   std::cout << std::endl;
 // }
 //   std::cout << std::endl;
 ////removeme
 
  gsl_matrix_free(jacobiancheck);//removeme
  gsl_matrix_free(jacobian);
  gsl_matrix_free(buffer);

//  for (size_t i = 0 ; i < 3 ; ++i){//removeme
//    for (size_t j = 0 ; j < 3 ; ++j)//removeme
//    {
//        std::cout <<  gsl_matrix_get(mymatrixcheck,i,j) << "\t";//removeme
//    }
//    std::cout << std::endl;
//  }
//  for (size_t i = 0 ; i < 3 ; ++i){//removeme
//    for (size_t j = 0 ; j < 3 ; ++j)//removeme
//    {
//        std::cout <<  gsl_matrix_get(mymatrix,i,j) << "\t";//removeme
//    }
//    std::cout << std::endl;
//  }
//    std::cout << std::endl;
//    std::cout << std::endl;
//    std::cout << std::endl;
//    std::cout << std::endl;



  gsl_matrix_free(mymatrixcheck);//removeme

  if (GSL_EDOM==(gsl_linalg_cholesky_decomp(mymatrix))) {
    gsl_matrix_free(mymatrix);
    std::cerr << "smearing error 2" << std::endl;
    return p->momentum();
  }

  gsl_matrix* Litself = gsl_matrix_alloc(3,3);
  for (size_t i = 0 ; i < 3 ; ++i)
    for (size_t j = 0 ; j < 3 ; ++j)
      gsl_matrix_set(Litself,i,j,(i>=j)?(gsl_matrix_get(mymatrix,i,j)):0.);

  unsigned int pnum = p;
  TRandom* rnd = new TRandom3(ev->eventNumber() + pnum);
  gsl_vector* stdvec = gsl_vector_alloc(3);
  gsl_vector* modvec = gsl_vector_alloc(3);
  for (size_t i = 0 ; i < 3 ; ++i)
    gsl_vector_set(stdvec,i,rnd->Gaus());
  delete rnd;

  if (isnullvector(stdvec)) {std::cerr << "not good" << std::endl;}
  int err = gsl_blas_dgemv( CblasNoTrans,1.,Litself, stdvec, 0., modvec);

  zooLorentzVector retval(p->momentum().x()  + g_smearingscale * gsl_vector_get(modvec,0),
                          p->momentum().y()  + g_smearingscale * gsl_vector_get(modvec,1),
                          p->momentum().z()  + g_smearingscale * gsl_vector_get(modvec,2),
                          p->momentum().E() // dummy -> fix in the next line
                          );

  retval.SetE(sqrt(retval.x()*retval.x()+retval.y()*retval.y()+retval.z()*retval.z()+p->momentum().M()*p->momentum().M()));
  //retval.SetPtEtaPhiM(retval.Pt(),retval.Eta(),retval.Phi(),p->momentum().M());
  gsl_vector_free(stdvec);
  gsl_vector_free(modvec);
  gsl_matrix_free(mymatrix);
  gsl_matrix_free(Litself);


  return  retval;
}

float mumumass1_smeared(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector one = smearedmomentum(p->Ch(0),ev);
  zooLorentzVector three = smearedmomentum(p->Ch(2),ev);
  one+=three;
  return one.M();
}
float mumumass2_smeared(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector two = smearedmomentum(p->Ch(1),ev);
  zooLorentzVector three = smearedmomentum(p->Ch(2),ev);
  two+=three;
  return two.M();
}
float mumumass3_smeared(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector one = smearedmomentum(p->Ch(0),ev);
  zooLorentzVector two = smearedmomentum(p->Ch(1),ev);
  one+=two;
  return one.M();
}


float mumumass1_true(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector one = truemomentum(p->Ch(0),ev);
  zooLorentzVector three = truemomentum(p->Ch(2),ev);
  one+=three;
  return one.M();
}
float mumumass2_true(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector two = truemomentum(p->Ch(1),ev);
  zooLorentzVector three = truemomentum(p->Ch(2),ev);
  two+=three;
  return two.M();
}
float mumumass3_true(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector one = truemomentum(p->Ch(0),ev);
  zooLorentzVector two = truemomentum(p->Ch(1),ev);
  one+=two;
  return one.M();
}
float mumumass1_truer(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector one = truermomentum(p->Ch(0),ev);
  zooLorentzVector three = truermomentum(p->Ch(2),ev);
  one+=three;
  return one.M();
}
float mumumass2_truer(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector two = truermomentum(p->Ch(1),ev);
  zooLorentzVector three = truermomentum(p->Ch(2),ev);
  two+=three;
  return two.M();
}
float mumumass3_truer(const ZooP* p , const ZooEv* ev) {
  zooLorentzVector one = truermomentum(p->Ch(0),ev);
  zooLorentzVector two = truermomentum(p->Ch(1),ev);
  one+=two;
  return one.M();
}


float trimumass(const ZooP* p , const ZooEv* ev) {
  if (abs(p->pid())!=15) return p->momentum().M();
  zooLorentzVector one = smearedmomentum(p->Ch(0),ev);
  zooLorentzVector two = smearedmomentum(p->Ch(1),ev);
  zooLorentzVector three = smearedmomentum(p->Ch(2),ev);
  one+=two;
  one+=three;
  return one.M();
}
      //

float ncandcommonfake(const ZooP* p ) {
  int val = 5;
  if (p->TrExtraInfo() && p->TrExtraInfo()->exists(val)) return p->TrExtraInfo()->value(val+1);
  return -100.;

}


float phiMCmass(const ZooP* p) {
  const ZooMCP* mc1 = (p->Ch(0)->assocMCP());
  const ZooMCP* mc2 = (p->Ch(1)->assocMCP());
  const ZooMCP* mc3 = (p->Ch(2)->assocMCP());
  if (!(mc1 && mc2 && mc3)) return -10.;
  bool comb12(false);
  bool comb13(false);
  bool comb23(false);
  if (mc1->NMo() && mc2->NMo() && mc1->Mo(0)==mc2->Mo(0)) comb12 = true;
  if (mc1->NMo() && mc3->NMo() && mc1->Mo(0)==mc3->Mo(0)) comb13 = true;
  if (mc2->NMo() && mc3->NMo() && mc2->Mo(0)==mc3->Mo(0)) comb23 = true;
  if (comb12 && 333==mc1->Mo(0)->pid()) return mc1->Mo(0)->momentum().M();
  if (comb13 && 333==mc1->Mo(0)->pid()) return mc1->Mo(0)->momentum().M();
  if (comb23 && 333==mc2->Mo(0)->pid()) return mc2->Mo(0)->momentum().M();
  return -12.;
}


float phiMCdaughtersmass(const ZooP* p){
  const ZooMCP* mc1 = (p->Ch(0)->assocMCP());
  const ZooMCP* mc2 = (p->Ch(1)->assocMCP());
  const ZooMCP* mc3 = (p->Ch(2)->assocMCP());
  if (!(mc1 && mc2 && mc3)) return -10.;
  bool comb12(false);
  bool comb13(false);
  bool comb23(false);
  if (mc1->NMo() && mc2->NMo() && mc1->Mo(0)==mc2->Mo(0)) comb12 = true;
  if (mc1->NMo() && mc3->NMo() && mc1->Mo(0)==mc3->Mo(0)) comb13 = true;
  if (mc2->NMo() && mc3->NMo() && mc2->Mo(0)==mc3->Mo(0)) comb23 = true;
  if (comb12 && 333==mc1->Mo(0)->pid()) { 
    zooLorentzVector lor = mc1->momentum() ;
    lor += mc2->momentum() ;
    return lor.M();
  }
  if (comb13 && 333==mc1->Mo(0)->pid()) {
    zooLorentzVector lor = mc1->momentum() ;
    lor += mc3->momentum() ;
    return lor.M();
  }
  if (comb23 && 333==mc2->Mo(0)->pid()) { 
    zooLorentzVector lor = mc2->momentum() ;
    lor += mc3->momentum() ;
    return lor.M();
  }
  return -12.;
}

float muon_opening_angle(const ZooP* p) {
  const ZooP* one;
  const ZooP* two;
  if (13==abs(p->Ch(0)->pid())) {
    one = p->Ch(0);
    two = (13==abs(p->Ch(1)->pid())?p->Ch(1):p->Ch(2));
  } else {
    one = p->Ch(1);
    two = p->Ch(2);
  }
  return ((one->momentum().Vect()).Dot(two->momentum().Vect())) / ((one->momentum().P() * two->momentum().P()));

}


float IPz(const ZooP* recpart) {
  const ZooMCP* particle = recpart->assocMCP();
  if (NULL==particle) return -9999;
  zooLorentzVector p = particle->momentum();

  double prodx = particle->prodVertex().X();
  double prody = particle->prodVertex().Y();
  double ipz = std::abs(1/p.Pt()*(prodx*p.Py()-prody*p.Px() ));
  return ipz;
}

float fcharge(const ZooP* p) {
  return (float)(int)(p->charge());
}

float strippingtrackchi2(const ZooP* p) {
  return p->TrExtraInfo()->value(20003);
}

std::vector<const ZooP*> stables(const ZooP* p) {
  std::vector<const ZooP*> v_stables;
  v_stables.clear();
  std::stack<const ZooP*> buffer;
  if (!buffer.empty())
    throw 44;
  const ZooP* poi = p;
  if (p->Info<ZooStable>())
    v_stables.push_back(p);
  else {
    buffer.push(p);
    while(!buffer.empty()) {
      poi = buffer.top();
      buffer.pop();
      for (unsigned int d = 0 ; d < poi->NCh() ; ++d) {
        if (poi->Ch(d)->Info<ZooStable>())
          v_stables.push_back(poi->Ch(d));
        else
          buffer.push(poi->Ch(d));
      }
    }
  }
  return v_stables;
}

void push_back_muon(const ZooMCP* mo , std::vector<const ZooMCP*> &vec) {
  for (unsigned d = 0 ; d < mo->NCh() ; ++d) {
    if (22==abs(mo->Ch(d)->pid())) continue;
    if (13==abs(mo->Ch(d)->pid())) {
      vec.push_back(mo->Ch(d));
      continue;
    } else {
      push_back_muon(mo->Ch(d),vec);
    }
  }
  return;
}

void push_back_stables(const ZooMCP* mo, std::vector<const ZooMCP*> &vec) {
  for (unsigned d = 0 ; d < mo->NCh() ; ++d) {
    if (22==abs(mo->Ch(d)->pid())) continue;
    if ((13==abs(mo->Ch(d)->pid())) || 
        (11==abs(mo->Ch(d)->pid())) || 
        (211==abs(mo->Ch(d)->pid())) || 
        (321==abs(mo->Ch(d)->pid())) || 
        (2212==abs(mo->Ch(d)->pid())) 
       ) {
      vec.push_back(mo->Ch(d));
    } else {
      push_back_stables(mo->Ch(d),vec);
    }
  }
  return;
}

float peaking(const ZooP* p) {
  if (NULL==p->assocMCP()) return 0.;
  std::vector<const ZooMCP*> recodaus;
  for (unsigned d = 0 ; d < p->NCh() ; ++d) {       
    if (NULL==p->Ch(d)->assocMCP()) return 0.;      
    recodaus.push_back(p->Ch(d)->assocMCP());
  }
  std::vector<const ZooMCP*> decay_products;
  push_back_stables(p->assocMCP(),decay_products);
  if (decay_products.size()!=recodaus.size()) return 0.;

  for (unsigned d = 0 ; d < recodaus.size() ; ++d) {
    bool found = false;
    for (unsigned k = 0 ; k < decay_products.size() ; ++k) {
      if (decay_products[k]==recodaus[d]) found = true;
    }
    if (false==found) return 0.;
  }

  return 1.;
}

///all reconstructed tracks are true muons
///all MC-muons comming directly or indirectly from the Ds (excluding gamma->mu+X) are among the reconstruced tracks
///there are exactly three muonic Ds daughters (excluding gamma->mu+X decays)
///the Ds decays into one muon, one nu_mu, one non-photon (hopefully an eta) and any number of photons (0 allowed)
///
///hence there's no check if the eta has on MC level further non-muonic decay products (there's also no check if the gamma is there)
///
///also I allow for eta excitations by not requireing the eta to be a eta.
///
///if any check is failed i return 0.
///if all checks are passed, the MC pid of what should be the eta is returned (i.e. 221)
///if the supposed eta is a nu_mu or mu, the identification of the supposed eta will fail (too lazy to implement) and 1. is returned.

float trudsetamunu(const ZooP* p) {
  if (NULL==p->assocMCP()) return 0.;
  if (431!=abs(p->assocMCP()->pid())) return 0.;
  std::vector<const ZooMCP*> recodaus;
  for (unsigned d = 0 ; d < p->NCh() ; ++d) {
    if (NULL==p->Ch(d)->assocMCP()) return 0.;
    if (13!=abs(p->Ch(d)->assocMCP()->pid())) return 0.;
    recodaus.push_back(p->Ch(d)->assocMCP());
  }
  if (3!=recodaus.size()) return 0.;
  std::vector<const ZooMCP*> muonic_decay_products;
  push_back_muon(p->assocMCP(),muonic_decay_products);
  if (3!=muonic_decay_products.size()) return 0.;
  for (unsigned i = 0 ; i < muonic_decay_products.size() ; ++i) {
    if (recodaus.end() == std::find(recodaus.begin(),recodaus.end(),muonic_decay_products[i])) return 0.;
    /// if all muonic Ds daughters are among the reconstructed muons, nothing is returned and the function goes on
  }
  std::vector<int> immediatedaus;
  for (unsigned d = 0 ; d < p->assocMCP()->NCh() ; ++d) {
    if (22==p->assocMCP()->Ch(d)->pid()) continue; // I don't like photons
    immediatedaus.push_back(p->assocMCP()->Ch(d)->pid());
  }
  if ((immediatedaus.end() == std::find(immediatedaus.begin(),immediatedaus.end(),13)) && (immediatedaus.end() == std::find(immediatedaus.begin(),immediatedaus.end(),-13))) return 0.;
  if ((immediatedaus.end() == std::find(immediatedaus.begin(),immediatedaus.end(),14)) && (immediatedaus.end() == std::find(immediatedaus.begin(),immediatedaus.end(),-14))) return 0.;
  // among the non-photonic decay products, there must be at least one muon, one nu_mu, and one eta
 
  if (3!=immediatedaus.size()) return 0.;
  // don't allow for non-photonic decay products more than one eta, one mu, one nu_mu
  
  // return PID of supposed eta
  for (unsigned d = 0 ; d < immediatedaus.size() ; ++d) {
    if (13==abs(immediatedaus[d])) continue;
    if (14==abs(immediatedaus[d])) continue;
    return (float)(int)immediatedaus[d];
  }
  // NB: no check of eta decay products performed
  return 1.;
}

float trudsphipi(const ZooP* p) {
  if (NULL==p->assocMCP()) return 0.;
  if (431!=abs(p->assocMCP()->pid())) return 0.;
  if (p->assocMCP()->NCh()!=2) return 0.;
  if ((333==abs(p->assocMCP()->Ch(0)->pid())) == (333==abs(p->assocMCP()->Ch(1)->pid()))) return 0.; // check if exaclty one phi daughter
  if ((211==abs(p->assocMCP()->Ch(0)->pid())) == (211==abs(p->assocMCP()->Ch(1)->pid()))) return 0.; // check if exaclty one pi daughter
  const ZooMCP* phi = ((333==abs(p->assocMCP()->Ch(0)->pid()))?(p->assocMCP()->Ch(0)):(p->assocMCP()->Ch(1)));
  const ZooMCP* pi = ((333==abs(p->assocMCP()->Ch(0)->pid()))?(p->assocMCP()->Ch(1)):(p->assocMCP()->Ch(0)));
  std::vector<const ZooMCP*> muonic_decay_products;
  for (unsigned d = 0 ; d < phi->NCh() ; ++d) {
    if (13==abs(phi->Ch(d)->pid())) muonic_decay_products.push_back(phi->Ch(d));
  }
  if (2!=muonic_decay_products.size()) return 0.;
  if (p->Ch(0)->assocMCP() != pi) return 0.;
  if ((p->Ch(1)->assocMCP() != muonic_decay_products[0]) && (p->Ch(1)->assocMCP() != muonic_decay_products[1])) return 0.;
  if ((p->Ch(2)->assocMCP() != muonic_decay_products[0]) && (p->Ch(2)->assocMCP() != muonic_decay_products[1])) return 0.;
  return 333.;
}

int n_stables(const ZooP* p) {
  return stables(p).size();
}
TRandom* get_TRandom() {
  return new TRandom3();
}

TRandom* g_rand = get_TRandom();

template <class kk>
float gammaVLLLL(const kk* p) {
  if (15!=trutau(p)) return -9.;
  /// following the notation of 0707.0988
  // in ntuple daughters 0 and 1 have the same sign
  float m12 = combine(p->Ch(0)->momentum(),p->Ch(1)->momentum());
  //float m23;
  //TRandom* rnd = new TRandom3();
  //if (rnd->Rndm()>0.5) {
  //  m23 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
  //} else {
  //  m23 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
  //}
  double denom = p->momentum().M();
  double mumass = p->Ch(0)->momentum().M();
  double taumass = p->momentum().M();
  denom = denom*denom*denom;
  double num = m12*m12*(taumass*taumass + (mumass*mumass - m12*m12  )) -  2* (mumass*mumass * (taumass*taumass + mumass*mumass) );
  return num/denom;
}

template float gammaVLLLL<ZooP>(const ZooP* p);
template float gammaVLLLL<ZooMCP>(const ZooMCP* p);

template <class kk>
float gammaVLLRR(const kk* p) {
  if (15!=trutau(p)) return -9.;
  /// following the notation of 0707.0988
  // in ntuple daughters 0 and 1 have the same sign
  float m12 = combine(p->Ch(0)->momentum(),p->Ch(1)->momentum());
  float m23, m13;
  if (g_rand->Rndm()>0.5) {
    m23 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
    m13 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
  } else {
    m23 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
    m13 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
  }
  double denom = p->momentum().M();
  double mumass = p->Ch(0)->momentum().M();
  double taumass = p->momentum().M();
  denom = denom*denom*denom;
  double num = -24.*mumass*mumass*(taumass*taumass+mumass*mumass - m12*m12/3.) - 4.*(m13*m13*m13*m13+m23*m23*m23*m23) + 4.*(taumass*taumass+3.*mumass*mumass) * (m13*m13+m23*m23) ; 
  return num/denom;
}

template float gammaVLLRR<ZooP>(const ZooP* p);
template float gammaVLLRR<ZooMCP>(const ZooMCP* p);

template <class kk>
float gammaRAD(const kk* p) {
  if (15!=trutau(p)) return -9.;
  /// following the notation of 0707.0988
  // in ntuple daughters 0 and 1 have the same sign
  float m12 = combine(p->Ch(0)->momentum(),p->Ch(1)->momentum());
  float m23, m13;
  if (g_rand->Rndm()>0.5) {
    m23 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
    m13 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
  } else {
    m23 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
    m13 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
  }
  double denom = p->momentum().M();
  double mumass = p->Ch(0)->momentum().M();
  double taumass = p->momentum().M();
  denom = denom*denom*denom;
  double a = taumass*taumass;
  double b = mumass*mumass;
  double c = m13*m13;
  double d = m23*m23;
  double e = m12*m12;
  denom = denom*c*d*c*d;
    //mathematica output
  double num = 
    -6*b*c*c*d*d - 6*b*b*c*d*(c + d) + 2*b*b*b*(c + d)*(c + d) + 
     2*a*a*b*(c*c + c*d + d*d) - 
      2*a*b*(3*c*d*(c + d) + b*(2*c*c + 3*c*d + 2*d*d)) + 
       c*d*(c*c*c + c*c*d + c*d*d + d*d*d + 4*c*d*e + (c + d)*e*e);
  return num/denom;
}

template float gammaRAD<ZooP>(const ZooP* p);
template float gammaRAD<ZooMCP>(const ZooMCP* p);

template <class kk>
float interferenceVLLLL_RAD(const kk* p) {
  if (15!=trutau(p)) return -9.;
  /// following the notation of 0707.0988
  // in ntuple daughters 0 and 1 have the same sign
  float m12 = combine(p->Ch(0)->momentum(),p->Ch(1)->momentum());
  float m23, m13;
  if (g_rand->Rndm()>0.5) {
    m23 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
    m13 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
  } else {
    m23 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
    m13 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
  }
  double denom = p->momentum().M();
  double mumass = p->Ch(0)->momentum().M();
  double taumass = p->momentum().M();
  denom = denom*denom*denom;
  double a = taumass*taumass;
  double b = mumass*mumass;
  double c = m13*m13;
  double d = m23*m23;
  double e = m12*m12;
  denom = denom*c*d;
  double num = a*b*(c+d) - b*(6*c*d+b*(c+d)) + 2*c*d*e;
  return num/denom;
}
template float interferenceVLLLL_RAD<ZooP>(const ZooP* p);
template float interferenceVLLLL_RAD<ZooMCP>(const ZooMCP* p);

template <class kk>
float interferenceVLLRR_RAD(const kk* p) {
  if (15!=trutau(p)) return -9.;
  /// following the notation of 0707.0988
  // in ntuple daughters 0 and 1 have the same sign
  float m12 = combine(p->Ch(0)->momentum(),p->Ch(1)->momentum());
  float m23, m13;
  if (g_rand->Rndm()>0.5) {
    m23 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
    m13 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
  } else {
    m23 = combine(p->Ch(1)->momentum(),p->Ch(2)->momentum());
    m13 = combine(p->Ch(0)->momentum(),p->Ch(2)->momentum());
  }
  double denom = p->momentum().M();
  double mumass = p->Ch(0)->momentum().M();
  double taumass = p->momentum().M();
  denom = denom*denom*denom;
  double a = taumass*taumass;
  double b = mumass*mumass;
  double c = m13*m13;
  double d = m23*m23;
  double e = m12*m12;
  denom = denom*c*d;
  double num = a*b*c + a*(b+c)*d - b*(3*c*d+b*(c+d)) - c*d*e;
  return num/denom;
}
template float interferenceVLLRR_RAD<ZooP>(const ZooP* p);
template float interferenceVLLRR_RAD<ZooMCP>(const ZooMCP* p);

template <class kk>
float norm_gammaVLLLL(const kk* p) {
  if (15!=trutau(p)) return -9.;
  return gammaVLLLL(p) * 0.00328524;
}
template float norm_gammaVLLLL<ZooP>(const ZooP* p) ;

template <class kk>
float norm_gammaVLLRR(const kk* p) {
  if (15!=trutau(p)) return -9.;
  return gammaVLLRR(p) * 0.000410582;
}
template float norm_gammaVLLRR<ZooP>(const ZooP* p) ;

template <class kk>
float norm_gammaRAD(const kk* p) {
  if (15!=trutau(p)) return -9.;
  return gammaRAD(p) *201.947;
}
template float norm_gammaRAD<ZooP>(const ZooP* p) ;

template <class kk>
float norm_interferenceVLLLL_RAD(const kk* p) {
  if (15!=trutau(p)) return -9.;
  return interferenceVLLLL_RAD(p) * 2555.49;
}
template float norm_interferenceVLLLL_RAD<ZooP>(const ZooP* p) ;

template <class kk>
float norm_interferenceVLLRR_RAD(const kk* p) {
  if (15!=trutau(p)) return -9.;
  return interferenceVLLRR_RAD(p)* 2555.41;
}
template float norm_interferenceVLLRR_RAD<ZooP>(const ZooP* p) ;



float norm_mc_gammaVLLLL(const ZooP* p) {
  if (15!=trutau(p)) return -9.;
  if (!p->assocMCP()) return -9999;
  return norm_gammaVLLLL(p->assocMCP());
}
float norm_mc_gammaVLLRR(const ZooP* p) {
  if (15!=trutau(p)) return -9.;
  if (!p->assocMCP()) return -9999;
  return norm_gammaVLLRR(p->assocMCP());
}
float norm_mc_gammaRAD(const ZooP* p) {
  if (15!=trutau(p)) return -9.;
  if (!p->assocMCP()) return -9999;
  return norm_gammaRAD(p->assocMCP());
}
float norm_mc_interferenceVLLLL_RAD(const ZooP* p) {
  if (15!=trutau(p)) return -9.;
  if (!p->assocMCP()) return -9999;
  return norm_interferenceVLLLL_RAD(p->assocMCP());
}
float norm_mc_interferenceVLLRR_RAD(const ZooP* p) {
  if (15!=trutau(p)) return -9.;
  if (!p->assocMCP()) return -9999;
  return norm_interferenceVLLRR_RAD(p->assocMCP());
}




