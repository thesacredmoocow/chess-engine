




unsigned long long encodeBitboard(bool** b)
{
    unsigned long long value;
    for(int k = 0; k < 8; k++)
    {
        for(int a = 0; a < 8; a++)
        {
            value *= 2;
            value += b[k][a];
        }
    }
    return value;
}
