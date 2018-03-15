#include <iostream>
#include <vector>
#include "generalPath.h"
#include "unLoadPath2.h"
#include <float.h>
#include <cmath>


unLoadPath2::unLoadPath2(std::vector<double> xdata, std::vector<double> ydata): generalPath(xdata, ydata)
{
    this->initial();
}

unLoadPath2::unLoadPath2(): generalPath() {}

unLoadPath2::~unLoadPath2() {}

unLoadPath2 unLoadPath2::unload(double y, double curE, double curRev)
{
    double Rev, tempx1;
    std::vector<double> tempxdata, tempydata;
    
    if (this->isLinear(this->curX) == false) {
        if (curE <= this->curY/this->curX)
        {
            curE = this->curY/this->curX;
        }


        if (std::abs(y) <= curRev)
            curRev = std::abs(y);


        if (this->curX > this->xdata[2]) {
            Rev = -1 * curRev;
        } else {
            Rev =  curRev;
        }
        tempx1 = this->curX - (this->curY - Rev) / curE;
        
        if (this->curX > this->xdata[2]) {
            tempxdata = {this->xdata[0], tempx1, this->curX, this->xdata.back()};
            tempydata = {-y, Rev, this->curY, this->ydata.back()};
        } else {
            tempxdata = {this->xdata[0], this->curX, tempx1, this->xdata.back()};
            tempydata = {this->ydata[0], this->curY, Rev, y};
        }
    }
    return unLoadPath2(tempxdata, tempydata);
}

unsigned int unLoadPath2::isLeave(double nextX)
{
    unsigned int out;
    if (this->isInRange(nextX)) {
        if (this->isLinear(this->curX)) {
            out = 0;
        } else {
            if (this->curX > this->xdata[2]) {
                if ((nextX - this->curX) > 0) {out = 0;} else {out = 22;}
            } else if (this->curX < this->xdata[1]) {
                if ((nextX - this->curX) < 0) {out = 0;} else {out = 22;}
            }
        }
    } else {
        out = 3;
    }
    return out;
}


void unLoadPath2::initial()
{
        this->linearRange.push_back(this->xdata[1]);
        this->linearRange.push_back(this->xdata[2]);
}
