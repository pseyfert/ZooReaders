#ifndef __ZOOF_H
#define __ZOOF_H

/**
 * @class ZooFunctor ZooFunctor.h
 * Wrapper to allow <del>easy</del> change-one-line-only inclusion of new functions to ZooSteamRoller and ZooGui
 *
 * @author Paul Seyfert
 * @date   2011-10-28
 */


#include "../Zoo/Zoo.h"
#include <Rtypes.h>
#include <string>
#include <iostream>
#include <limits>
#include "likely.h"

class Foo
{
  public:
    std::string bar(int number)
    {
      std::string tensNames[] = {
        "",
        "ten",
        "twenty",
        "thirty",
        "forty",
        "fifty",
        "sixty",
        "seventy",
        "eighty",
        "ninety"
      };

      std::string numNames[] = {
        "",
        "p1_",
        "p2_",
        "p3_",
        "p4_",
        "p5_",
        "p6_",
        "seventh",
        "eight",
        "nine",
        "ten",
        "eleven",
        "twelve",
        "thirteen",
        "fourteen",
        "fifteen",
        "sixteen",
        "seventeen",
        "eighteen",
        "nineteen"
      };
      if (0 == number) return std::string("p0_");
      std::string soFar;

      if (number % 100 < 20)
      {
        soFar = numNames[number % 100];
        number /= 100;
      }
      else
      {
        soFar = numNames[number % 10];
        number /= 10;

        soFar = tensNames[number % 10] + soFar;
        number /= 10;
      }
      if (number == 0) return soFar;
      return numNames[number] + "hundred" + soFar;
    }
};


class ZooFunctor {
  public:
    typedef float (ZooP::*func)()const;
    typedef int (ZooP::*particleintfunc)()const;
    typedef float (ZooEv::*eunc)()const;
    typedef int (ZooEv::*iunc)()const;
    typedef unsigned int (ZooEv::*uiunc)()const;
    typedef float (*gfunc)(const ZooP*);//const;
    typedef int (*ifunc)(const ZooP*);
    typedef float (*efunc)(const ZooP*, const ZooEv*);
    typedef int (*eifunc)(const ZooP*, const ZooEv*);
  protected:
    union numbers {unsigned int u; float f; int i;} m_lastresult;
  public:
    virtual void* lastresult() {return &m_lastresult;}
    std::string name;
    virtual std::string name_with_type() {return std::string(Form("%s/F",name.c_str()));}
    bool success;
    virtual void evaluate(const ZooP* p, const ZooEv* e) {m_lastresult.f = eval(p,e);return;};
    virtual float eval(const ZooP*) {std::cout << "Functor default" << std::endl;return std::numeric_limits<float>::quiet_NaN();};
    virtual float eval(const ZooP* part, const ZooEv*) {return eval(part);};
  public:
    virtual ~ZooFunctor();
    ZooFunctor() {}
    ClassDef(ZooFunctor,1);
};

class ZooNPV : public ZooFunctor {
  public:
    ZooNPV() {name = std::string("NumberPV");}
    float eval(const ZooP*, const ZooEv* ev) {
      return ev->pvs().size();
    }
    ClassDef(ZooNPV,1);
};

class ZooLeg : public ZooFunctor {
  private:
    ZooFunctor* method;
    const ZooP* vmleg (const ZooP* mother) {
      const ZooP* thevmleg = NULL;
      const ZooP* ch1 = (*mother)[0];
      const ZooP* ch2 = (*mother)[1];
      if (ch1->TrInfo() && 0==ch1->TrInfo()->history())
        thevmleg = ch1;
      if (ch2->TrInfo() && 0==ch2->TrInfo()->history()) {
        if (thevmleg) std::cout << "watch out!" << std::endl;
        thevmleg = ch2;
      }
      if (NULL == thevmleg) std::cout << "VM leg not found" << std::endl;
      return thevmleg;
    }
  public:
    /// constructor. Attention: ZooLeg instance takes ownership of »usethis«
    ZooLeg(ZooFunctor* usethis) {name = std::string(Form("leg%s",usethis->name.c_str()));method = usethis;}
    virtual ~ZooLeg() {delete method;}
    float eval(const ZooP* part) {
      return method->eval(vmleg(part));
    }
    ClassDef(ZooLeg,1);
};

class ZooBuChild : public ZooFunctor {
  private:
    ZooFunctor* method;
    unsigned int  m_leg;
    Foo bar;
    const ZooP* Ch0(const ZooP* p) {
      if (443==p->Ch(0)->pid()) {
        return p->Ch(0)->Ch(0);
      } else {
        return p->Ch(0);
      }
    }
    const ZooP* Ch1(const ZooP* p) {
      if (443==p->Ch(0)->pid()) {
        return p->Ch(0)->Ch(1);
      } else {
        return p->Ch(1)->Ch(0);
      }
    }
    const ZooP* Ch2(const ZooP* p) {
      if (443==p->Ch(0)->pid()) {
        return p->Ch(1);
      } else {
        return p->Ch(1)->Ch(1);
      }
    }

  public:
    ZooBuChild() {method = NULL;}
    virtual ~ZooBuChild() {delete method;}
    ZooBuChild(ZooFunctor* meth, int leg) {m_leg = leg; method = meth; name = std::string(Form("%s%s",bar.bar(leg).c_str(),meth->name.c_str()));}
    virtual void* lastresult() {return method->lastresult();};
    virtual float eval(const ZooP* part) ;
    ClassDef(ZooBuChild,1);
};

