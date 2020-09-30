#include <iostream>

#include "MtmVec.h"
#include "MtmMat.h"
#include "MtmMatSq.h"
#include "MtmMatTriag.h"
#include "Complex.h"


using namespace MtmMath;
using std::cout;
using std::endl;

void myAssert(bool flag) {
    if (not(flag)) {
        cout << "fail" <<endl;
    }
}


void operators() {
    MtmVec<int> v(3,2);v[2]=0;
    v=(-v) * 3 + v - 2* v ;
    myAssert (v[0]==-8 and v[1]==-8 and v[2]==0);
    MtmMatSq<int> mr(2,2);
    MtmMat<int> m(Dimensions(3,2),3);
    MtmMat<int> mr2=m*mr;
    myAssert (mr2[0][0]==12);
    MtmMatSq<int> m3(3);
    MtmMat<int> m4(Dimensions(3,3));
    MtmMat<int> m5=m3-m4;
    m4.transpose();
    m5=m3*m4;
}



int main() {

    operators(); //15 points

}

