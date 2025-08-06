#include "macro.h"

extern char url3[];

extern u8* wp;
extern u8* rp;

extern u8* str;
extern u8 len;

extern bool p;

void case1() // 접수, 접수취소, 배달준비, 미배달
{
    wp[1] = rp[49];
    wp[2] = rp[51];
    wp[3] = rp[52];
    wp[4] = ' ';

    wp[5] = rp[79];
    wp[6] = rp[80];
    wp[7] = rp[82];
    wp[8] = rp[83];
    wp[9] = ' ';
    wp += 10;

    rp += 159;
    cpylen(wp, rp, '\'', len);
    *wp++ = ' ';

    if (len == 6) // 접수
    {
        if (rp[89] == '<') // color:blue
        {
            rp += 114;
            cpy(wp, rp, '<');
            rp += 416;
        }
        else
        {
            rp += 89;
            cpy(wp, rp, '<');
            rp += 409;
        }
        if (*rp == '\n')
        {
            rp += 143;
        }
        else // 마감 후 접수(익일발송)
        {
            cpy(wp, rp, '\n');
            rp += 148;
        }
    }
    else if (len == 12)
    {
        if (rp[95] == '<') // color:blue
        {
            rp += 120;
            cpy(wp, rp, '<');
            rp += 73;
        }
        else
        {
            rp += 95;
            cpy(wp, rp, '<');
            rp += 66;
        }
        if (*rp == '\t') // 접수취소
        {
            rp += 472;
        }
        else // 배달준비
        {
            cpy(wp, rp, '\n');
            rp += 739;
        }
    }
    else // 미배달
    {
        if (rp[92] == '<') // color:blue
        {
            rp += 117;
            cpy(wp, rp, '<');
            rp += 410;
        }
        else
        {
            rp += 92;
            cpy(wp, rp, '<');
            rp += 403;
        }
        *wp++ = ' ';

        if (*rp == '-') // 오도착전송등록
        {
            rp += 193;
            cpy(wp, rp, '\n');
            rp += 501;
        }
        else // 기타
        {
            cpy(wp, rp, '\n');
            rp += 224;
        }
    }
    wp[0] = ' ';
}

void case2() // 발송, 도착
{
    do
    {
        wp[1] = rp[56];
        wp[2] = rp[58];
        wp[3] = rp[59];
        wp[4] = ' ';

        wp[5] = rp[88];
        wp[6] = rp[89];
        wp[7] = rp[91];
        wp[8] = rp[92];
        wp[9] = ' ';
        wp += 10;

        rp += 170;
        cpy(wp, rp, '\'');
        *wp++ = ' ';

        if (rp[89] == '<') // color:blue
        {
            rp += 114;
            cpy(wp, rp, '<');
            rp += 171;
        }
        else
        {
            rp += 89;
            cpy(wp, rp, '<');
            rp += 164;
        }
        wp[0] = ' ';
    } while (rp[55] == '0');
    rp += 9;
}

void case3() // 배달준비, 배달완료
{
    wp[1] = rp[37];
    wp[2] = rp[39];
    wp[3] = rp[40];
    wp[4] = ' ';

    wp[5] = rp[64];
    wp[6] = rp[65];
    wp[7] = rp[67];
    wp[8] = rp[68];
    wp[9] = ' ';
    wp += 10;

    if (rp[88] == '<') // 배달준비
    {
        rp += 141;
        cpy(wp, rp, '\'');
        *wp++ = ' ';

        if (rp[95] == '<') // color:blue
        {
            rp += 120;
            cpy(wp, rp, '<', len);
            rp += 37;
        }
        else
        {
            rp += 95;
            cpy(wp, rp, '<', len);
            rp += 30;
        }
        *wp++ = ' ';

        cpy(wp, rp, '\n');

        rp += 333;
        wp[0] = ' ';
    }
    else // 배달완료
    {
        // Update url3
        url3[209] = rp[135];
        url3[210] = rp[136];
        url3[211] = rp[137];
        url3[212] = rp[138];
        url3[213] = rp[139];

        rp += 143;
        cpy(wp, rp, '\'');
        *wp++ = ' ';

        rp += 95;
        str = wp;
        cpylen(wp, rp, '<', len);
        *wp++ = ',';

        p = false;
    }
}