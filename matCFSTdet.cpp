#include "matCFSTdet.h"
#include "triBackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include <iostream>
#include <vector>
#include <cmath>

matCFSTdet::matCFSTdet(double E, double f1, double f2, double b1, double b2, double revRatio, double Dfactor1, double Efactor1, double Rfactor1, double Dfactor2, double Efactor2, double Rfactor2)
{
    this->E_ini = E;
    this->f1 = f1;
    this->f2 = f2;
    this->b1 = b1;
    this->b2 = b2;
    this->revRatio = revRatio;
    this->Dfactor1 = Dfactor1;
    this->Efactor1 = Efactor1;
    this->Rfactor1 = Rfactor1;

    this->Dfactor2 = Dfactor2;
    this->Efactor2 = Efactor2;
    this->Rfactor2 = Rfactor2;

    this->initial();
}

matCFSTdet::matCFSTdet() {}

matCFSTdet::~matCFSTdet() {}

void matCFSTdet::nextStress(double strain)
{
    generalPath *curLP, *nextLP;
    double Dratio, Eratio, Rratio;
    double stressD;

    curLP = this->getCurLP();
    Dratio = this->DdetRatio(this->strainCum/this->Ystrain, this->Dfactor1, this->Dfactor2);
    Eratio = this->EdetRatio(this->strainCum/this->Ystrain, this->Efactor1, this->Efactor2);
    Rratio = this->RdetRatio(this->strainCum/this->Ystrain, this->Rfactor1, this->Rfactor2);

    this->nextCondition = curLP->isLeave(strain);
    if (this->nextCondition == 0) {
        //0 - stay on path
        nextLP = curLP;
    } else if (this->nextCondition == 1) {
        //1 - new LP1
        stressD = std::abs(this->BB_base.getY(this->strain) * Dratio);
        this->LP1_next = this->BB.unload(stressD, this->E_ini * Eratio, this->f1 * this->revRatio * Rratio);
        this->BB_next = this->BB_base.updateBackbone(Dratio);
        nextLP = &this->LP1_next;
    } else if (this->nextCondition == 21) {
        //21 - new LP2 from LP1
        stressD = std::abs(this->BB_base.getY(curLP->xdata.back()) * Dratio);
        this->LP2_next = this->LP1.unload(stressD, this->E_ini * Eratio, this->f1 * this->revRatio * Rratio);
        this->BB_next = this->BB_base.updateBackbone(Dratio);
        nextLP = &this->LP2_next;
    } else if (this->nextCondition == 22) {
        //22 - new LP2 from LP2
        stressD = std::abs(this->BB_base.getY(curLP->xdata.back()) * Dratio);
        this->LP2_next = this->LP2.unload(stressD, this->E_ini * Eratio, this->f1 * this->revRatio * Rratio);
        this->BB_next = this->BB_base.updateBackbone(Dratio);
        nextLP = &this->LP2_next;
    } else if (this->nextCondition == 3) {
        //3 - back to BB
        nextLP = &this->BB;
    }

    this->strain_next = strain;
    this->strainCum_next = this->strainCum + std::abs(this->strain_next - curLP->curX);

    this->stress_next = nextLP->getY(this->strain_next);
    this->E_next = nextLP->getE(this->strain_next);
}

void matCFSTdet::next()
{
    generalPath *curLP;
    curLP = this->getCurLP();

    if (this->nextCondition == 1) {
        //1 - new LP1
        this->LP1 = this->LP1_next;
        this->BB = this->BB_next;
        curLP = &this->LP1;
    } else if (this->nextCondition == 21) {
        //21 - new LP2 from LP1
        this->LP2 = this->LP2_next;
        this->BB = this->BB_next;
        curLP = &this->LP2;
    } else if (this->nextCondition == 22) {
        //22 - new LP2 from LP2
        this->LP2 = this->LP2_next;
        this->BB = this->BB_next;
        curLP = &this->LP2;
    } else if (this->nextCondition == 3) {
        //3 - back to BB
        curLP = &this->BB;
    }


    this->BB.isOnPath = false;
    this->LP1.isOnPath = false;
    this->LP2.isOnPath = false;
    this->strainCum = this->strainCum_next;
    curLP->back2Path(this->strain_next);

    this->getDataFromPath();
}

void matCFSTdet::revertToLast()
{
    this->E_next = this->E;
    this->strain_next = this->strain;
    this->stress_next = this->stress;
    this->nextCondition = 0;
    this->strainCum_next = this->strainCum;
}

void matCFSTdet::reset()
{
    this->initial();
}

void matCFSTdet::initial()
{
    this->E = this->E_ini;
    this->E_next = this->E;
    this->strain = 0;
    this->strain_next = 0;
    this->stress = 0;
    this->stress_next = 0;
    this->strainCum = 0;
    this->strainCum_next = 0;

    this->Ystrain = this->f1 / this->E_ini;
    this->nextCondition = 0;

    this->LP1 = unLoadPath1();
    this->LP2 = unLoadPath2();
    this->BB = TriBackbone();
    this->LP1_next = unLoadPath1();
    this->LP2_next = unLoadPath2();
    this->BB_next = TriBackbone();
    this->BB_base = TriBackbone();


    double tempx1, tempx2;
    double tempy1, tempy2;
    double x_ult, y_ult;
    tempx1 = this->f1 / this->E_ini;
    tempy1 = this->f1;
    tempx2 = tempx1 + (this->f2 - this->f1) / (this->b1 * this->E_ini);
    tempy2 = this->f2;

    if (this->b2 < 0)
    {
        x_ult = tempx2 - this->f2 / (this->b2 * this->E_ini);
        y_ult = 0;
    } else {
        x_ult = 1E5;
        y_ult = this->f2 + (x_ult - tempx2) * this->b2 * this->E_ini;
    }
    
    std::vector<double> xdata = {-x_ult, -tempx2, -tempx1, tempx1, tempx2, x_ult};
    std::vector<double> ydata = {-y_ult, -tempy2, -tempy1, tempy1, tempy2, y_ult};
    
    this->BB_base =TriBackbone(xdata, ydata);
    this->BB =TriBackbone(xdata, ydata);
    this->BB.back2Path(this->strain);
    this->getDataFromPath();

}

void matCFSTdet::getDataFromPath()
{
    generalPath *curLP = this->getCurLP();
    this->strain = curLP->curX;
    this->stress = curLP->curY;
    this->E = curLP->curE;
}

generalPath* matCFSTdet::getCurLP()
{
    generalPath *curLP;
    if (this->BB.isOnPath) {
        curLP = &this->BB;
    } else if (this->LP1.isOnPath) {
        curLP = &this->LP1;
    } else if (this->LP2.isOnPath) {
        curLP = &this->LP2;
    }
    return curLP;
}

double matCFSTdet::DdetRatio(double x, double factor1, double factor2, double lowerbound)
{
    double out;
    out = 1.0 - std::pow(factor1 * x, factor2);
    if (out < lowerbound) out = lowerbound;
    return out;
}

double matCFSTdet::EdetRatio(double x, double factor1, double factor2, double lowerbound)
{
    double out;
    out = 1.0 - std::pow(factor1 * x, factor2);
    if (out < lowerbound) out = lowerbound;
    return out;
}

double matCFSTdet::RdetRatio(double x, double factor1, double factor2, double lowerbound)
{
    double out;
    out = 1.0 - std::pow(factor1 * x, factor2);
    if (out < lowerbound) out = lowerbound;
    return out;
}
