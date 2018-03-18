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

void Dijkstra_Init(map<unsigned, double> &value, map<unsigned, unsigned> &predecessor, set<unsigned> &remaining_nodes)
{
    // helper function to initializa Dijkstra's algorithm
    set<unsigned>::const_iterator i;
    for (i = remaining_nodes.begin(); i != remaining_nodes.end(); i++)
    {
        predecessor[*i] = UNDEFINED;
        value[*i] = INFINITY;
    }
}

// Table class functions
Table::Table(unsigned i)
{
    current = i;
    info.table_links = deque<const Link*>();
    info.table_paths = map<unsigned, Edge>();
}


void Table::UpdateAlgorithm()
{
    // Use Dijkstra's Algorithm to update table
    map<unsigned, double> value;
    map<unsigned, unsigned> predecessor;
    set<unsigned> remaining_nodes (info.table_nodes);
    
    Dijkstra_Init(value, predecessor, remaining_nodes);
    
    value[current] = 0;  // distance to the beginning node is 0
    
    // main loop
    while(!remaining_nodes.empty())
    {
        set<unsigned>::iterator closest_node = remaining_nodes.begin();
        double shortest_dist = value[*closest_node];
        
        set<unsigned>::const_iterator curr_node;
        map<unsigned, double>::const_iterator neighbor;
        
        for (curr_node = remaining_nodes.begin(); curr_node != remaining_nodes.end(); curr_node++)
        {
            // if there is a new closest node, save it
            if (value[*curr_node] < shortest_dist)
            {
                shortest_dist = value[*curr_node];
                closest_node = curr_node;
            }
        }
        
        if (shortest_dist == INFINITY) {
            cerr << "Graph is unsolvable using Dijkstra's Algorithm" << endl;
            break;
        }
        
        
        for (neighbor = info.table_connections[*closest_node].begin(); neighbor != info.table_connections[*closest_node].end(); neighbor++)
        {
            // if the distance to the current + the distance to the neighbor is less than the stored distance to the neighbor, update the table
            double neighbor_dist = value[*closest_node] + neighbor->second;
            
            if (neighbor_dist < value[neighbor->first])
            {
                value[neighbor->first] = neighbor_dist;
                predecessor[neighbor->first] = *closest_node;
            }
        }
        
        remaining_nodes.erase(closest_node);
    }
    
    
    // update the table with new info
    UpdateTable(value, predecessor);
}

void Table::UpdateTable(map<unsigned, double> &value, map<unsigned, unsigned> &predecessor)
{
    info.table_paths.clear();
    
    set<unsigned>::const_iterator curr_node;
    
    for (curr_node = info.table_nodes.begin(); curr_node != info.table_nodes.end(); curr_node++)
    {
        // skip the current node
        if (*curr_node == current) {
            continue;
        }
        
        // otherwise, update table info if there is a path to the current node
        unsigned temp_node = *curr_node;
        
        while (predecessor[temp_node] != current) {
            temp_node = predecessor[temp_node];
        }
        info.table_paths[*curr_node] = Edge(temp_node, value[*curr_node]);
    }
}

ostream & Table::Print(ostream &os) const
{
    return os;
}

// TableInfo class functions
void TableInfo::NewLink(const Link* l)
{
    if (!ExistingLink(l))
    {
        unsigned source = l->GetSrc();
        unsigned dest = l->GetDest();

        table_links.push_back(l);
        table_connections[source][dest] = l->GetLatency();
        table_nodes.insert(source);
        table_nodes.insert(dest);
    }
}

bool TableInfo::ExistingLink(const Link* l)
{
    deque<const Link*>::const_iterator i;
    
    for (i = table_links.begin(); i != table_links.end(); i++)
    {
        // if link has the same source/dest, it could be there
        if ((*i)->GetSrc() == l->GetSrc() && (*i)->GetDest() == l->GetDest())
        {
            // if link has the same latency, it's there
            if ((*i)->GetLatency() == l->GetLatency())
            {
                return true;
            }
        }
    }
    // otherwise, it's not there
    return false;
}

unsigned TableInfo::GetNextHop_Table(unsigned next) const
{
    map<unsigned, Edge>::const_iterator i = table_paths.find(next);
    
    if (i == table_paths.end())
    {
        return UNDEFINED;
    }
    else
    {
        return i->second.pointsto;
    }
}


#endif

#if defined(DISTANCEVECTOR)

// constructors
Table::Table() {}

Table::Table(Node n) {
    deque<Link*> l = n.GetOutgoingLinks();
    int dest;
    int lat;
    deque<Link*>::iterator i;
    
    for (i = l->begin(); i != l->end();, i++;)
    {
        dest = (*i)->GetDest();
        lat = (*i)->GetLatency();
        WriteTable(dest, dest, lat);
    }
    
    delete l;
}



#endif
