#ifndef SYMBOL_H
#define SYMBOL_H

typedef long long int LL;

struct Symbol
{
    LL frequency;
    double low_lim;
    double hi_lim;
    char symbol;
    int idx;

    Symbol(char s, int i, LL f)
    {
        frequency = f;
        low_lim = 0;
        hi_lim = 0;
        idx = i;
        symbol = s;
    }

    Symbol(char s, double low, double hi)
    {
        symbol = s;
        low_lim = low;
        hi_lim = hi;
        idx = 0;
        frequency = 0;
    }
};

#endif // SYMBOL_H
