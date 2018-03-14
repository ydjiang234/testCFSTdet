#include <iostream>
#include <vector>  
#include "generalPath.h"
#include "triBackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"

using namespace std;

int main()
{
    vector<double> xdata, ydata;
    xdata = {-2, -1, 1, 2};
    ydata = {-1.1, -1, 1, 1.1};
    generalPath gp = generalPath(xdata, ydata);
    TriBackbone bb = TriBackbone(xdata, ydata);
    bb.back2Path(1.5);
    unLoadPath1 UP1 = bb.unload(1, 1.0, 0.1);
    UP1.back2Path(-1.3);
    unLoadPath2 up2 = UP1.unload(1, 1.0, 0.1);
    up2.back2Path(-1.4);
    unLoadPath2 up3 = up2.unload(1.3, 1.0, 0.1);
    TriBackbone bb1 = bb.updateBackbone(0.5, -0.3);
    cout<<bb1.ydata[0]<<endl;
    cout<<bb1.ydata[1]<<endl;
    cout<<bb1.ydata[2]<<endl;
    cout<<bb1.ydata[3]<<endl;
    
    
    //cout<<gp.getE(0.1)<<endl;
    //cout<<gp.getE(4.5)<<endl;
    return 0;
}
