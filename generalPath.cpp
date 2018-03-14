#include "generalPath.h"
#include <iostream>
#include <vector>
#include <float.h>
#include <algorithm>

generalPath::generalPath(std::vector<double> xdata, std::vector<double> ydata)
{
    this->xdata = xdata;
    this->ydata = ydata;
    this->isOnPath = false;
    for (int i=1; i<xdata.size(); i++){
        this->Edata.push_back((this->ydata[i] - this->ydata[i-1]) / (this->xdata[i] - this->xdata[i-1]));
    }
}

generalPath::generalPath() {};

generalPath::~generalPath() {};

void generalPath::back2Path(double x)
{
    this->curX = x;
    this->curY = this->getY(x);
    this->curE = this->getE(x);
    this->isOnPath = true;
}

double generalPath::getY(double x)
{
    double out;
    unsigned int curId;
    if (this->isInRange(x))
    {
        curId = this->indexLowBound(x);
        out = this->interp(x, this->xdata[curId], this->xdata[curId+1], this->ydata[curId], this->ydata[curId+1]);
    } else {
        out = DBL_EPSILON;
    }
    
    return out;
}

double generalPath::getE(double x)
{
    double out;
    if (this->isInRange(x)) {
        unsigned int curId = this->indexLowBound(x);
        out = this->Edata[curId];
    } else {
        out = DBL_EPSILON;
    }

    return out;
}

unsigned int generalPath::isLeave(double nextX)
{
    return 0;
}

double generalPath::interp(double x, double x1, double x2, double y1, double y2)
{
    double k = (y2 - y1) / (x2 - x1);
    double out = y1 + (x - x1) * k;
    return out;
}

bool generalPath::isLinear(double x)
{
    double low, up;
    low = this->linearRange[0];
    up = this->linearRange[1];
    if (this->inRange(x, low, up))
        return true;
    else
        return false;
}

bool generalPath::isInRange(double x)
{
    return this->inRange(x, this->xdata[0], this->xdata.back());
}

bool generalPath::inRange(double x, double low, double high)
{
    if (x < low)
        return false;
    else if (x > high)
        return false;
    else
        return true;
}

unsigned int generalPath::indexLowBound(double x)
{
    unsigned int out;
    if (this->isInRange(x)) {
        int curId;
        std::vector<double>::iterator low;
        low=std::lower_bound(this->xdata.begin(), this->xdata.end(), x);
        curId = low - this->xdata.begin() - 1;
        if (curId < 0) curId = 0;
        out = curId;
    }
    return out;
}
