#include "ParticleHandle.h"
#include "MyKin.h"
#include <TDirectory.h>


void ParticleHandle::Fill(const ZooP* p, const ZooEv* ev, double w) {
  if (!m_initialised) initialise(p);
  m_kinplot->Fill(p,ev,w);
  for (unsigned d = 0 ; d < p->NCh() ; ++d) {
    int pid = abs(p->Ch(d)->pid());
    if (map.end() == map.find(pid)) {
      std::cerr << "Found a particle " << pid << " where I didn't expect one." << std::endl;
    } else {
      map[pid]->Fill(p->Ch(d),ev,w);
    }
  }

}

void ParticleHandle::Write(TDirectory* dir) {
  if (!m_initialised) return;
  m_kinplot->Write(dir);
  for (std::map<int,ParticleHandle*>::iterator it = map.begin() ; it != map.end() ; ++it) {
    TDirectory* subdir = dir->mkdir(Form("%d",(*it).first));
    (*it).second->Write(subdir);
  }
}

ParticleHandle::~ParticleHandle() {
  for (std::map<int,ParticleHandle*>::iterator it = map.begin() ; it != map.end() ; ++it) {
    delete (*it).second;
  }
  delete m_kinplot;
}

void ParticleHandle::initialise(const ZooP* p) {
  m_kinplot = new MyKin(m_prefix, p);
  for (unsigned d = 0 ; d < p->NCh() ; ++d) {
    int pid = abs(p->Ch(d)->pid());
    if (map.end() == map.find(pid)) {
      // need to make a new daughter
      map.insert( std::pair<int,ParticleHandle*>(pid, new ParticleHandle(m_prefix + Form("_%d",pid)) ) );
    }
  }
  m_initialised = true;
}

ParticleHandle::ParticleHandle() {
  //m_kinplot = new MyKin();
  m_initialised = false;
}

ParticleHandle::ParticleHandle(std::string prefix) {
  m_prefix = prefix;
  //m_kinplot = new MyKin(prefix);
  m_initialised = false;
}
