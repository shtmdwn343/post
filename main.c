#define _CRT_SECURE_NO_WARNINGS
#include <curl/curl.h>
#include <time.h>
#include "case.h"
#include "hash.h"
#include "macro.h"

char url1[] = "service.epost.go.kr/trace.RetrieveDomRigiTraceList.comm?sid1=1068821878456";
char url2[] = "service.epost.go.kr/trace.RetrieveDomRigiTraceSolvList.comm?sid1=1068821878456&prm_sender_nm=%EC%84%A0%EA%B1%B0&prm_receiver_nm=%EA%B4%91%EC%A3%BC";
char url3[] = "trace.epost.go.kr/xtts/servlet/kpl.tts.common.svl.VisSVL?target_command=kpl.tts.tt.fmt.cmd.RetrieveCmsDetailInfoCMD&JspURI=/xtts/tt/epost/trace/sttfmt03p09.jsp&RegiNo=1068821878456&DelivYmd=20250530&EventPocd=40326";

u8 data1[131072];
u8 data2[131072];
u8 data3[128];

string buffer1 = { data1, 0 };
string buffer2 = { data2, 0 };
string buffer3 = { data3, 0 };

CURLM* curlm;
CURL* curl1;
CURL* curl2;
CURL* curl3;

// 등기 번호, 응답 시간, 배송 진행 상황, 받는 분, 수령인, 우체국, 집배원 성명, 집배원 전화번호
char data[1024] = "1068821878456,20250801:123456,";
FILE* fp;

u8* wp;
u8* rp;

u8* str;
u8 len;

bool p;

static inline void update_url1(u32 n)
{
    url1[73] = '0' + n % 10; n /= 10;
    url1[72] = '0' + n % 10; n /= 10;
    url1[71] = '0' + n % 10; n /= 10;
    url1[70] = '0' + n % 10; n /= 10;
    url1[69] = '0' + n % 10; n /= 10;
    url1[68] = '0' + n % 10;
    url1[67] = '0' + n / 10;
}

static inline char ntoh(const u8 n)
{
    return (n < 10) ? n + 48 : n + 55;
}

static size_t write_callback(char* contents, size_t size, size_t nmemb, string* userp)
{
    size_t total_size = size * nmemb;
    memcpy(userp->data + userp->size, contents, total_size);
    userp->size += total_size;
    return total_size;
}

static bool request(u32 rgist, u32 count)
{
    update_url1(rgist++);
    curl_easy_setopt(curl1, CURLOPT_URL, url1);

    buffer1.size = 0;
    guard (curl_easy_perform(curl1) == CURLE_OK);

    while (count--)
    {
        // 등기 번호
        cpy8(data + 5, url1 + 66);
        cpy8(url2 + 70, url1 + 66);
        cpy8(url3 + 172, url1 + 66);

        // 응답 시간
        {
            u32 t = time(NULL) - 86400 * 20300 + 3600 * 9;
            data[28] = '0' + t % 10; t /= 10;
            data[27] = '0' + t % 6; t /= 6;
            data[26] = '0' + t % 10; t /= 10;
            data[25] = '0' + t % 6; t /= 6;
            u8 n = t % 24; t /= 24;
            data[24] = '0' + n % 10;
            data[23] = '0' + n / 10;
            data[21] = '0' + t % 10;
            data[20] = '0' + t / 10;
        }

        update_url1(rgist++);
        curl_easy_setopt(curl1, CURLOPT_URL, url1);

        // Omit
        if (buffer1.size <= 102679)
        {
            data[30] = ',';
            data[31] = ',';
            data[32] = ',';
            data[33] = ',';
            data[34] = ',';
            data[35] = '\n';
            fwrite(data, 1, 36, fp);

            buffer1.size = 0;
            guard (curl_easy_perform(curl1) == CURLE_OK);

            continue;
        }

        // Update url3
        rp = buffer1.data + 85359;
        find ('<', rp, 3);

        rp += 62;
        find ('<', rp, 3);

        url3[195] = rp[9];
        url3[196] = rp[11];
        url3[197] = rp[12];

        // 배송 진행상황
        wp = data + 29;
        rp += 1675;

        p = true;
        do
        {
            switch (rp[41])
            {
                case 'd': case1(); break; // 접수, 접수취소, 배달준비, 미배달
                case ' ': case2(); break; // 발송, 도착
                case '<': case3(); break; // 배달준비, 배달완료
            }
        }
        while (p);

        curl_easy_setopt(curl3, CURLOPT_URL, url3);

        // Update url2
        rp = buffer1.data + 85334;

        url2[130] = ntoh(rp[0] & 15);
        url2[132] = ntoh(rp[1] >> 4);
        url2[133] = ntoh(rp[1] & 15);
        url2[135] = ntoh(rp[2] >> 4);
        url2[136] = ntoh(rp[2] & 15);

        u8* ast = hash_map1 + hash1(rp);
        if (!ast[0]) ast = hash_map2 + hash2(str, len);

        url2[139] = ntoh(ast[0] & 15);
        url2[141] = ntoh(ast[1] >> 4);
        url2[142] = ntoh(ast[1] & 15);
        url2[144] = ntoh(ast[2] >> 4);
        url2[145] = ntoh(ast[2] & 15);

        curl_easy_setopt(curl2, CURLOPT_URL, url2);

        // Request
        buffer1.size = 0;
        buffer2.size = 0;
        buffer3.size = 0;

        curl_multi_add_handle(curlm, curl1);
        curl_multi_add_handle(curlm, curl2);
        curl_multi_add_handle(curlm, curl3);

        int still_running;
        curl_multi_perform(curlm, &still_running);

        while (still_running) {
            int numfds;
            guard (curl_multi_poll(curlm, NULL, 0, 1000, &numfds) == CURLM_OK);
            curl_multi_perform(curlm, &still_running);
        }

        curl_multi_remove_handle(curlm, curl3);
        curl_multi_remove_handle(curlm, curl2);
        curl_multi_remove_handle(curlm, curl1);
        
        reject (buffer2.size <= 102570);

        // 받는 분
        rp = buffer2.data + 84939;
        cpy (wp, rp, '<');
        *wp++ = ',';

        // 수령인
        rp += 60;
        cpy (wp, rp, '<');
        *wp++ = ',';

        // 우체국
        rp = buffer3.data;
        cpy (wp, rp, '<');
        *wp++ = ',';

        // 집배원 성명
        rp += 22;
        cpy (wp, rp, '<');
        *wp++ = ',';

        // 집배원 전화번호
        rp += 22;
        cpy (wp, rp, '<');
        *wp = '\n';

        fwrite(data, 1, wp - data + 1, fp);
        system("cls");
        printf("%u", count);
    }
}

void main()
{
    curlm = curl_multi_init();
    curl1 = curl_easy_init();
    curl2 = curl_easy_init();
    curl3 = curl_easy_init();

    curl_easy_setopt(curl1, CURLOPT_WRITEDATA, &buffer1);
    curl_easy_setopt(curl2, CURLOPT_WRITEDATA, &buffer2);
    curl_easy_setopt(curl3, CURLOPT_WRITEDATA, &buffer3);

    curl_easy_setopt(curl1, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl3, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl3, CURLOPT_RANGE, "1481-1608");

    fp = fopen("output0.txt", "wb");
    request(1878388 + 262144 * 0, 262144); // 1068821878388 ~ 1068826173279
    fclose(fp);
}