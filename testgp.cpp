#include <iostream>
#include "generalPath.h"
#include "triBackbone.h"
#include "unLoadPath1.h"

using namespace std;

int main()
{
    vector<double> xdata, ydata;
    xdata = {-2, -1, 1, 2};
    ydata = {-1.1, -1, 1, 1.1};
    generalPath gp = generalPath(xdata, ydata);
    TriBackbone bb = TriBackbone(xdata, ydata);
    bb.back2Path(1.5);
    cout<<bb.isLinear(bb.curY)<<endl;
    unLoadPath1 UP1 = bb.unload(-1.2, 1.0, 0.1);


    //cout<<gp.getE(0.1)<<endl;
    //cout<<gp.getE(4.5)<<endl;
    return 0;
}
