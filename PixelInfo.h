#ifndef PixelInfoH
#define PixelInfoH

/////////////////////////////////////////////////////////////////////////////
struct TPixelInfo
{
    int x, y;
    int r, g, b;
    int h, s, v;
    COLORREF color;

    //-----------------------------------------------------------------------
    TPixelInfo(bool example=false)
    {
        if (example)
        {
            x = 123;
            y = 456;
            r = 0xAB;
            g = 0xCD;
            b = 0xEF;
            CalculateHsv();
        }
    }

    //-----------------------------------------------------------------------
    TPixelInfo(int mx, int my)
    {
        Sample(mx, my);
    }

    //-----------------------------------------------------------------------
    void Sample(int mx, int my)
    {
        x = mx;
        y = my;

        // Probe color under mouse
        HDC dcDesktop = GetDC(NULL);
        color = ::GetPixel(dcDesktop, x, y);
        ReleaseDC(NULL, dcDesktop);

        r = GetRValue(color);
        g = GetGValue(color);
        b = GetBValue(color);

        CalculateHsv();
    }


private:
    //-----------------------------------------------------------------------
    void CalculateHsv()
    // In:   r, g and b in [0,255]
    // Out:  h in [0,360]°, except when s == 0,
    //       then h is undefined (some value not in [0,360])
    //       s in [0,100]%
    //       v in [0,100]%
    {
        float h, s, v;

        // Map r, g and b to [0,1]
        float r = (float)this->r / 255;
        float g = (float)this->g / 255;
        float b = (float)this->b / 255;
        float max = Maximum(r, g, b);
        float min = Minimum(r, g, b);

        v = max;                    // This settles the value of v

        // Calculate saturation
        if (max == 0)
            s = 0;                  // Saturation is 0 if r, g and b all are 0
        else
            s = (max - min)/max;

        if (s == 0)                 // Achromatic color, we are done
            h = -1;

        else                        // Cromatic color, calculate hue
        {
            float delta = max - min;

            if (r == max)
                h = (g - b)/delta;      // Resulting color is between yellow and magenta

            else if (g == max)
                h = 2 + (b - r)/delta;  // Resulting color is between cyan and yellow

            else if (b == max)
                h = 4 + (r - g)/delta;  // Resulting color is between magenta and cyan

            h *= 60;                    // Convert hue to degrees
            if (h < 0)
                h += 360;               // Make sure h is non-negative
        }

        // Round to nearest integer
        this->h = (int)(h + 0.5);
        this->s = (int)((s * 100) + 0.5);
        this->v = (int)((v * 100) + 0.5);
    }

    //-----------------------------------------------------------------------
    inline float Minimum(float x1, float x2, float x3)
    {
        float xmin = x1 < x2 ? x1 : x2;
        return xmin < x3 ? xmin : x3;
    }

    //-----------------------------------------------------------------------
    inline float Maximum(float x1, float x2, float x3)
    {
        float xmax = x1 > x2 ? x1 : x2;
        return xmax > x3 ? xmax : x3;
    }

};

#endif
