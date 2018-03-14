#include "triBackbone.h"
#include "generalPath.h"
#include <float.h>
#include <iostream>
#include <vector>

TriBackbone::TriBackbone(std::vector<double> xdata, std::vector<double> ydata) : generalPath(xdata, ydata)
{
    this->initial();
}

TriBackbone::TriBackbone() : generalPath(){};

TriBackbone::~TriBackbone() {}

unLoadPath1 TriBackbone::unload(double y, double curE, double curRev)
{
    double Rev, tempx1;
    std::vector<double> tempxdata, tempydata;
    int tempDirection;
    if (this->isLinear(this->curX) == false)
    {
        if (std::abs(this->curY) <= curRev)
            curRev = std::abs(y);

        if (this->curX > 0) {
            Rev = -1 * curRev;
            tempDirection = -1;
        } else {
            Rev = curRev;
            tempDirection = 1;
        }
        tempx1 = this->curX - (this->curX - Rev) / curE;

        if (this->curX > 0) {
            tempxdata = {-this->curX, tempx1, this->curX};
            tempydata = {-y, Rev, this->curY};
        } else {
            tempxdata = {this->curX, tempx1, -this->curX};
            tempydata = {this->curY, Rev, -y};
        }
    }
    
    return unLoadPath1(tempxdata, tempydata, tempDirection);
}

unsigned int TriBackbone::isLeave(double nextX)
{
    unsigned int out;
    if (this->isInRange(nextX)) {
        if (this->isLinear(this->curX)) {
            out = 0;
        } else {
            if (this->curX * (nextX - this->curX) < 0) {
                out = 1;
            } else {
                out = 0;
            }
        }
    } else {
        out = 0;
    }

    return out;
}

void TriBackbone::initial()
{
    this->linearRange.push_back(this->xdata[2]) ;
    this->linearRange.push_back(this->xdata[3]) ;
}

