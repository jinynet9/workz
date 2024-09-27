#include "utility.h"
int g_Value = 0;
void setValue(int value)
{
    g_Value = value;
}
int getValue(void)
{
    return g_Value * 4;
}
