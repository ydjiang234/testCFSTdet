#include "matCFSTdetana.h"

#include "matCFSTdet.h"
#include <iostream>
#include <vector>

matCFSTdetana::matCFSTdetana(double E, double f1, double f2, double b1, double b2, double revRatio,
                             double Dfactor1, double Efactor1, double Rfactor1,
                             double Dfactor2, double Efactor2, double Rfactor2):
matCFSTdet(E, f1, f2, b1, b2, revRatio,
        Dfactor1, Efactor1, Rfactor1,
           Dfactor2, Efactor2, Rfactor2) {}

matCFSTdetana::matCFSTdetana() {}

matCFSTdetana::~matCFSTdetana() {}

void matCFSTdetana::analysis(std::vector<double> loadingProtocol, double interval)
{
    this->reset();
    this->loadingProtocol = loadingProtocol;
    for (int i=0; i<this->loadingProtocol.size(); ++i) {
        this->analysisOneStep(this->loadingProtocol[i], interval);
    }
}

void matCFSTdetana::analysisOneStep(double target, double interval)
{
    if (this->strain <= target) {
        while (this->strain < target) {
            this->nextStress(this->strain + interval);
            this->next();
            this->Xresults.push_back(this->strain);
            this->Yresults.push_back(this->stress);
        }
    } else {
        while (this->strain > target) {
            this->nextStress(this->strain - interval);
            this->next();
            this->Xresults.push_back(this->strain);
            this->Yresults.push_back(this->stress);
        }
    }
}

void matCFSTdetana::reset()
{
    this->Xresults.clear();
    this->Yresults.clear();
    this->loadingProtocol.clear();
}
