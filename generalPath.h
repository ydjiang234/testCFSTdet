#ifndef GENERALPATH_h
#define GENERALPATH_h

#include <iostream>
#include <vector>

class generalPath
{
public:
    generalPath(std::vector<double> xdata, std::vector<double> ydata);
    generalPath();
    ~generalPath();

    virtual unsigned int isLeave(double nextX);// 0 - stay, 1 - new LP1, 21 - new LP2 from LP1, 22 - new LP2 from LP2 3 - back to BB
    double getE(double x);
    double getY(double x);
    void back2Path(double x);

    bool isLinear(double x);
    bool isInRange(double x);

    double curX, curY, curE;
    bool isOnPath;
    std::vector<double> linearRange;
    std::vector<double> xdata, ydata,Edata;
      
private:





    bool inRange(double x, double low, double high);
    double interp(double x, double x1, double x2, double y1, double y2);
    unsigned int indexLowBound(double x);
};

#endif