class ZooChild : public ZooFunctor {
  private:
    ZooFunctor* method;
    unsigned int  m_leg;
    Foo bar;
  public:
    ZooChild() {method = NULL;}
    virtual ~ZooChild() {delete method;}
    ZooChild(ZooFunctor* meth, int leg) {m_leg = leg; method = meth; name = std::string(Form("%s%s",bar.bar(leg).c_str(),meth->name.c_str()));}
    //virtual void* lastresult() {return method->lastresult();};
    virtual float eval(const ZooP* part) ;
    virtual float eval(const ZooP* part, const ZooEv*) ;
    virtual std::string name_with_type() {return std::string(Form("%s%s",bar.bar(m_leg).c_str(),method->name_with_type().c_str()));}
    ClassDef(ZooChild,2);
};

class ZooMinChild : public ZooFunctor {
  private:
    ZooFunctor* method;
    Foo bar;
  public:
    ZooMinChild() {method = NULL;}
    virtual ~ZooMinChild() {delete method;}
    ZooMinChild(ZooFunctor* meth) {method = meth; name = std::string(Form("min_%s",meth->name.c_str()));}
    //virtual void* lastresult() {return method->lastresult();};
    virtual float eval(const ZooP* part) ;
    virtual float eval(const ZooP* part, const ZooEv*) ;
    virtual std::string name_with_type() {return std::string(Form("min_%s",method->name_with_type().c_str()));}
    ClassDef(ZooMinChild,1);
};

class ZooMaxChild : public ZooFunctor {
  private:
    ZooFunctor* method;
    Foo bar;
  public:
    ZooMaxChild() {method = NULL;}
    virtual ~ZooMaxChild() {delete method;}
    ZooMaxChild(ZooFunctor* meth) {method = meth; name = std::string(Form("max_%s",meth->name.c_str()));}
    //virtual void* lastresult() {return method->lastresult();};
    virtual float eval(const ZooP* part) ;
    virtual float eval(const ZooP* part, const ZooEv*) ;
    virtual std::string name_with_type() {return std::string(Form("max_%s",method->name_with_type().c_str()));}
    ClassDef(ZooMaxChild,1);
};




class ZooNChild : public ZooFunctor {
  private:
    ZooFunctor* method;
    unsigned int  m_leg;
    Foo bar;
  public:
    ZooNChild() {method = NULL;}
    virtual ~ZooNChild() {delete method;}
    ZooNChild(ZooFunctor* meth, int leg) {m_leg = leg; method = meth; name = std::string(Form("%s",meth->name.c_str()));}
    virtual void* lastresult() {return method->lastresult();};
    virtual std::string name_with_type() { return method->name_with_type();}
    virtual float eval(const ZooP* part) ;
    virtual float eval(const ZooP* p, const ZooEv* ev);
    ClassDef(ZooNChild,1);
};


class ZooPtChild : public ZooFunctor {
  private:
    ZooFunctor* method;
    unsigned int  m_leg;
    Foo bar;
  public:
    ZooPtChild() {method = NULL;}
    virtual ~ZooPtChild() {delete method;}
    ZooPtChild(ZooFunctor* meth, int leg) {m_leg = leg; method = meth; name = std::string(Form("ptordered%s%s",bar.bar(leg).c_str(),meth->name.c_str()));}
    virtual void* lastresult() {return method->lastresult();};
    float eval(const ZooP* part) ;
    ClassDef(ZooPtChild,1);
};

class ZooJPsi : public ZooFunctor {
  private:
    ZooFunctor* method;
  public:
    ZooJPsi() {method = NULL;}
    ZooJPsi(ZooFunctor* meth) {method = meth; name = std::string(Form("JPsi_%s",meth->name.c_str()));}
    virtual float eval(const ZooP* part);
    ClassDef(ZooJPsi,1);
};

class ZooOLeg : public ZooFunctor {
  private:
    ZooFunctor* method;
    const ZooP* vmleg (const ZooP* mother) {
      const ZooP* thevmleg = NULL;
      const ZooP* ch1 = (*mother)[0];
      const ZooP* ch2 = (*mother)[1];
      if (ch1->TrInfo() && 0==ch1->TrInfo()->history())
        thevmleg = ch1;
      if (ch2->TrInfo() && 0==ch2->TrInfo()->history()) {
        if (thevmleg) std::cout << "watch out!" << std::endl;
        thevmleg = ch2;
      }
      if (thevmleg == ch1) return ch2;
      return ch1;
    }
  public:
    /// constructor. Attention: ZooLeg instance takes ownership of »usethis«
    ZooOLeg(ZooFunctor* usethis) {name = std::string(Form("longleg%s",usethis->name.c_str()));method = usethis;}
    virtual ~ZooOLeg() {delete method;}
    float eval(const ZooP* part) {
      return method->eval(vmleg(part));
    }
    ClassDef(ZooOLeg,1);
};


