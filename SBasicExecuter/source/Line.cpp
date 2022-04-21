#include "Line.hpp"

namespace dms
{
    Line::Line( void )
    :X(0)
    ,Y(0)
    ,E(0)
    ,Dx(0)
    ,Dy(0)
    ,Sx(0)
    ,Sy(0)
    ,Type(0)
    ,Rest(0)
    ,Count(0)
    ,CountMax(-1)
    {
    }

    Line::~Line(void)
    {
    }

    void Line::Set(int x0, int y0, int x1, int y1)
    {
        if((( x0 - x1 ) == 0) && ((y0 - y1) == 0))
        {
            Count = 0;
            CountMax = -1;
            return;
        }
        Sx = (x1 > x0) ? 1 : -1;
        Dx = (x1 > x0) ? x1 - x0 : x0 - x1;
        Sy = (y1 > y0) ? 1 : -1;
        Dy = (y1 > y0) ? y1 - y0 : y0 - y1;
        X = x0;
        Y = y0;
        if(Dx >= Dy)
        {
            E = -Dx;
            Type = 0;
            CountMax = Dx;
        }
        else
        {
            E = -Dy;
            Type = 1;
            CountMax = Dy;
        }
        Count = 0;
    }

    bool Line::Get(int& x, int& y)
    {
        if(Count >= CountMax)
        {
            x = 0;
            y = 0;
            return true;
        }
        if(Type == 0)
        {
            X += Sx;
            E += 2 * Dy;
            if(E >= 0)
            {
                Y += Sy;
                E -= 2 * Dx;
            }
        }
        else
        {
            Y += Sy;
            E += 2 * Dx;
            if(E >= 0)
            {
                X += Sx;
                E -= 2 * Dy;
            }
        }
        x = X;
        y = Y;
        Count ++;
        return false;
    }
};
