#ifndef UNLOADPATH1_H
#define UNLOADPATH1_H

#include <iostream>
#include <vector>
#include "generalPath.h"
#include "unLoadPath2.h"

class unLoadPath1 : public generalPath
{
public:
    
    //Three Points
    unLoadPath1(std::vector<double> xdata, std::vector<double> ydata, int direction);
    unLoadPath1();
    ~unLoadPath1();
    
    unLoadPath2 unload(double y, double curE, double curRev);
    unsigned int isLeave(double nextX);
    
private:
    virtual void initial();
    int direction;//1 - low to high; -1 - high to low
    
};


#endif
