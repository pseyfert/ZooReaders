#ifndef PARTICLEHANDLE_H
#define PARTICLEHANDLE_H 1
/**
 * @class ParticleHandle
 * @brief handles a decay structure to MyKin ploters
 *
 * @author Paul Seyfert
 * @date   2012-01-21
 */
#include <map>
#include <string>
class MyKin;
class ZooP;
class ZooEv;
class TDirectory;

class ParticleHandle {
  private:
    MyKin* m_kinplot;
    std::map<int,ParticleHandle*> map;
    bool m_initialised;
    void initialise(const ZooP*);
    std::string m_prefix;
  public:
    void Fill(const ZooP*, const ZooEv*, double w=1.);
    void Write(TDirectory* dir);
    ParticleHandle();
    ParticleHandle(std::string prefix);
    ~ParticleHandle();
};

#endif

