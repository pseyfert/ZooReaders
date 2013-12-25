TARGETS = ZooFunctorDict.cpp libZooFunctorROOT.a libZooFunctorROOT.so
# make sure we include the dictionary when linking
alldicts += ZooFunctorDict.cpp

include Rules.mk
CXXFLAGS += -fpermissive

# what belongs in which file
ZooFunctorDict.cpp:   $(filter-out %Dict.h,$(allsrc:%.cpp=%.h)) ZooFunctorLinkDef.h
libZooFunctorROOT.a:  libZooFunctorROOT.a($(alldicts:%.cpp=%.o) $(allsrc:%.cpp=%.o))
libZooFunctorROOT.so: $(alldicts:%.cpp=%.os) $(allsrc:%.cpp=%.os)
