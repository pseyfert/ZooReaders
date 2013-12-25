#include "comb.h"

float combine(const ZooP* p, const ZooP* p2) {
  zooLorentzVector mom = p->momentum();
  mom+=p2->momentum();
  return mom.M();

  return 2.3;
}

float combine(zooLorentzVector p, zooLorentzVector p2) {
  p+=p2;
  return p.M();
}