class Zoo4448 : public ZooFunctor {
  public:
    Zoo4448() {name = std::string("extrainfo4448");}
    float eval(const ZooP* p) {
      if ((p->TrExtraInfo() && p->TrExtraInfo()->exists(4448)))
        return p->TrExtraInfo()->value(4448);
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(Zoo4448,1);
};

class ZooInfoFunctor : public ZooFunctor {
  private:
    int m_num;
  public:
    ZooInfoFunctor(int num) {name = std::string(Form("Info%d",num)); m_num = num;}
    float eval(const ZooP* p) {
      if (p->TrExtraInfo()) {
        if (p->TrExtraInfo()->exists(m_num)) {
          return p->TrExtraInfo()->value(m_num);
        } else {
          std::cout << "this info doesn't exist" << std::endl;
        }
      } else {
        std::cout << "no extra info" << std::endl;
      }
      success = false;
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooInfoFunctor,1);
};

class ZooParticleInfoFunctor : public ZooFunctor {
  private:
    int m_num;
  public:
    ZooParticleInfoFunctor(int num) {name = std::string(Form("ParticleInfo%d",num)); m_num = num;}
    float eval(const ZooP* p) {
      if (p->ParticleInfo()) {
        if (p->ParticleInfo()->exists(m_num)) {
          return p->ParticleInfo()->value(m_num);
        } else {
          std::cout << "this info doesn't exist" << std::endl;
        }
      } else {
        std::cout << "no extra info" << std::endl;
      }
      success = false;
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooParticleInfoFunctor,1);
};
class ZooOldCut : public ZooFunctor {
  private:
    ZooLeg*     legreg;
    ZooLeg*     legd;
    ZooFunctor* method;
  public:
    virtual ~ZooOldCut() {delete method; delete legd; delete legreg;}
    ZooOldCut(ZooFunctor* retval) {name = std::string(Form("%s",retval->name.c_str())); method = retval ; legreg = new ZooLeg(new ZooInfoFunctor(4446)); legd = new ZooLeg(new ZooInfoFunctor(4445));}
    float eval(const ZooP* p) {
      int region = legreg->eval(p);
      float dist = legd->eval(p);
      //    std::cout << "Region   " << region << std::endl
      //             << "Distance " << dist   << std::endl;
      if ((0==region && dist<100) || (1==region && dist<200) || (2==region && dist < 9000) || (3==region && dist < 38000))
        return method->eval(p);
      success = false;
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooOldCut,1);
};

class ZooMichel : public ZooFunctor {
  private:
    const ZooP* longleg (const ZooP* mother) {
      const ZooP* thevmleg = NULL;
      const ZooP* ch1 = (*mother)[0];
      const ZooP* ch2 = (*mother)[1];
      if (ch1->TrInfo() && 0==ch1->TrInfo()->history())
        thevmleg = ch2;
      if (ch2->TrInfo() && 0==ch2->TrInfo()->history()) {
        if (thevmleg) std::cout << "watch out!" << std::endl;
        thevmleg = ch1;
      }
      if (NULL == thevmleg) std::cout << "VM leg not found" << std::endl;
      return thevmleg;
    }
    const ZooP* vmleg (const ZooP* mother) {
      const ZooP* thevmleg = NULL;
      const ZooP* ch1 = (*mother)[0];
      const ZooP* ch2 = (*mother)[1];
      if (ch1->TrInfo() && 0==ch1->TrInfo()->history())
        thevmleg = ch1;
      if (ch2->TrInfo() && 0==ch2->TrInfo()->history()) {
        if (thevmleg) std::cout << "watch out!" << std::endl;
        thevmleg = ch2;
      }
      if (NULL == thevmleg) std::cout << "VM leg not found" << std::endl;
      return thevmleg;
    }
  public:
    ZooMichel() {name = std::string("michel");}
    float eval(const ZooP* p) {
      if (vmleg(p)->momentum().R() < 00000) return std::numeric_limits<float>::quiet_NaN();
      if (longleg(p)->momentum().R() < 07000) return std::numeric_limits<float>::quiet_NaN();
      if (vmleg(p)->momentum().Rho() < 500) return std::numeric_limits<float>::quiet_NaN();
      if (longleg(p)->momentum().Rho() < 500) return std::numeric_limits<float>::quiet_NaN();
      if ((*p)[0]->TrInfo()->chi2()/(*p)[0]->TrInfo()->ndf() > 5) return std::numeric_limits<float>::quiet_NaN();
      if ((*p)[1]->TrInfo()->chi2()/(*p)[1]->TrInfo()->ndf() > 5) return std::numeric_limits<float>::quiet_NaN();
      if (p->momentum().M() > 0)
        return p->momentum().M();
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooMichel,1);
};

class ZooMDll : public ZooFunctor {
  public:
    ZooMDll(){name = std::string("muonDLL");}
    float eval(const ZooP* p) {if (p->DLL()) return p->DLL()->DLLmu(); std::cout << "nononono" << std::endl; success=false;return std::numeric_limits<float>::infinity();};
    ClassDef(ZooMDll,1);
};

class ZooTrackChi : public ZooFunctor {
  public:
    ZooTrackChi() {name = std::string("trackchi2");}
    float eval(const ZooP* p) {
      const ZooP* thevmleg;
      const ZooP* ch1 = (*p)[0];
      const ZooP* ch2 = (*p)[1];
      int errorcode = 0;
      if (!ch1->TrExtraInfo()) {std::cout << "WHAT1?" << std::endl;errorcode++;} 
      if (!ch2->TrExtraInfo()) {std::cout << "WHAT2?" << std::endl;errorcode++;}
      if (errorcode>1) return std::numeric_limits<float>::quiet_NaN();
      if (ch1->TrExtraInfo() && ch1->TrExtraInfo()->exists(4444))
        thevmleg = ch1;
      else {
        thevmleg = ch2;
      }
      if (!thevmleg->TrInfo()) {std::cout << "WHAT?" << std::endl; return std::numeric_limits<float>::quiet_NaN();}
      return thevmleg->TrInfo()->chi2()/thevmleg->TrInfo()->ndf();}
      ClassDef(ZooTrackChi,1);
};

class ZooRealTrackChi : public ZooFunctor {
  public:
    ZooRealTrackChi() {name = std::string("realtrackchi2");}
    float eval(const ZooP* p) {
      if (NULL == p->TrInfo() ){ return -999.;}
      return p->TrInfo()->chi2()/p->TrInfo()->ndf();}
      ClassDef(ZooRealTrackChi,1);
};


class ZooTrackFive : public ZooFunctor {
  public:
    ZooTrackFive() {name = std::string("info4445");}
    float eval(const ZooP* p) {
      const ZooP* thevmleg;
      const ZooP* ch1 = (*p)[0];
      const ZooP* ch2 = (*p)[1];
      int errorcode = 0;
      if (!ch1->TrExtraInfo()) {std::cout << "WHAT1?" << std::endl;errorcode++;}
      if (!ch2->TrExtraInfo()) {std::cout << "WHAT2?" << std::endl;errorcode++;}
      if (errorcode>1) return std::numeric_limits<float>::quiet_NaN();
      if (ch1->TrExtraInfo() && ch1->TrExtraInfo()->exists(4444))
        thevmleg = ch1;
      else {
        thevmleg = ch2;
      }
      if (!thevmleg->TrInfo()) {std::cout << "WHAT?" << std::endl; return std::numeric_limits<float>::quiet_NaN();}
      return thevmleg->TrExtraInfo()->value(4445);}
      ClassDef(ZooTrackFive,1);
};

class ZooMass : public ZooFunctor {
  public:
    ZooMass() {name = std::string("mass");}
    float eval(const ZooP* part) {return part->momentum().M();}
    ClassDef(ZooMass,1);
};

class ZooAssoc : public ZooFunctor {
  public:
    ZooAssoc() {name = std::string("associatedmass");}
    float eval(const ZooP* part) {
      float nan = std::numeric_limits<float>::quiet_NaN();
      if (!(*part)[0]->assocMCP()) {std::cout << "no one" << std::endl;return nan;}
      if (!(*part)[1]->assocMCP()) {std::cout << "no two" << std::endl;return nan;}

      if (13!=abs((*part)[0]->assocMCP()->pid())) {std::cout << "no three" << std::endl;return nan;}
      if (13!=abs((*part)[1]->assocMCP()->pid())) {std::cout << "no four" << std::endl;return nan;}

      if ((*part)[0]->assocMCP()->Mo(0) != (*part)[1]->assocMCP()->Mo(0)) {std::cout << "no five" << std::endl;return nan;}
      if (443!=abs((*part)[0]->assocMCP()->Mo(0)->pid())) {std::cout << "no six" << std::endl;return nan;}
      if (part->assocMCP() || ((*part)[0]->assocMCP() && (*part)[1]->assocMCP()/*&& 13 == abs(part->assocMCP()->pid())*/)) 
        return part->momentum().M(); 
      else return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooAssoc,1);
};
class Zoo4040Mass : public ZooFunctor {
  private:
    ZooFunctor* mich;
  public:
    Zoo4040Mass() {name = std::string("confirmedmass");mich = new ZooMichel();}
    virtual ~Zoo4040Mass() {delete mich;}
    float eval(const ZooP* part) {
      const ZooP* thevmleg = NULL;
      const ZooP* ch1 = (*part)[0];
      const ZooP* ch2 = (*part)[1];
      /*
         if (!ch1->TrExtraInfo()) {std::cout << "WHAT1?" << std::endl;errorcode++;}
         if (!ch2->TrExtraInfo()) {std::cout << "WHAT2?" << std::endl;errorcode++;}
         if (errorcode>1) return std::numeric_limits<float>::quiet_NaN();
         if (ch1->TrExtraInfo() && ch1->TrExtraInfo()->exists(4444))
         */
      if (ch1->TrInfo() && 0==ch1->TrInfo()->history())
        thevmleg = ch1;
      if (ch2->TrInfo() && 0==ch2->TrInfo()->history()) {
        if (thevmleg) std::cout << "watch out!" << std::endl;
        thevmleg = ch2;
      }
      if (NULL == thevmleg || ! thevmleg->TrExtraInfo())// || ! thevmleg->TrExtraInfo()->exists(4040))
      {std::cout << "vmleg not found" << std::endl;return std::numeric_limits<float>::quiet_NaN();}
      if (thevmleg->TrExtraInfo()->exists(4040) && //printf("%x\n",int(thevmleg->TrExtraInfo()->value(4040))) && 
          64==(64&(int(thevmleg->TrExtraInfo()->value(4040)))))
        return mich->eval(part);//part->momentum().M();
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(Zoo4040Mass,1);
};

class ZooLinkkey : public ZooFunctor {
  public:
    ZooLinkkey() {name = std::string("LinkKey");}
    virtual ~ZooLinkkey() {}
    float eval(const ZooP* part) {
      if (part->TrExtraInfo() && part->TrExtraInfo()->exists(4040))
        return part->TrExtraInfo()->value(4040);
      return 0;
    }
    ClassDef(ZooLinkkey,1);
};

class ZooLinkfraction : public ZooFunctor {
  private:
    float fraccut;
  public:
    ZooLinkfraction(float cut) {name = std::string(Form("Linkfraction%f",cut)); fraccut = cut;}
    virtual ~ZooLinkfraction() {}
    float eval(const ZooP* part) {
      if (NULL==part->Links()) {std::cout << "link container missing" << std::endl;return std::numeric_limits<float>::quiet_NaN();}
      if (0==part->Links()->NLinks()) {/*std::cout << "zero links" << std::endl;*/return std::numeric_limits<float>::quiet_NaN();}
      float max = 0.;
      for (unsigned int i = 0 ; i < part->Links()->NLinks();++i) {
        if ((part->Links()->Frac(i) > fraccut) && (max < part->Links()->Frac(i))) 
          max = part->Links()->Frac(i);
      }
      if (max>fraccut)  {return max;}
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooLinkfraction,1);
};

class ZooBestLink : public ZooFunctor {
  private:
    ZooFunctor* m_method;
    float fraccut;
  public:
    ZooBestLink(ZooFunctor* method, float frac) {fraccut = frac;name = std::string(Form("BestLinks%s",method->name.c_str()));m_method=method;}
    virtual ~ZooBestLink() {delete m_method;}
    float eval(const ZooP* part) {
      if (NULL==part->Links()) return std::numeric_limits<float>::quiet_NaN();
      if (0==part->Links()->NLinks()) return std::numeric_limits<float>::quiet_NaN();
      float max = 0.;
      int maxind = -1;
      for (unsigned int i = 0 ; i < part->Links()->NLinks();++i) {
        if (part->Links()->Frac(i) > fraccut && max < part->Links()->Frac(i)) {
          max = part->Links()->Frac(i);
          maxind = i;
        }
      }
      if (max<fraccut) return std::numeric_limits<float>::quiet_NaN();
      return m_method->eval(part->Links()->Link(maxind));
    }
    ClassDef(ZooBestLink,1);
};

template <class T>
class ZooTPSTriggerDec : public ZooFunctor {
  private:
  public:
    std::string name_with_type() {return std::string(name+"/I");}
    ZooTPSTriggerDec(std::string decname) { name = std::string("TriggerTPS")+decname ; }
    virtual ~ZooTPSTriggerDec() {};
    virtual float eval(const ZooP* p, const ZooEv* ev) {
      evaluate(p,ev);
      return (float)(int)m_lastresult.i;
    }
    virtual void evaluate(const ZooP* p, const ZooEv*) {
      if (NULL==p->TrigDec()) {
        m_lastresult.i = -99;
        return;
      }
      m_lastresult.i = (int)(bool)(p->TrigDec()->getTPS<T>());
      return ;
    }
    virtual float eval(const ZooP* p) {
      return eval(p,NULL);
    }
};
template <class T>
class ZooTOSTriggerDec : public ZooFunctor {
  private:
  public:
    std::string name_with_type() {return std::string(name+"/I");}
    ZooTOSTriggerDec(std::string decname) { name = std::string("TriggerTOS")+decname ; }
    virtual ~ZooTOSTriggerDec() {};
    virtual float eval(const ZooP* p, const ZooEv* ev) {
      evaluate(p,ev);
      return (float)(int)m_lastresult.i;
    }
    virtual void evaluate(const ZooP* p, const ZooEv*) {
      if (NULL==p->TrigDec()) {
        m_lastresult.i = -99;
        return;
      }
      m_lastresult.i = (int)(bool)(p->TrigDec()->getTOS<T>());
      return ;
    }
    virtual float eval(const ZooP* p) {
      return eval(p,NULL);
    }
};
template <class T>
class ZooTISTriggerDec : public ZooFunctor {
  private:
  public:
    std::string name_with_type() {return std::string(name+"/I");}
    ZooTISTriggerDec(std::string decname) { name = std::string("TriggerTIS")+decname ; }
    virtual ~ZooTISTriggerDec() {};
    virtual float eval(const ZooP* p, const ZooEv* ev) {
      evaluate(p,ev);
      return (float)(int)m_lastresult.i;
    }
    virtual void evaluate(const ZooP* p, const ZooEv*) {
      if (NULL==p->TrigDec()) {
        m_lastresult.i = -99;
        return;
      }
      m_lastresult.i = (int)(bool)(p->TrigDec()->getTIS<T>());
      return ;
    }
    virtual float eval(const ZooP* p) {
      return eval(p,NULL);
    }
};


template <class T>
class ZooEventTriggerDec : public ZooFunctor {
  private:
  public:
    std::string name_with_type() {return std::string(name+"/I");}
    ZooEventTriggerDec(std::string decname) { name = std::string("TriggerDec")+decname ; }
    virtual ~ZooEventTriggerDec() {};
    virtual float eval(const ZooP* p, const ZooEv* ev) {
      evaluate(p,ev);
      return (float)(int)m_lastresult.i;
    }
    virtual void evaluate(const ZooP*, const ZooEv* ev) {
      m_lastresult.i = (int)ev->getTriggerDec<T>();
      return ;
    }
    virtual float eval(const ZooP*) {
      std::cerr << "You shouldn't do that" << std::endl;
      return std::numeric_limits<float>::quiet_NaN();
    }

