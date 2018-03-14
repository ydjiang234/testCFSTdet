#include "matCFSTd.h"
#include "triBackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include <iostream>
#include <vector>

matCFSTd::matCFSTd(double E, double f1, double f2, double b1, double b2, double revRatio, double Dfactor, double Efactor, double Rfactor)
{
    this->E_ini = E;
    this->f1 = f1;
    this->f2 = f2;
    this->b1 = b1;
    this->b2 = b2;
    this->revRatio = revRatio;
    this->Dfactor = Dfactor;
    this->Efactor = Efactor;
    this->Rfactor = Rfactor;

    this->isDet = false;
    this->isDet_next = false;
    
    this->E = this->E_ini;
    this->EDet = this->E_ini;
    this->RevRatioDet = this->revRatio;
    this->Ystrain = this->f1 / this->E_ini;

    this->E_next = this->E;
    this->strain = 0;
    this->strain_next = 0;
    this->stress = 0;
    this->stress_next = 0;
    this->nextCondition = 0;

    this->stressOri = 0;
    this->stressOri_next = 0;
    this->strainCum = 0;
    this->strainCum_next = 0;
    
    this->initial();
}

matCFSTd::matCFSTd() {}

matCFSTd::~matCFSTd() {}

void matCFSTd::nextStress(double strain)
{
    generalPath *curLP, *nextLP;
    curLP = this->getCurLP();

    this->nextCondition = curLP->isLeave(strain);
    if (this->nextCondition == 0) {
        //0 - stay on path
        nextLP = curLP;
    } else if (this->nextCondition == 1) {
        //1 - new LP1
        this->LP1_next = this->BB.unload(curLP->curX, curLP->curY, this->EDet, this->f1 * this->RevRatioDet);
        nextLP = &this->LP1_next;
    } else if (this->nextCondition == 21) {
        //21 - new LP2 from LP1
        this->LP2_next = this->LP1.unload(curLP->curX, curLP->curY, this->EDet, this->f1 * this->RevRatioDet);
        nextLP = &this->LP2_next;
    } else if (this->nextCondition == 22) {
        //22 - new LP2 from LP2
        this->LP2_next = this->LP2.unload(curLP->curX, curLP->curY, this->EDet, this->f1 * this->RevRatioDet);
        nextLP = &this->LP2_next;
    } else if (this->nextCondition == 3) {
        //3 - back to BB
        nextLP = &this->BB;
    }

    this->strain_next = strain;
    this->stressOri_next = nextLP->getY(strain);

    if (this->isDet == false) {
        if (nextLP != &this->BB) {
            this->isDet_next = true;
        }
    }

    if (this->isDet_next) {
        this->strainCum_next = this->strainCum + std::abs(this->strain_next - curLP->curX);
    } else {
        this->strainCum_next = 0;
    }

    this->stressOri_next = nextLP->getY(strain);
    this->stress_next = this->stressD(this->strainCum, this->stressOri_next);
    this->E_next = (this->strain_next - this->stress) / (this->strain_next - this->strain);
    //std::cout<<this->nextCondition<<std::endl;
    //std::cout<<(curLP==nextLP)<<std::endl;
    //std::cout<<curLP->xdata.size()<<std::endl;
    //std::cout<<nextLP->isInRange(strain)<<std::endl;
    //std::cout<<nextLP->getE(strain)<<std::endl;

    //std::cout<<this->strain_next<<std::endl;
    //std::cout<<this->stress_next<<std::endl;
    //std::cout<<this->E_next<<std::endl;
    //std::cout<<std::endl;
}

void matCFSTd::next()
{
    generalPath *curLP;
    if (this->BB.isOnPath) {
        curLP = &this->BB;
    } else if (this->LP1.isOnPath) {
        curLP = &this->LP1;
    } else if (this->LP2.isOnPath) {
        curLP = &this->LP2;
    }

    if (this->nextCondition == 1) {
        //1 - new LP1
        this->LP1 = this->LP1_next;
        curLP = &this->LP1;
    } else if (this->nextCondition == 21) {
        //21 - new LP2 from LP1
        this->LP2 = this->LP2_next;
        curLP = &this->LP2;
    } else if (this->nextCondition == 22) {
        //22 - new LP2 from LP2
        this->LP2 = this->LP2_next;
        curLP = &this->LP2;
    } else if (this->nextCondition == 3) {
        //3 - back to BB
        curLP = &this->BB;
    }


    this->BB.isOnPath = false;
    this->LP1.isOnPath = false;
    this->LP2.isOnPath = false;
    this->strainCum = this->strainCum_next;
    this->isDet = this->isDet_next;
    curLP->back2Path(this->strain_next);

    this->getDataFromPath();
    this->updateERevRatio();
}

void matCFSTd::revertToLast()
{
    this->E_next = this->E;
    this->strain_next = this->strain;
    this->stress_next = this->stress;
    this->nextCondition = 0;

    this->strainCum_next = this->strainCum;
    this->stressOri_next = this->stressOri;
    this->isDet_next = this->isDet;
}

void matCFSTd::reset()
{
    this->isDet = false;
    this->isDet_next = false;
    this->EDet = this->E_ini;
    this->RevRatioDet = this->revRatio;

    this->E = this->E_ini;
    this->Ystrain = this->f1 / this->E_ini;

    this->E_next = this->E;
    this->strain = 0;
    this->strain_next = 0;
    this->stress = 0;
    this->stress_next = 0;
    this->nextCondition = 0;

    this->stressOri = 0;
    this->stressOri_next = 0;
    this->strainCum = 0;
    this->strainCum_next = 0;
    this->LP1 = unLoadPath1();
    this->LP2 = unLoadPath2();
    this->BB = TriBackbone();
    this->initial();
}

void matCFSTd::initial()
{
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
    
    this->BB =TriBackbone(xdata, ydata);
    this->BB.back2Path(this->strain);
    this->getDataFromPath();

}

void matCFSTd::getDataFromPath()
{
    generalPath *curLP = this->getCurLP();
    this->strain = curLP->curX;
    this->stress = this->stressD(this->strainCum, curLP->curY);
    this->E = curLP->curE();
}

generalPath* matCFSTd::getCurLP()
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

double matCFSTd::stressD(double strainCum, double stress)
{
    double out, factor;
    factor = 1 - strainCum/ this->Ystrain * this->Dfactor;
    if (factor <= 0)
        factor = DBL_EPSILON;
    out = stress * factor;
    return out;
}

void matCFSTd::updateERevRatio()
{
    double Eratio, Rratio;
    Eratio = 1 - strainCum/ this->Ystrain * this->Efactor;
    Rratio = 1 - strainCum/ this->Ystrain * this->Rfactor;
    if (Eratio <= 0) {Eratio = DBL_EPSILON;}
    if (Rratio <= 0) {Rratio = DBL_EPSILON;}
    this->EDet = this->E_ini * Eratio;
    this->RevRatioDet = this->revRatio * Rratio;
}
