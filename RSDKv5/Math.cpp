#include "RetroEngine.hpp"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

int sinVal1024[0x400];
int cosVal1024[0x400];
int tanVal1024[0x400];
int aSinVal1024[0x400];
int aCosVal1024[0x400];

int sinVal512[0x200];
int cosVal512[0x200];
int tanVal512[0x200];
int aSinVal512[0x200];
int aCosVal512[0x200];

int sinVal256[0x100];
int cosVal256[0x100];
int tanVal256[0x100];
int aSinVal256[0x100];
int aCosVal256[0x100];

byte atanVal256[0x100 * 0x100];

uint randKey = 0;

void CalculateTrigAngles()
{
    randKey = rand();

    for (int i = 0; i < 0x400; ++i) {
        sinVal1024[i]  = (int)(sinf((i / 512.0) * M_PI) * 1024.0);
        cosVal1024[i]  = (int)(cosf((i / 512.0) * M_PI) * 1024.0);
        tanVal1024[i]  = (int)(tanf((i / 512.0) * M_PI) * 1024.0);
        aSinVal1024[i] = (int)((asin(i / 1023.0) * 512.0) / M_PI);
        aCosVal1024[i] = (int)((acos(i / 1023.0) * 512.0) / M_PI);
    }

    cosVal1024[0x000] = 0x400;
    cosVal1024[0x100] = 0;
    cosVal1024[0x200] = -0x400;
    cosVal1024[0x300] = 0;
    sinVal1024[0x000] = 0;
    sinVal1024[0x100] = 0x400;
    sinVal1024[0x200] = 0;
    sinVal1024[0x300] = -0x400;

    for (int i = 0; i < 0x200; ++i) {
        sinVal512[i]   = (int)(sinf((i / 256.0) * M_PI) * 512.0);
        cosVal512[i]   = (int)(cosf((i / 256.0) * M_PI) * 512.0);
        tanVal512[i]   = (int)(tanf((i / 256.0) * M_PI) * 512.0);
        aSinVal512[i]  = (int)((asin(i / 511.0) * 256.0) / M_PI);
        aCosVal512[i]  = (int)((acos(i / 511.0) * 256.0) / M_PI);
    }

    cosVal512[0x00]  = 0x200;
    cosVal512[0x80]  = 0;
    cosVal512[0x100] = -0x200;
    cosVal512[0x180] = 0;
    sinVal512[0x00]  = 0;
    sinVal512[0x80]  = 0x200;
    sinVal512[0x100] = 0;
    sinVal512[0x180] = -0x200;

    for (int i = 0; i < 0x100; i++) {
        sinVal256[i]  = (int)((sinVal512[i * 2] >> 1));
        cosVal256[i]  = (int)((cosVal512[i * 2] >> 1));
        tanVal256[i]  = (int)((tanVal512[i * 2] >> 1));
        aSinVal256[i] = (int)((asin(i / 255.0f) * 128.0f) / M_PI);
        aCosVal256[i] = (int)((acos(i / 255.0f) * 128.0f) / M_PI);
    }

    for (int y = 0; y < 0x100; ++y) {
        byte *arcTan = (byte *)&atanVal256[y];
        for (int x = 0; x < 0x100; ++x) {
            *arcTan = (byte)(atan2f(y, x) * 40.743664f);
            arcTan += 0x100;
        }
    }
}

byte ArcTanLookup(int X, int Y)
{
    int x = abs(X);
    int y = abs(Y);

    if (x <= y) {
        while (y > 0xFF) {
            x >>= 4;
            y >>= 4;
        }
    }
    else {
        while (x > 0xFF) {
            x >>= 4;
            y >>= 4;
        }
    }
    if (X <= 0) {
        if (Y <= 0)
            return atanVal256[(x << 8) + y] + -0x80;
        else
            return -0x80 - atanVal256[(x << 8) + y];
    }
    else if (Y <= 0)
        return -atanVal256[(x << 8) + y];
    else
        return atanVal256[(x << 8) + y];
}