    //ClassDef(ZooEventTriggerDec,1);
};
//ClassImp(ZooEventTriggerDec);

class ZooTISTOS : public ZooFunctor {
  public:
    ZooTISTOS() {name = "tistosser"; }
    float eval(const ZooP* part) {
      if(!(part->TrigDec())) return -1;
      const ZooTrigger* tr = part->TrigDec();
      //return (float)(tr->getTOS<TriggerDecisions::Hlt1TrackMuon>());
      //return (float)(0xFF&(tr->getTriggerTOSWord(1)));
      return (float)(bool(0x300000fc&(tr->getTriggerTOSWord(1)) ) && bool(0xfffffff7&(tr->getTriggerTOSWord(0))));
    }
    ClassDef(ZooTISTOS,1);
};

class ZooConfirmedVeloCalo : public ZooFunctor {
  private:
    const ZooP* vcleg(const ZooP* mother) {
      if (mother->NCh() != 2) {std::cout << "NCh != 2" << std::endl; return NULL;}
      const ZooP* ch1 = (*mother)[0];
      const ZooP* ch2 = (*mother)[1];
      if (ch1->TrExtraInfo() && ch1->TrExtraInfo()->exists(4001)) {
        if (ch2->TrExtraInfo() && ch2->TrExtraInfo()->exists(4001))
          std::cout << "more than one child found" << std::endl;
        return ch1;
      }
      if (ch2->TrExtraInfo() && ch2->TrExtraInfo()->exists(4001))
        return ch2;
      std::cout << "child not found" << std::endl;
      return NULL;
    }
    ZooFunctor* m_method;
  public:
    ZooConfirmedVeloCalo(ZooFunctor* method) {name = std::string(Form("ZooConfirmedVeloCalo%s",method->name.c_str())); m_method = method;}
    virtual ~ZooConfirmedVeloCalo() {delete m_method;}
    float eval(const ZooP* mother) {
      const ZooP* vc = vcleg(mother);
      if (vc && vc->TrExtraInfo()->value(4001))
        return m_method->eval(mother);
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooConfirmedVeloCalo,1);
};

class ZooBestSelect :public ZooFunctor {
  private:
    ZooFunctor* m_method;
    ZooFunctor* m_dll;
    ZooFunctor* m_michel;
    ZooFunctor* m_tistos;
  public:
    ZooBestSelect(ZooFunctor* method) {
      m_method=method;
      name=std::string(Form("Selected%s",method->name.c_str()));
      m_dll = new ZooOLeg(new ZooMDll());
      m_michel = new ZooMichel();
      m_tistos = new ZooOLeg(new ZooTISTOS());
    }
    virtual ~ZooBestSelect() {delete m_method; delete m_michel ; delete m_dll; delete m_tistos;}
    float eval(const ZooP* p) {
      if (p->decayVertexPacked().chi2() < 1
          && m_dll->eval(p) > 2
          && m_tistos->eval(p) != 0
         )//&& m_michel->eval(p) == p->momentum().M())
           return m_method->eval(p);
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooBestSelect,1);
};

class ZooMomentum : public ZooFunctor {
  public:
    ZooMomentum() {name = std::string("momentum");}
    float eval(const ZooP* part) {return part->momentum().P();}
    ClassDef(ZooMomentum,1);
};
class ZooPt : public ZooFunctor {
  public:
    ZooPt() {name = std::string("pt");}
    float eval(const ZooP* part) {return part->momentum().Pt();}
    ClassDef(ZooPt,1);
};
class ZooPhi : public ZooFunctor {
  public:
    ZooPhi() {name = std::string("phi");}
    float eval(const ZooP* part) {return part->momentum().phi();}
    ClassDef(ZooPhi,1);
};
class ZooEta : public ZooFunctor {
  public:
    ZooEta() {name = std::string("eta");}
    float eval(const ZooP* part) {return part->momentum().eta();}
    ClassDef(ZooEta,1);
};
class ZooUnconfirmed : public ZooFunctor {
  private:
    ZooFunctor* mich;
    ZooFunctor* conf;
  public:
    ZooUnconfirmed() {
      name = std::string("unconfirmedmass");
      mich = new ZooMichel();
      conf= new Zoo4040Mass();
    }
    virtual ~ZooUnconfirmed() {delete mich;}
    float eval(const ZooP* part) {
      if (mich->eval(part) != conf->eval(part)) return mich->eval(part);
      return std::numeric_limits<float>::quiet_NaN(); 
    }
    ClassDef(ZooUnconfirmed,1);
};


class ZooVertChi : public ZooFunctor {
  public:
    ZooVertChi() {name = std::string("VertexChi2");}
    float eval(const ZooP* p) {return p->decayVertexPacked().chi2();}
    ClassDef(ZooVertChi,1);
};

class ZooVertNDOF : public ZooFunctor {
  public:
    ZooVertNDOF() {name = std::string("VertexNDOF");}
    float eval(const ZooP* p) {return p->decayVertexPacked().nDoF();}
    ClassDef(ZooVertNDOF,1);
};

class ZooHash : public ZooFunctor {
  public:
    float eval(const ZooP* , const ZooEv* event) {
      unsigned runnr = event->runNumber();
      m_lastresult.i = TString::Hash(&runnr,sizeof(unsigned));
      std::size_t evtnr = event->eventNumber();
      m_lastresult.i ^= TString::Hash(&evtnr,sizeof(std::size_t));
      return (float)m_lastresult.i;
    }
    virtual std::string name_with_type() {return std::string("EventHash/I");}
    ZooHash() {name = std::string("EventHash");}
    ClassDef(ZooHash,1);
};

class ZooLocalVar : public ZooFunctor {
  private:
    float *fpoint;
    int*  ipoint;
    unsigned int* uipoint;
  public:
    ZooLocalVar(float* point,std::string thename) {name = thename; fpoint = point, ipoint=NULL, uipoint=NULL;}
    ZooLocalVar(int* point,std::string thename) {name = thename; ipoint = point, fpoint=NULL, uipoint=NULL;}
    ZooLocalVar(unsigned int* point,std::string thename) {name = thename; uipoint = point, ipoint=NULL, fpoint=NULL;}
    virtual std::string name_with_type() {if (fpoint) return std::string(name+"/F"); return std::string(name+"/I");}
    virtual ~ZooLocalVar() {};
    virtual void evaluate(const ZooP* , const ZooEv* ) { if (fpoint) {m_lastresult.f = *fpoint ; return ;} if (ipoint) {m_lastresult.i = *ipoint; return ; } if (uipoint) {m_lastresult.u = *uipoint; return ; } return ;}
    virtual float eval(const ZooP*) {
      if (fpoint) {
        m_lastresult.f = *fpoint ;
        return *fpoint;
      } if (ipoint) {
        m_lastresult.i = *ipoint; 
        return (float)(int)*ipoint; 
      }
      if (uipoint) {
        m_lastresult.u = *uipoint; 
        return (float)(unsigned int)(*uipoint); 
      }
      return std::numeric_limits<float>::quiet_NaN();
    }
    virtual float eval(const ZooP*, const ZooEv*) { 
      if (fpoint) {
        m_lastresult.f = *fpoint ; 
        return *fpoint;
      } 
      if (ipoint) {
        m_lastresult.i = *ipoint; 
        return (float)(int)*ipoint; 
      } 
      if (uipoint) {
        m_lastresult.u = *uipoint; 
        return (float)(unsigned int)(*uipoint); 
      } 
      return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(ZooLocalVar,1);
};


class ZooEventPointer : public ZooFunctor {
  private:
    eunc theeunc;
    iunc theiunc;
    uiunc theuiunc;
    efunc thefunc;
    eifunc theeifunc;
    std::string type;
  public:
    virtual std::string name_with_type() {return std::string(Form("%s/%s",name.c_str(),type.c_str()));}
    ZooEventPointer() {type = std::string("F");name = std::string("dummy"); thefunc = NULL;std::cout  << "nonononono" << std::endl;}
    //virtual ~ZooEventPointer(); 
    ZooEventPointer(eunc locfunc,const char* thename) {type = std::string("F");name = std::string(thename); theeunc = locfunc; thefunc = NULL;} 
    ZooEventPointer(iunc locfunc,const char* thename) {type = std::string("I");name = std::string(thename); theiunc = locfunc; thefunc = NULL; theeunc = NULL;} 
    ZooEventPointer(uiunc locfunc,const char* thename) {type = std::string("I");name = std::string(thename); theuiunc = locfunc;theiunc=NULL; thefunc = NULL; theeunc = NULL;} 
    ZooEventPointer(efunc locfunc, const char* thename) {type = std::string("F");name=std::string(thename); thefunc = locfunc;}
    ZooEventPointer(eifunc locfunc, const char* thename) {type = std::string("I");name=std::string(thename); theeifunc = locfunc;theiunc=NULL; thefunc = NULL; theeunc = NULL;theuiunc = NULL;}
    // float eval(const ZooP* part, const ZooEv* event) {if (thefunc) {return thefunc(part,event);} if (theeunc) {return ((*event).*theeunc)();} if (theiunc) {return (float)((*event).*theiunc)();} std::cout << "WTF"<< std::endl;return 0;}
    float eval(const ZooP* part, const ZooEv* event) {
      evaluate(part,event);
      if (thefunc || theeunc)
        return m_lastresult.f;
      if (theiunc || theuiunc || theeifunc)
        return (float)(int)m_lastresult.i;
      return 0.;
    }
    float eval(const ZooP*) {std::cout << "this should not happen" << std::endl;return std::numeric_limits<float>::quiet_NaN();}
    virtual void evaluate(const ZooP* part, const ZooEv* event) {
      if (thefunc) {
        m_lastresult.f = thefunc(part,event);
        return;
      }
      if (theeunc) {
        m_lastresult.f = ((*event).*theeunc)();
        return;
      } 
      if (theiunc) {
        m_lastresult.i = ((*event).*theiunc)();
        return;
      }
      if (theuiunc) {
        m_lastresult.i = ((*event).*theuiunc)();
        return;
      }
      if (theeifunc) {
        m_lastresult.i = theeifunc(part,event);
        return;
      }
      std::cout << "WTF"<< std::endl;
      return;
    }
    ClassDef(ZooEventPointer,2);
};

class ZooPointer : public ZooFunctor {
  private:
    std::string type;
  public:
    virtual std::string name_with_type() {return std::string(Form("%s/%s",name.c_str(),type.c_str()));}
    ZooPointer() {name = std::string("pointer"); thefunc = &ZooP::ip;type = std::string("F");}
    ZooPointer(gfunc locfunc) {name = std::string("pointer");theifunc=NULL; thegfunc = locfunc; thefunc = NULL;type = std::string("F");}
    ZooPointer(ifunc locfunc,const char* thename) {name = std::string(thename); theifunc = locfunc; thefunc = NULL; thegfunc=NULL; type = std::string("I");}
    ZooPointer(ifunc locfunc,std::string thename) {name = thename; theifunc = locfunc; thefunc = NULL; thegfunc=NULL; type = std::string("I");}
    ZooPointer(gfunc locfunc,const char* thename) {name = std::string(thename); theifunc = NULL;thegfunc = locfunc; thefunc = NULL;type = std::string("F");}
    ZooPointer(gfunc locfunc,std::string thename) {name = thename; thegfunc = locfunc;theifunc=NULL; thefunc = NULL;type = std::string("F");}
    ZooPointer(func locfunc) {name = std::string("pointer"); thefunc = locfunc; thegfunc = NULL;theifunc=NULL;type = std::string("F");}
    ZooPointer(func locfunc,const char* thename) {name = std::string(thename); thefunc = locfunc; thegfunc = NULL;theifunc=NULL;type = std::string("F");}
    ZooPointer(func locfunc,std::string thename) {name = thename; thefunc = locfunc; thegfunc = NULL;theifunc=NULL;type = std::string("F");}
    ZooPointer(particleintfunc locfunc, std::string thename) {name = thename ; thefunc =NULL; thegfunc = NULL; pint = locfunc; type = std::string("I");theifunc=NULL;}
    //~ZooPointer() ;
    func thefunc;
    gfunc thegfunc;
    ifunc theifunc;
    particleintfunc pint;
    float eval(const ZooP* part) {evaluate(part,NULL);
      if (thefunc||thegfunc) return m_lastresult.f;
      return (float)(int)m_lastresult.i;
    }
    /*float eval(const ZooP* part) {if (thefunc) return ((*part).*thefunc)();
      if (thegfunc) return thegfunc(part);
      return (float) ((*part).*pint)();}*/
    virtual void evaluate(const ZooP* part, const ZooEv* ) {
      if (thefunc) {m_lastresult.f = ((*part).*thefunc)(); return;}
      if (thegfunc) {m_lastresult.f = thegfunc(part); return;}
      if (theifunc) {m_lastresult.i = theifunc(part); return;}
      m_lastresult.i = ((*part).*pint)();
      return;
    }
    ClassDef(ZooPointer,2);
};

class ZooIP : public ZooPointer {
  public:
    ZooIP() : ZooPointer(&ZooP::ip) {name = std::string("IP");}
    ClassDef(ZooIP,1);
};

/*float flight(const ZooP* particle) {
  return particle->fDistance();
  }

  class ZooFlight : public ZooPointer {
  public:
  ZooFlight() : ZooPointer(&flight,(char*)"FlightDistance") {}
  ClassDef(ZooFlight,1);
  };
  */

class ZooIPSig : public ZooPointer {
  public:
    ZooIPSig() : ZooPointer(&ZooP::ipSig) {name = std::string("IPSignificance");}
    ClassDef(ZooIPSig,1);
};

class ZooOTHits : public ZooFunctor{
  public:
    ZooOTHits() {name = std::string("OTocc");}
    float eval(const ZooP* , const ZooEv* ev) {
      unsigned count = 0;
      std::vector< std::vector< std::vector< std::vector< int > > > > occOT;
      ev->occupancy()->getOccOT(occOT);
      for (unsigned sta = 0; sta < 3; ++sta)
        for (unsigned qua = 0; qua < 4; ++qua)
          for (unsigned lay = 0; lay < 4; ++lay)
            for (unsigned mod = 0; mod < 9; ++mod)
              count += occOT[sta][qua][lay][mod];
      return float(count);
    }
    ClassDef(ZooOTHits,1);
};

class ZooITHits : public ZooFunctor{
  public:
    ZooITHits() {name = std::string("ITocc");}
    float eval(const ZooP* , const ZooEv* ev) {
      unsigned count = 0;
      std::vector< std::vector< std::vector< std::vector< int > > > > occIT;
      ev->occupancy()->getOccIT(occIT);
      for (unsigned sta = 0; sta < 3; ++sta)
        for (unsigned lay = 0; lay < 4; ++lay)
          for (unsigned box = 0; box < 4; ++box)
            for (unsigned sen = 0; sen < 7; ++sen)
              count += occIT[sta][lay][box][sen];
      return float(count);
    }
    ClassDef(ZooITHits,1);
};

class ZooTTHits : public ZooFunctor{
  public:
    ZooTTHits() {name = std::string("TTocc");}
    float eval(const ZooP* , const ZooEv* ev) {
      unsigned count = 0;
      std::vector< std::vector< std::vector< std::vector< int > > > > occTT;
      ev->occupancy()->getOccTT(occTT);
      for (unsigned sta = 0; sta < 3; ++sta)
        for (unsigned lay = 0; lay < 4; ++lay)
          for (unsigned box = 0; box < 4; ++box)
            for (unsigned sen = 0; sen < 7; ++sen)
              count += occTT[sta][lay][box][sen];
      return float(count);
    }
    ClassDef(ZooTTHits,1);
};

class ZooVeloHits : public ZooFunctor{
  public:
    ZooVeloHits() {name = std::string("VELOocc");}
    float eval(const ZooP* , const ZooEv* ev) {
      unsigned count = 0;
      std::vector< std::vector< std::vector< int > > > occVelo;
      ev->occupancy()->getOccVelo(occVelo);
      for (unsigned sta = 0; sta < 23; ++sta)
        for (unsigned sid = 0; sid < 2; ++sid)
          for (unsigned typ = 0; typ < 2; ++typ)
            count += occVelo[sta][sid][typ];
      return float(count);
    }
    ClassDef(ZooVeloHits,1);
};



class NewIPCut :public ZooFunctor {
  public:
    ZooFunctor* best;
    ZooFunctor* ipcut;
    NewIPCut() {best = new ZooBestSelect(new ZooMass()); name = std::string("whatIPcut"); ipcut = new ZooOLeg(new ZooIPSig());}
    virtual ~NewIPCut() {delete best;delete ipcut;}
    float eval(const ZooP* p) {
      if (4<ipcut->eval(p)) return best->eval(p);
      else return std::numeric_limits<float>::quiet_NaN();
    }
    ClassDef(NewIPCut,1);
};

class UpdatedMass : public ZooFunctor {
  private:
    const ZooP* assocLong(const ZooP* JPsi);
    const ZooP* longleg(const ZooP* JPsi);
    const ZooP* vmleg(const ZooP* JPsi);
  public:
    UpdatedMass() {name = std::string("UpdatedMass");}
    float eval(const ZooP* p);
    ClassDef(UpdatedMass,1);
};

class ZooCloneDist : public ZooInfoFunctor {
  public:
    ZooCloneDist() : ZooInfoFunctor(101) {name = std::string("CloneDist");}
    ClassDef(ZooCloneDist,1);
};
ClassImp(ZooCloneDist);

#endif
