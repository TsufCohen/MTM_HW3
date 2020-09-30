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


void theDreadedTriagTest() {
    MtmMatTriag<int> m4(2,1,true);
    MtmMatTriag<int> m5(2,1,false);
    const MtmMatTriag<int>& m4_ref(m4);
    const MtmMatTriag<int>& m5_ref(m5);
    myAssert(1==m4_ref[0][1] and m4_ref[0][0]==1 and m4_ref[1][0]==0);
    myAssert(1==m5_ref[1][0] and m5_ref[0][1]==0 and m5_ref[0][0]==1);
    m4[0][1]=3;
    try {
        m4[1][0]=3;
        myAssert(false);
    }
    catch (MtmExceptions::AccessIllegalElement& e){}
    m4.transpose();
    m4[1][0]=3;
    try {
        m4[0][1]=3;
        myAssert(false);
    }
    catch (MtmExceptions::AccessIllegalElement& e){}
}


int main() {

    theDreadedTriagTest(); //10 points
}

