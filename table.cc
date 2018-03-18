#include "table.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)

#endif

#if defined(DISTANCEVECTOR)

Table::Table(const Table &rhs) {
	this->lookup = rhs.lookup;
}

Table::Table() {}

void Table::LinkInit(unsigned num, const Link *l) {
	next_lat nl;
	nl.next = l->GetDest();
	nl.lat = l->GetLatency();

	map<unsigned, map<unsigned,next_lat> >::iterator i = this->lookup.find(num);
	if(i!=this->lookup.end()) {
		map<unsigned,next_lat>::iterator j = i->second.find(l->GetDest());
		if(j == i->second.end()) {
			i->second[l->GetDest()] = nl;
		}
	}
	else {
		map<unsigned, next_lat> m;
		m[l->GetDest()] = nl;
		this->lookup[num] = m;
	}
}

void Table::SetDV(unsigned num, map<unsigned,next_lat> dv) {
	this->lookup[num] = dv;
}

bool Table::ComputeDV(unsigned num, deque<Link*> *links) {
	map<unsigned,next_lat> old_DV = GetDV(num);
	deque<Link*>::iterator i;
	next_lat nl;
	for(i=links->begin(); i!=links->end(); ++i) {
		nl.next = (*i)->GetDest();
		nl.lat = (*i)->GetLatency();
		this->lookup[num][nl.next] = nl; 
	}

	map<unsigned,map<unsigned,next_lat> >::const_iterator j;
	map<unsigned,next_lat>::const_iterator k,l;
	for(j=this->lookup.begin(); j!=this->lookup.end(); ++j) {
		if(num != j->first) {
			for(k=j->second.begin(); k!=j->second.end(); ++k) {
				if(k->first != num) {
					l = this->lookup[num].find(k->first);
					if(l == this->lookup[num].end() || l->second.lat > k->second.lat) {
						this->lookup[num][k->first] = k->second;
					}
				}
			}
		}
	}

	bool change = false;
	for(k=this->lookup[num].begin(); k!=this->lookup[num].end(); ++k) {
		if(old_DV.find(k->first) == old_DV.end() || k->second.next != old_DV[k->first].next ||
			k->second.lat != old_DV[k->first].lat) {
			change = true;
		}
	}
	return change;
}

map<unsigned,next_lat> Table::GetDV(unsigned num) {
	return lookup[num];
}

ostream & Table::Print(ostream &os) const {
	map<unsigned,map<unsigned,next_lat> >::const_iterator i;
	map<unsigned,next_lat>::const_iterator j;
	for(i=this->lookup.begin(); i!=this->lookup.end(); ++i) {
		for(j=i->second.begin(); j!=i->second.end(); ++j) {
			os << "From:" << i->first << " To:" << j->first << " Through:" << j->second.next << " Lat:" << j->second.lat << endl;
		}
	}
	return os;
}
#endif
