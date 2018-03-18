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

// void Table::ComputeDV(unsigned num) {
// 	map<unsigned,next_lat> old_DV = GetDV(num);


// }

// void Table::GetDV(unsigned num) {
// 	return lookup[num];
// }

ostream & Table::Print(ostream &os) const {
	map<unsigned,map<unsigned,next_lat> >::iterator i;
	map<unsigned,next_lat>::iterator j;
	for(i=this->lookup.begin(); i!=this->lookup.end(); ++i) {
		for(j=i->second.begin(); j!=i->second.end(); ++j) {
			os << "From: " << i->first << " To: " << j->first << "Through: " << j->second.next << " Lat: " << j->second.lat << endl;
		}
	}
	return os;
}
#endif
