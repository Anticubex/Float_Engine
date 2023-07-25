#include <iostream>
#include <math/matrix.h>

int main(int argc, char *argv[]) {

        // Testing matrices
        using namespace std;

        // Strassen's Multiplication

        Matrix A(4, 4, vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});

        Matrix B(4, 4, vector<float>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});

        /* Result should be
                C1	C2	C3	C4
        1	90	100	110	120
        2	202	228	254	280
        3	314	356	398	440
        4	426	484	542	600
        */

        Matrix C = (A * B);

        cout << "\nResult:";

        C.printout();

        return 0;
}