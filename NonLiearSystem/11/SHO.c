#include <stdio.h>
#include <windows.h>
#include "screen_int.f"
#include "pset.f"


void main()
{
    double x[3], x0[3];
    double t, dt, tmax;
    int ix, iy[3], nt, ntmax, nx, ny;
    double sigma = 10, R = 28, b = 2.666667;

    FILE *fp1;
    fp1=fopen("data.dat", "w");

    x0[0] = 7;
    x0[1] = 5;
    x0[2] = 10;

    dt = 0.001;
    tmax = 40.0;
    ntmax = tmax / dt+0.1;

    nx = 1000;
    ny = 1000;
    screen_int(nx, ny, RGB(255,255,255));


    for(nt=1; nt<=ntmax; nt++){
        // ローレンツ方程式による値の更新
        x[0] = x0[0] + dt * (-sigma * (x0[0] - x0[1]));
        x[1] = x0[1] + dt * (R * x0[0] - x0[1] - x0[0] * x0[2]);
        x[2] = x0[2] + dt * (x0[0] * x0[1] - b * x0[2]);
        t = dt * nt;

        fprintf(fp1, "%15.6e %15.6e %15.6e\n", t, x[0], x[1], x[2]);
        ix= t * nx / tmax;
        iy[0] = ny/2 - x[0] * 10;
        iy[1] = ny/2 - x[1] * 10;
        iy[2] = ny/2 - x[2] * 10;
        pset(ix, iy[0], RGB(255, 0, 0));
        pset(ix, iy[1], RGB(0, 255, 0));
        pset(ix, iy[2], RGB(0, 0, 255));

        x0[0] = x[0];
        x0[1] = x[1];
        x0[2] = x[2];
    }

    fclose(fp1);
}
