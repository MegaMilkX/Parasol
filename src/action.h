#ifndef _ACTION_H_
#define _ACTION_H_

class Action
{
public:
    virtual void Execute() = 0;
    virtual Action* clone() = 0;
};

#endif