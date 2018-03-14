#ifndef matCFSTd_H
#define matCFSTd_H
#include <iostream>
#include <vector>
#include "triBackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include "generalPath.h"

class matCFSTd
{
public:
    matCFSTd(double E, double f1, double f2, double b1, double b2, double revRatio, double Dfactor, double Efactor, double Rfactor);
    matCFSTd();
    ~matCFSTd();

    void nextStress(double strain_next);
    void next();
    void revertToLast();
    void reset();


    double E_ini, f1, f2, b1, b2, revRatio, Dfactor, Efactor, Rfactor;
    double E, strain, stress;
    double E_next, strain_next, stress_next;

private:
    void initial();
    void getDataFromPath();
    unsigned int nextCondition;
    double stressD(double strainCum, double stress);
    generalPath* getCurLP();

    TriBackbone BB, BB_next, BB_base;
    unLoadPath1 LP1, LP1_next;
    unLoadPath2 LP2, LP2_next;

    double strainCum, strainCum_next;
    double Ystrain;
    bool isDet, isDet_next;
    double EDet, RevRatioDet;
    void updateERevRatio();
};

#endif
