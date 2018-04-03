#include <elementAPI.h>
#include <Vector.h>
#include <Channel.h>
#include <math.h>
#include <float.h>
#include "matCFSTdet.h"
#include "CFSTdet.h"

#ifdef _USRDLL
#define OPS_Export extern "C" _declspec(dllexport)
#elif _MACOSX
#define OPS_Export extern "C" __attribute__((visibility("default")))
#else
#define OPS_Export extern "C"
#endif


static int numCFSTdet = 0;

OPS_Export void *OPS_CFSTdet()
{
    // print out some KUDO's
    if (numCFSTdet == 0) {
        opserr << "CFSTdet uniaxial material - Written by Yadong Jiang\n";
        numCFSTdet = 1;
    }

    // Pointer to a uniaxial material that will be returned
    UniaxialMaterial *theMaterial = 0;

    //
    // parse the input line for the material parameters
    //

    int    iData[1];
    double dData[12];
    int numData;
    numData = 1;
    if (OPS_GetIntInput(&numData, iData) != 0) {
        opserr << "WARNING invalid uniaxialMaterial CFSTdet tag" << endln;
        return 0;
    }

    numData = 12;
    if (OPS_GetDoubleInput(&numData, dData) != 0) {
        opserr << "WARNING invalid E & f1 & f2 & b1 & b2 & revRatio & Dfactor1 & Efactor1 & Rfactor1 & Dfactor2 & Efactor2 & Rfactor2\n";
        return 0;
    }

    //
    // create a new material
    //

    theMaterial = new CFSTdet(iData[0], dData[0], dData[1], dData[2], dData[3], dData[4], dData[5], dData[6], dData[7], dData[8], dData[9], dData[10], dData[11]);

    if (theMaterial == 0) {
        opserr << "WARNING could not create uniaxialMaterial of type CFSTdet!\n";
        return 0;
    }

    // return the material
    return theMaterial;
}


CFSTdet::CFSTdet(int tag, double E, double f1, double f2, double b1, double b2, double revRatio,
                 double Dfactor1, double Efactor1, double Rfactor1,
                 double Dfactor2, double Efactor2, double Rfactor2):UniaxialMaterial(tag, 0)
{
    this->curMat = matCFSTdet(E, f1, f2, b1, b2, revRatio,
                            Dfactor1, Efactor1, Rfactor1,
                            Dfactor2, Efactor2, Rfactor2);
}


CFSTdet::CFSTdet() :UniaxialMaterial(0, 0) {}

CFSTdet::~CFSTdet()
{
    //do nothing here
}

int CFSTdet::setTrialStrain(double strain, double strainRate)
{
    if (fabs(strain - this->curMat.strain) < DBL_EPSILON)
    {
        return 0;
    } else {
        this->curMat.nextStress(strain);
        return 0;
    }
}

double CFSTdet::getStrain(void)
{
    return this->curMat.strain_next;
}

double CFSTdet::getStress(void)
{
    return this->curMat.stress_next;
}

double CFSTdet::getTangent(void)
{
    return this->curMat.E_next;
}

double CFSTdet::getInitialTangent(void)
{
    return this->curMat.E_ini;
}

int CFSTdet::commitState(void)
{
    this->curMat.next();
    return 0;
}

int CFSTdet::revertToLastCommit(void)
{

    this->curMat.revertToLast();
    return 0;
}

int CFSTdet::revertToStart(void)
{
    this->curMat.reset();
    return 0;
}

UniaxialMaterial *CFSTdet::getCopy(void)
{
    CFSTdet *theCopy = new CFSTdet(this->getTag(), this->curMat.E_ini, this->curMat.f1, this->curMat.f2, this->curMat.b1, this->curMat.b2, this->curMat.revRatio,
                                   this->curMat.Dfactor1, this->curMat.Efactor1, this->curMat.Rfactor1,
                                   this->curMat.Dfactor2, this->curMat.Efactor2, this->curMat.Rfactor2);
    theCopy->curMat = this->curMat;
    //opserr << this->i << endln;
    return theCopy;
}

int CFSTdet::sendSelf(int commitTag, Channel &theChannel)
{
    int res = 0;
    /*
    static Vector data(2);
    data(0) = this->getTag();
    data(1) = this->curMat;
    res = theChannel.sendVector(this->getDbTag(), commitTag, data);
    if (res < 0)
        opserr << "CFSTdet::sendSelf() - failed to send data\n";
    */
    return res;
}
int CFSTdet::recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    int res = 0;
    /*
    static Vector data(2);
    res = theChannel.recvVector(this->getDbTag(), commitTag, data);
    if (res < 0) {
        opserr << "CFSTdet::recvSelf() - failed to receive data\n";
    }
    else {
        this->setTag(data(0));
        this->curMat = data(1);
    }
    */
    return res;
}

void CFSTdet::Print(OPS_Stream &s, int flag)
{
    s << "CFSTdet tag: " << this->getTag() << endln;
}



