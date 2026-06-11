/* @file ctype.c */

int isprint(unsigned char in)
{
    int aRet;

    if (in >= '!' && in <= '~')
    {
        aRet = 0;
    }
    else
    {
        aRet = 1;
    }
}