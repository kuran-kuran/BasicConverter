#ifndef LINE_HPP
#define LINE_HPP

namespace dms
{
    class Line
    {
    public:
        Line( void );
        ~Line( void );
        void Set( int x0, int y0, int x1, int y1 );
        bool Get( int& x, int& y );
    private:
        Line( Line& line );
        Line& operator = ( Line &line );
        int X;
        int Y;
        int E;
        int Dx;
        int Dy;
        int Sx;
        int Sy;
        int Type;
        int Rest;
        int Count;
        int CountMax;
    };
};

#endif
