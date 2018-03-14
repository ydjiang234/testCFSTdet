#include <iostream>
#include "generalPath.h"

using namespace std;

int main()
{
    vector<double> xdata, ydata;
    xdata = {1, 2, 3, 4};
    ydata = {1, 2, 2.5, 2.6};
    generalPath gp = generalPath(xdata, ydata);

    //cout<<gp.getE(0.1)<<endl;
    cout<<gp.getY(1)<<endl;
    cout<<gp.getY(1.5)<<endl;
    cout<<gp.getY(2)<<endl;
    cout<<gp.getY(2.5)<<endl;
    cout<<gp.getY(3)<<endl;
    cout<<gp.getY(3.5)<<endl;
    cout<<gp.getY(4)<<endl;
    //cout<<gp.getE(4.5)<<endl;
    return 0;
}
