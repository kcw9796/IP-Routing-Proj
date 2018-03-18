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

  Table();
  Table(const Table &rhs);
  void LinkInit(unsigned num, const Link *l);
  bool ComputeDV(unsigned num, deque<Link*> *links);
  map<unsigned,next_lat> GetDV(unsigned num);
  void SetDV(unsigned num, map<unsigned,next_lat> dv);
  unsigned GetNextHop(const unsigned num, const unsigned dest) const;
  ostream & Print(ostream &os) const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
