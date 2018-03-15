#include <iostream>
#include <vector>
#include "generalPath.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include <float.h>
#include <cmath>


unLoadPath1::unLoadPath1(std::vector<double> xdata, std::vector<double> ydata, int direction) : generalPath(xdata, ydata)
{
    this->direction = direction;
    this->initial();
}

unLoadPath1::unLoadPath1() : generalPath() {}

unLoadPath1::~unLoadPath1() {}

unLoadPath2 unLoadPath1::unload(double y, double curE, double curRev)
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

        if (this->direction==1) {
            Rev = -1 * curRev;
        } else {
            Rev =  curRev;
        }
        tempx1 = this->curX - (this->curY - Rev) / curE;
        
        if (this->direction==1) {
            tempxdata = {this->xdata[0], tempx1, this->curX, this->xdata.back()};
            tempydata = {-y, Rev, this->curY, this->ydata.back()};
        } else {
            tempxdata = {this->xdata[0], this->curX, tempx1, this->xdata.back()};
            tempydata = {this->ydata[0], this->curY, Rev, y};
        }
    }
    //std::cout<<tempxdata[0]<<' '<<tempxdata[1]<<' '<<tempxdata[2]<<' '<<tempxdata[3]<<std::endl;
    //std::cout<<tempydata[0]<<' '<<tempydata[1]<<' '<<tempydata[2]<<' '<<tempydata[3]<<std::endl;
    return unLoadPath2(tempxdata, tempydata);
}

unsigned int unLoadPath1::isLeave(double nextX)
{
    unsigned int out;
    if (this->isInRange(nextX)) {
        if (this->isLinear(this->curX)) {
            out = 0;
        } else {
            if (this->direction * (nextX - this->curX) > 0) {
                out = 0;
            } else {
                out = 21;
            }
        }
    } else {
        out = 3;
    }
    return out;
}


void unLoadPath1::initial()
{
    if (this->direction==1) {
        this->linearRange.push_back(this->xdata[0]);
        this->linearRange.push_back(this->xdata[1]);
    } else {
        this->linearRange.push_back(this->xdata[1]);
        this->linearRange.push_back(this->xdata[2]);
    }
}
