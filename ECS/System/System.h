#ifndef __SYSTEM__H__
#define  __SYSTEM__H__

#include <iterator>
#include <set>

#include "../Entity.h"

class System
{
public:
    virtual void Update() = 0;

protected:
    std::set<Entity> m_Entities;

};

#endif // __SYSTEM__H__