#include "funs.h"
float flight(const ZooP* particle) {
  return particle->fDistance();
}

float nmcp(const ZooP*, const ZooEv* e) {
  return e->NMCP();
}

float etatest(const ZooP* p) {
  const ZooMCP* resomo = NULL;
  const ZooMCP* dirmo = NULL;
  for (int d = 0 ; d < p->NCh() ; ++d) {
    if (13==abs(p->Ch(d)->pid())) {
      if (p->Ch(d)->assocMCP()) dirmo = p->Ch(d)->assocMCP()->Mo(0);
    } else {
      bool firstdau = (p->Ch(d)->Ch(0)->assocMCP() && abs(p->Ch(d)->Ch(0)->pid())==abs(p->Ch(d)->Ch(0)->assocMCP()->pid()));
      bool seconddau = (p->Ch(d)->Ch(1)->assocMCP() && abs(p->Ch(d)->Ch(1)->pid())==abs(p->Ch(d)->Ch(1)->assocMCP()->pid()));
      bool thirddau = (p->Ch(d)->Ch(2)->assocMCP() && abs(p->Ch(d)->Ch(2)->pid())==abs(p->Ch(d)->Ch(2)->assocMCP()->pid()));
      bool samemo;
      if (thirddau&&firstdau&&seconddau) samemo = (p->Ch(d)->Ch(0)->assocMCP()->Mo(0) == p->Ch(d)->Ch(1)->assocMCP()->Mo(0) && p->Ch(d)->Ch(0)->assocMCP()->Mo(0) == p->Ch(d)->Ch(2)->assocMCP()->Mo(0)) ;
      else samemo = false;
      
      bool frometa;
      if (samemo)
      frometa =  (221==abs(p->Ch(d)->Ch(0)->assocMCP()->Mo(0)->pid()));
      else frometa = false;
      if (frometa) {
        //std::cout << "MOTHER " << p->Ch(d)->Ch(0)->assocMCP()->Mo(0)->Mo(0)->pid() << std::endl;
        resomo = p->Ch(d)->Ch(0)->assocMCP()->Mo(0)->Mo(0);
      }
    }
  }
  if (resomo == dirmo && dirmo != NULL)
        return p->measuredMass();
  return 0.;
}

float andy(const ZooP* zp) {
  if ( ((zp->TrigDec()->getTOS<TriggerDecisions::Hlt1TrackMuon>()))//TOS HLT1
      &&
      (zp->TrigDec()->getTOS<TriggerDecisions::Hlt2SingleMuon>())//TOS HLT2
     ){
    //cout<<"ip stdloosemuonTOS: "<<zp->ip()<<endl;
    return (zp->ip());
  }
  return -5.;
}


float somept(const ZooP* p) {
  int dd = gRandom->Rndm() * p->NCh();
  return (*p)[dd]->momentum().rho();
}

float maxpt(const ZooP* p) {
  float retval = 0;
  for (unsigned k = 0 ; k < p->NCh() ; ++k) {
    retval = std::max(retval,((float)(*p)[k]->momentum().rho()));
  }
  return retval;
}

float mctruth(const ZooP* p) {
  if (p->assocMCP()) return (float) p->assocMCP()->pid();
  return 0.;

}

float minips(const ZooP* p) {
  float retval = 100000;
  for (unsigned k = 0 ; k < p->NCh() ; ++k) {
    retval = std::min(retval,(*p)[k]->ipSig());
  }
  return retval;
}
float mumumass1(const ZooP* p) {
  return combine(p->Ch(0),p->Ch(2));
}

float mumumass2(const ZooP* p) {
  return combine(p->Ch(1),p->Ch(2));
}

float mumumass3(const ZooP* p) {
  return combine(p->Ch(1),p->Ch(0));
}
float trackchi2(const ZooP* p) {
  return p->TrInfo()->chi2() / p->TrInfo()->ndf();
}

float spam(const ZooP*, const ZooEv* pev) {
  if (!(pev->getTriggerDec<TriggerDecisions::Hlt1TrackMuon>()))
    return (float)( pev->getTriggerWord(1) & 0xFF);//printf("%#x\n",pev->getTriggerWord(1));

  return std::numeric_limits<float>::quiet_NaN();
}
float runnumber(const ZooP* , const ZooEv* pev) {return pev->runNumber();}
//float tautrigger(const ZooP*, const ZooEv* pev) {return (float)(int)accept(pev);}


float geo(const ZooP* p) {return p->ParticleInfo()->value(1);}
float pid(const ZooP* p) {return p->ParticleInfo()->value(2);}
/*
   float hlt2triggeredmass(const ZooP* particle, const ZooEv* pev) {
   if (pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuon>()) {return particle->measuredMass();}
   return std::numeric_limits<float>::quiet_NaN(); 
   }
   float hlt2untriggeredmass(const ZooP* particle, const ZooEv* pev) {
   if (! (pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuon>())) {return particle->measuredMass();}
   return std::numeric_limits<float>::quiet_NaN(); 
   }*/
float hlt1untriggeredmass(const ZooP* particle, const ZooEv* pev) {
  if (//(! (pev->getTriggerDec<TriggerDecisions::Hlt1SingleMuonNoIPL0>()))
      (! (pev->getTriggerDec<TriggerDecisions::Hlt1TrackMuon>()))
     ) {return particle->measuredMass();}
  return std::numeric_limits<float>::quiet_NaN(); 
}
float oldstripped(const ZooP* particle, const ZooEv* pev) {
  //  if (pev->getTriggerDec<TriggerDecisions::StrippingVeloMuonJPsiLine>()) return particle->measuredMass();
  //  if (pev->getTriggerDec<TriggerDecisions::StrippingJpsi2MuMuLine>()) return particle->measuredMass();
  return std::numeric_limits<float>::quiet_NaN();
}
float myhit(const ZooP* p) {
  const ZooHitPattern* thehit = p->TrInfo()->expectedHitPattern();
  bool veryfirst = thehit->otMono1()[0];
  return veryfirst?1:0;
}
float assocMCP(const ZooP* p) {
  if (p->assocMCP()) return 1.;
  return 0.;
}

