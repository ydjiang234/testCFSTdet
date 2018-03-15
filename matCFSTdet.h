#ifndef matCFSTdetet_H
#define matCFSTdetet_H
#include <iostream>
#include <vector>
#include "triBackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include "generalPath.h"

class matCFSTdet
{
public:
    matCFSTdet(double E, double f1, double f2, double b1, double b2, double revRatio, double Dfactor1, double Efactor1, double Rfactor1, double Dfactor2=1.0, double Efactor2=1.0, double Rfactor2=1.0);
    matCFSTdet();
    ~matCFSTdet();

    void nextStress(double strain_next);
    void next();
    void revertToLast();
    void reset();


    double E_ini, f1, f2, b1, b2, revRatio, Dfactor1, Efactor1, Rfactor1, Dfactor2, Efactor2, Rfactor2;
    double E, strain, stress;
    double E_next, strain_next, stress_next;

private:
    void initial();
    void getDataFromPath();
    unsigned int nextCondition;
    double DdetRatio(double x, double factor1, double factor2, double lowerbound=DBL_EPSILON);
    double EdetRatio(double x, double factor1, double factor2, double lowerbound=DBL_EPSILON);
    double RdetRatio(double x, double factor1, double factor2, double lowerbound=DBL_EPSILON);
    generalPath* getCurLP();

    TriBackbone BB, BB_next, BB_base;
    unLoadPath1 LP1, LP1_next;
    unLoadPath2 LP2, LP2_next;

    double strainCum, strainCum_next;
    double Ystrain;
};

#endif
