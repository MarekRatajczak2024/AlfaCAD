/*   ______   ___       ___         ____     ______  ____
*   /\  _  \ /\_ \    /'___\       /\  _`\  /\  _  \/\  _`\
*   \ \ \L\ \\//\ \  /\ \__/   __  \ \ \/\_\\ \ \L\ \ \ \/\ \
*    \ \  __ \ \ \ \ \ \ ,__\/'__`\ \ \ \/_/_\ \  __ \ \ \ \ \
*     \ \ \/\ \ \_\ \_\ \ \_/\ \L\.\_\ \ \L\ \\ \ \/\ \ \ \_\ \
*      \ \_\ \_\/\____\\ \_\\ \__/.\_\\ \____/ \ \_\ \_\ \____/
*       \/_/\/_/\/____/ \/_/ \/__/\/_/ \/___/   \/_/\/_/\/___/
*
*   A CAD
*
*   By Marek Ratajczak 2024
*
*   See readme_alfacad.txt for copyright information.
*
*/

#include "forwin.h"
#include "bib_e.h"
#ifdef LINUX
#include <bits/stdc++.h>
#else
#include <vector>
#endif

#include "leak_detector_cpp.hpp"

#ifdef __cplusplus
extern "C" {
#endif

bool Check_ifConvex(QUAD *quad);

using namespace std;

// Utility function to find cross product
// of two vectors
float CrossProduct(vector<vector<float> >& A)
{
    // Stores coefficient of X
    // direction of vector A[1]A[0]
    float X1 = (A[1][0] - A[0][0]);

    // Stores coefficient of Y
    // direction of vector A[1]A[0]
    float Y1 = (A[1][1] - A[0][1]);

    // Stores coefficient of X
    // direction of vector A[2]A[0]
    float X2 = (A[2][0] - A[0][0]);

    // Stores coefficient of Y
    // direction of vector A[2]A[0]
    float Y2 = (A[2][1] - A[0][1]);

    // Return cross product
    return (X1 * Y2 - Y1 * X2);
}

// Function to check if the polygon is
// convex polygon or not
bool isConvex(vector<vector<float> >& points)
{
    // Stores count of
    // edges in polygon
    int N = (int)points.size();

    // Stores direction of cross product
    // of previous traversed edges
    float prev = 0.0;

    // Stores direction of cross product
    // of current traversed edges
    float curr = 0.0;

    // Traverse the array
    for (int i = 0; i < N; i++) {

        // Stores three adjacent edges
        // of the polygon
        vector<vector<float> > temp
                = { points[i],
                    points[(i + 1) % N],
                    points[(i + 2) % N] };

        // Update curr
        curr = CrossProduct(temp);

        // If curr is not equal to 0
        if (curr != 0) {

            // If direction of cross product of
            // all adjacent edges are not same
            if (curr * prev < 0) {
                return false;
            }
            else {
                // Update curr
                prev = curr;
            }
        }
    }
    return true;
}

bool Check_ifConvex(QUAD *quad)
{
    vector<vector<float> > points
            = { { quad->xy[0], quad->xy[1] }, { quad->xy[2], quad->xy[3] },
                { quad->xy[4], quad->xy[5] }, { quad->xy[6], quad->xy[7] } };

    if (isConvex(points)) {
        return true;
    }
    else {
        return false;
    }
}

#ifdef __cplusplus
}
#endif