/*
 * Assignment 1: Bigint - Computer Programming 2 (COMP2711, COMP8801) 
 * Jonathan Souter - 2136593 
 */

#include <iostream>
#include <unistd.h>
#include "Bigint.h"

using namespace std;

// ============================================= //
// =============== MAIN FUNCTION =============== //
// ============================================= //

int main(int argc, char *argv[])
{ 
    Bigint n1, n2;
    char op;
    int opt, out_radix;

    while ((opt = getopt(argc, argv, "r:")) != -1)
    {
        switch (opt)
        {
        case 'r':
            out_radix = atoi(optarg);
            if (out_radix > 1 && out_radix < 37)
            {
                n1.set_radix(out_radix);
                n2.set_radix(out_radix);
            }
            else 
            {
                cout << argv[0] << " requires a radix between 2 and 36" << endl;
                exit(0);
            }
            break;
        }
    }

    while (cin >> n1 >> op >> n2)
    {
        switch (op)
        {
        case '+':
            cout << n1 + n2 << endl;
            break;
        case '-':
            cout << n1 - n2 << endl;
            break;
        case '*':
            cout << n1 * n2 << endl;
            break;
        case '/':
            cout << n1 / n2 << endl;
            break;
        }
        n1.clear();
        n2.clear();
    }

    return 0;
}
