#include "systembase.h"
#include "entity.h"

unsigned int SystemBase::ID = 0;

SystemBase::SystemBase()
{
    mSystemID = ID;
    ID++;
}

SystemBase::~SystemBase()
{

}

void SystemBase::init()
{

}

void SystemBase::draw()
{

}

void SystemBase::update()
{

}

unsigned int SystemBase::getSystemID() const
{
    return mSystemID;
}
