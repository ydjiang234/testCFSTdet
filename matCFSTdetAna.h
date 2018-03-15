#ifndef MATCFSTdetANA_H
#define MATCFSTdetANA_H

#include "matCFSTdet.h"
#include <iostream>
#include <vector>


class matCFSTdetana : public matCFSTdet
{
public:
    matCFSTdetana(double E, double f1, double f2, double b1, double b2, double revRatio,
                  double Dfactor1, double Efactor1, double Rfactor1,
                  double Dfactor2, double Efactor2, double Rfactor2);
    matCFSTdetana();
    ~matCFSTdetana();

    void analysis(std::vector<double> loadingProtocol, double interval);

    std::vector<double> Xresults, Yresults;
    std::vector<double> loadingProtocol;

private:
    void analysisOneStep(double target, double interval);
    void reset();

};

#endif // MATCFSTdetANA_H
