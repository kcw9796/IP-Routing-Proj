#ifndef _table
#define _table

#include "link.h"
#include <iostream>
#include <map>
#include <deque>
#include <set>

#define INFINITY (~0x00)
#define UNDEFINED (~0x00)

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)

struct Edge {
    double cost;  // cost of the edge (latency)
    unsigned pointsto; // the item that the edge is directed towards (next hop)
    
    Edge() {}
    Edge(double c, unsigned p) {
        cost = c;
        pointsto = p;
    }
};

class TableInfo {
public:
    deque<const Link*> table_links;
    map<unsigned, Edge> table_paths;
    set<unsigned> table_nodes;
    map<unsigned, map<unsigned, double> > table_connections;
    
    void NewLink(const Link* l);
    bool ExistingLink(const Link* l);
    unsigned GetNextHop_Table(unsigned next) const;
};

class Table {
  // Students should write this class
public:
    unsigned current;
    TableInfo info;
    
    // constructors
    Table() {};
    Table(unsigned i);
    
    // table manipulations
    void UpdateAlgorithm();
    void UpdateTable(map<unsigned, double> &value, map<unsigned, unsigned> &predecessor);


    // table viewing/search
    ostream & Print(ostream &os) const;

};

#endif

#if defined(DISTANCEVECTOR)

#include <deque>

class Table {
public:
    ostream & Print(ostream &os) const;

private:
    voctor<vector<double> > table;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
