//
// Created by Brett on 4/24/2018.
//

#ifndef PROGRAM3_TEXTURE_H
#define PROGRAM3_TEXTURE_H

#include <cstdlib>
#include <algorithm>
#include <cmath>




class Texture{
    int width;
    int height;
    unsigned seed;


public:

    void generate1DNoise(unsigned seed){
        srand(seed);
        for (unsigned i = 0; i < period; ++i) {
            texture[i] = ((float) rand() / (RAND_MAX));
            /*if(i == 0){
                cout << "i: " << i << " val: " << texture[i] << endl;
            }*/

        }
    }

    void generate2DNoise(unsigned seed){
        srand(seed);
        for (unsigned i = 0; i < periodSquared; ++i) {
            texture[i] = ((float) rand() / (RAND_MAX));
            /*if(i == 0){
                cout << "i: " << i << " val: " << texture[i] << endl;
            }*/

        }
    }


    float lerp(const float &lo, const float &hi, const float &t)
    { return lo * (1 - t) + hi * t; }

    float sample1DTexture(const float &x){
        int xFloor = (int)x;
        float t = x - xFloor;
        int xLo = xFloor % period;
        int xHi = (xLo + 1) % period;
        float smoothT = (1 - cos(t * M_PI)) * 0.5;
        return lerp(texture[xLo], texture[xHi], smoothT);
    }

    float sample2DTexture(const float &x, const float &y){
        int xFloor = abs((int)x);
        int yFloor = abs((int)y);

        float tx = x - xFloor;
        int xLo = xFloor % period;
        int xHi = (xLo + 1) % period;
        float smoothTX = (1 - cos(tx * M_PI)) * 0.5;

        float ty = y - yFloor;
        int yLo = yFloor % period;
        int yHi = (yLo + 1) % period;
        float smoothTY = (1 - cos(ty * M_PI)) * 0.5;

        // random values at the corners of the cell using permutation table
        float cor00 = texture[yLo * period + xLo];
        float cor10 = texture[yLo * period + xHi];
        float cor01 = texture[yHi * period + xLo];
        float cor11 = texture[yHi * period + xHi];


        // linearly interpolate values along the x axis
        float valX0 = lerp(cor00, cor10, smoothTX);
        float valX1 = lerp(cor01, cor11, smoothTX);

// linearly interpolate the nx0/nx1 along they y axis
        return lerp(valX0, valX1, smoothTY);
    }

    static const unsigned period = 64;
    static const int periodSquared = period * period;
    float texture[periodSquared];
};

#endif //PROGRAM3_TEXTURE_H


