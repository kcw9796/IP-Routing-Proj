#ifndef _table
#define _table


#include <iostream>
#include <map>

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
class Table {
  // Students should write this class
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)

#include <deque>
#include "link.h"
struct next_lat {
	unsigned next;
	double lat;
};

class Table {
 public:
  map<unsigned,map<unsigned,next_lat> > lookup;

  void LinkInit(unsigned num, const Link *l);
  // void ComputeDV(unsigned num);
  // void GetDV(unsigned num);
  ostream & Print(ostream &os) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
