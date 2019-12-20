using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class Percent
{
    private float val;

    public static implicit operator float(Percent p)
    {
        return p.val;
    }

    public Percent()
    {
        val = 0;
    }
    public Percent(float v)
    {
        if (v > 1)
        {
            v = 1;
        }
        if (v < 0)
        {
            v = 0;
        }
        val = v;
    }

    public static implicit operator Percent (float num)
    {
        if (num > 1) {
            num = 1;
        }
        if (num < 0) {
            num = 0;
        }
        return new Percent(num);
    }
    public static implicit operator Percent(int num)
    {
        float n = num / 100f;
        if (n > 1)
        {
            n = 1;
        }
        if (n < 0)
        {
            n = 0;
        }
        return new Percent(n);
    }
}