float hasperfektlinks(const ZooP* p) {
  const ZooLinks* links = p->Links();
  if (NULL==links) {std::cout << " no links " << std::endl;return 0;}
  if (0==links->NLinks()) std::cout << " zero links " << std::endl;
  int counter = 0;
  const ZooP* tolinked = NULL;
  for (int i = 0 ; i<links->NLinks();++i) {
    std::cout << links->Frac(i) << std::endl;
    if (links->Frac(i) >0.999999) tolinked=links->Link(i);
  }
  if (tolinked) return tolinked->TrInfo()->chi2() / tolinked->TrInfo()->ndf();
  return (float)0.;
}

float printmothers(const ZooP* p) {
  int retval=0;
  if (p->assocMCP()) {
    const ZooMCP* part = p->assocMCP();
    for (int i = 0 ; i < 100 ; ++i) {
      std::cout << part->pid() << "\t";
      if (1==i) retval = part->pid();
      std::cout << "(" << part->NMo() << ")";
      if (part->NMo()>1) std::cout << "WTF";
      if (0==part->NMo()) {i+=1000;}
      else {part = part->Mo(0);}
    } 
    std::cout << std::endl;
  }
  return (float)(retval);
}
float topofthepile(const ZooP* p) {
  int retval=0;
  if (p->assocMCP()) {
    const ZooMCP* part = p->assocMCP();
    for (int i = 0 ; i < 100 ; ++i) {
      std::cout << part->pid() << "\t";
      //if (1==i) retval = part->pid();
      std::cout << "(" << part->NMo() << ")";
      if (part->NMo()>1) std::cout << "WTF";
      if (0==part->NMo()) {
        retval = part->pid();
        i+=1000;
      }
      else {part = part->Mo(0);}
    } 
    std::cout << std::endl;
  }
  return (float)(retval);
}
/*
   float nottriggeredmass(const ZooP* particle, const ZooEv* pev) {
   if (pev->getTriggerDec<TriggerDecisions::Hlt2SingleMuon>()) return std::numeric_limits<float>::quiet_NaN();
   if (pev->getTriggerDec<TriggerDecisions::Hlt2SingleHighPTMuon>()) return std::numeric_limits<float>::quiet_NaN();
   if (pev->getTriggerWord(5) & 0x07ff0000) return std::numeric_limits<float>::quiet_NaN();
   if (pev->getTriggerDec<TriggerDecisions::Hlt1SingleMuonNoIPL0>()) {
   for (int w = 5 ; w <= 8 ; ++w)
   for (int s = 0 ; s <= 8 ; ++s) {
   if ( 5 == w && s == 0) continue;
   if (pev->getTriggerWord(w) & 1<<s) printf("%d\t0x%08x\n",w,1<<s);
   }
   return particle->measuredMass();}
   else return std::numeric_limits<float>::quiet_NaN();
   }*/

float prompttck(const ZooP*, const ZooEv* pev) {
  printf("%x\n", pev->TCK());
  return (float)(int)pev->TCK();
  //return std::numeric_limits<float>::quiet_NaN();
}

float hasvmdaughter(const ZooP* particle) {
  const ZooP* ch1 = (*particle)[0];
  const ZooP* ch2 = (*particle)[1];
  if (ch1->TrExtraInfo()->exists(4040))
    return 1.+ch1->TrExtraInfo()->value(4040);
  if (ch2->TrExtraInfo()->exists(4040) )
    return 1.+ch2->TrExtraInfo()->value(4040);
  else
    return 0.;
}
/*
float expectedTT(const ZooP* p){
  if (! p->TrInfo()) return 0;
  if (! p->TrInfo()->expectedHitPattern()) return 0;
  return (p->TrInfo()->expectedHitPattern()->tt()[0]
      +p->TrInfo()->expectedHitPattern()->tt()[1]
      +p->TrInfo()->expectedHitPattern()->tt()[2]
      +p->TrInfo()->expectedHitPattern()->tt()[3]);
}
float expectedT(const ZooP* p){
  std::bitset<12> dump = p->TrInfo()->expectedHitPattern()->otMono1();
  dump |= p->TrInfo()->expectedHitPattern()->otMono2();
  dump |= p->TrInfo()->expectedHitPattern()->itAC();
  dump |= p->TrInfo()->expectedHitPattern()->itTopBottom();
  bool isit = (dump[11])||(dump[8]);
  isit = isit && ((dump[7])||(dump[4]));
  isit = isit && ((dump[3])||(dump[0]));
  isit = isit && ((dump[10])||(dump[9]));
  isit = isit && ((dump[6])||(dump[5]));
  isit = isit && ((dump[2])||(dump[1]));
  if (isit) return 4.;
  else return 0.;
  return -1.;
  //  if (! p->TrInfo()) return 0;
  //  if (! p->TrInfo()->expectedHitPattern()) return 0;
  //  int k=0;
  //  for (int i = 0 ; i < 12 ; ++i) {
  //    k+=p->TrInfo()->expectedHitPattern()->otMono1()[i];
  //    k+=p->TrInfo()->expectedHitPattern()->otMono2()[i];
  //    k+=p->TrInfo()->expectedHitPattern()->itAC()[i];
  //    k+=p->TrInfo()->expectedHitPattern()->itTopBottom()[i];
  //  //  if (k>0) return k;
  //  }
  //  return k;
}


*/

