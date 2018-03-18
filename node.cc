#include "node.h"
#include "context.h"
#include "error.h"

Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
number(n), context(c), bw(b), lat(l)
{
#if defined(LINKSTATE)
    lTable = Table(n);
#endif
}

Node::Node() 
{ throw GeneralException(); }

Node::Node(const Node &rhs) : 
number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat) {}

Node & Node::operator=(const Node &rhs) 
{
    return *(new(this)Node(rhs));
}

void Node::SetNumber(const unsigned n) 
{ number=n;}

unsigned Node::GetNumber() const 
{ return number;}

void Node::SetLatency(const double l)
{ lat=l;}

double Node::GetLatency() const 
{ return lat;}

void Node::SetBW(const double b)
{ bw=b;}

double Node::GetBW() const 
{ return bw;}

Node::~Node()
{}

// Implement these functions to post an event to the event queue in the event simulator
// so that the corresponding node can recieve the ROUTING_MESSAGE_ARRIVAL event at the proper time
void Node::SendToNeighbors(const RoutingMessage *m)
{
    context->SendToNeighbors(this, m);
    
    //    // Send to each neighbor individually
    //    deque <Node*> *neighbors = GetNeighbors();
    //    deque <Node*>::iterator i;
    //    for (i = neighbors->begin(); i != neighbors->end(); i++)
    //    {
    //        SendToNeighbor(*i, m);
    //    }
}

void Node::SendToNeighbor(const Node *n, const RoutingMessage *m)
{
    context->SendToNeighbor(this, n, m);
    
    //    const Link* temp_link = new Link(this->number, n->number, NULL, 0, 0);
    //    Link* matching_link = context->FindMatchingLink(temp_link);
    //
    //    if (matching_link != 0)
    //    {
    //        Event* new_event = new Event(context->GetTime() + matching_link->GetLatency(),
    //                                     ROUTING_MESSAGE_ARRIVAL,
    //                                     (void*)n,
    //                                     (void*)m);
    //        context->PostEvent(new_event);
    //    }
    //    delete temp_link;
}

deque<Node*> *Node::GetNeighbors()
{
    return context->GetNeighbors(this);
}

void Node::SetTimeOut(const double timefromnow)
{
    context->TimeOut(this,timefromnow);
}


bool Node::Matches(const Node &rhs) const
{
    return number==rhs.number;
}


#if defined(GENERIC)
void Node::LinkHasBeenUpdated(const Link *l)
{
    cerr << *this << " got a link update: "<<*l<<endl;
    //Do Something generic:
    SendToNeighbors(new RoutingMessage);
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
    cerr << *this << " got a routing messagee: "<<*m<<" Ignored "<<endl;
}


void Node::TimeOut()
{
    cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
    return 0;
}

Table *Node::GetRoutingTable() const
{
    return new Table;
}


ostream & Node::Print(ostream &os) const
{
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
    return os;
}

#endif

#if defined(LINKSTATE)

//////// WRITE THESE ////////
void Node::LinkHasBeenUpdated(const Link *l)
{
    cerr << *this<<": Link Update: "<<*l<<endl;
    
    
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
    cerr << *this << " Routing Message: "<<*m;
}

void Node::TimeOut()
{
    cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
    // WRITE
    return 0;
}

Table *Node::GetRoutingTable() const
{
    // WRITE
    return 0;
}


ostream & Node::Print(ostream &os) const
{
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
    return os;
}
#endif


#if defined(DISTANCEVECTOR)

//////// WRITE THESE ////////

void Node::LinkHasBeenUpdated(const Link *l)
{
    // cerr << *this << ": Link Update: " << *l << endl;
    
    // update our table and send out routing message
    if (!lTable.info.ExistingLink(l))
    {
        RoutingMessage* m = new RoutingMessage(l);
        lTable.info.NewLink(l);
        lTable.UpdateAlgorithm();
        SendToNeighbors(m);
    }
}

void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
    LinkHasBeenUpdated(m->link); /////////////////// maybe change
}

void Node::TimeOut()
{
    cerr << *this << " got a timeout: ignored" << endl;
}


Node *Node::GetNextHop(const Node *destination) const
{
    unsigned next_node;
    
    next_node = lTable.GetNextHop_Table(destination->number);
    
    if (next_node != UNDEFINED)
    {
        return new Node(next_node, 0, 0, 0);
    }
    else
    {
        return 0;
    }
}

Table *Node::GetRoutingTable() const
{
    return new Table(lTable);
}

ostream & Node::Print(ostream &os) const
{
    os << "Node(number=" << number << ", lat=" << lat << ", bw=" << bw;
    return os;
}
#endif
