#include "messages.h"


#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
    os << "RoutingMessage()";
    return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
    os << "RoutingMessage (message_link = " << *message_link << ")";
    return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{
    message_link = rhs.message_link;
}

RoutingMessage::RoutingMessage(const Link *l)
{
    this->message_link = l;
}

RoutingMessage &RoutingMessage::operator=(const RoutingMessage &rhs)
{
    return *(new(this) RoutingMessage(rhs));
}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
    return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

#endif


