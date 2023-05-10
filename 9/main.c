#include <stdio.h>
#include <stdlib.h>
#define reverse(T, arr, len)            \
    {                                   \
        T tmp;                          \
        int idx_l = 0, idx_r = len - 1; \
        while (idx_l < idx_r) {         \
            tmp = arr[idx_l];           \
            arr[idx_l] = arr[idx_r];    \
            arr[idx_r] = tmp;           \
            idx_l += 1;                 \
            idx_r -= 1;                 \
        }                               \
    }

void reverseIntArray(int* arr, int len);
char* makePSP(int N, int M);
int* makePrefixZTable(char* str, int N, int M);
int* makePostfixZTable(char* str, int N, int M);
int calculateNumOfEffectiveMagicSpells(int* prefix_z_table, int* postfix_z_table, int N, int M);

int main()
{
    // N is the length of S, and M is the length of P.
    int N, M;
    scanf("%d %d", &N, &M);
    char* PSP = makePSP(N, M);
    int* prefix_z_table = makePrefixZTable(PSP, N, M);
    int* postfix_z_table = makePostfixZTable(PSP, N, M);
    printf("%d", calculateNumOfEffectiveMagicSpells(prefix_z_table, postfix_z_table, N, M));
}

char* makePSP(int N, int M)
{
    char* PSP = (char*)malloc((N + 2 * M + 2) * sizeof(char)); // P $ S $ P
    scanf("%s", PSP + M + 1);
    scanf("%s", PSP);
    for (int m = 0; m < M; m++) {
        PSP[M + N + 2 + m] = PSP[m];
    }
    PSP[M] = '$';
    PSP[M + 1 + N] = '$';
    return PSP;
}

void reverseStr(char* str, int len)
{
    reverse(char, str, len);
}

void reverseIntArray(int* arr, int len)
{
    reverse(int, arr, len);
}

void z_algorithm(char* str, int* z_table, int len)
{
    int interval_l = 1, interval_r = 1, idx_l = 0, idx_r = 1;
    for (idx_r = 1; idx_r < len; idx_r++) {
        if (idx_r < interval_r) {
            z_table[idx_r] = z_table[idx_r - interval_l];
            continue;
        }
        if (str[idx_r] != str[idx_l]) {
            z_table[idx_r] = 0;
            continue;
        }
        interval_l = idx_r;
        interval_r = idx_r;
        while (interval_r < len && str[interval_r] == str[idx_l]) {
            interval_r += 1;
            idx_l += 1;
        }
        z_table[idx_r] = idx_l;
        idx_l = 0;
    }
}

int* makePrefixZTable(char* str, int N, int M)
{
    int len = N + M + 1;
    int* z_table = (int*)calloc(len, sizeof(int));
    z_algorithm(str, z_table, len);
    return z_table + M + 1;
}

int* makePostfixZTable(char* str, int N, int M)
{
    int t_len = N + M + 1, len = N + 2 * M + 2;
    int* z_table = (int*)calloc(t_len, sizeof(int));
    reverseStr(str, len);
    z_algorithm(str, z_table, t_len);
    reverseStr(str, len);
    reverseIntArray(z_table, t_len);
    return z_table;
}

int calculateNumOfEffectiveMagicSpells(int* prefix_z_table, int* postfix_z_table, int N, int M)
{
    int cnt = 0, reverse_cnt = 0, idx = 0;
    int* record = (int*)calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) {
        if (prefix_z_table[i] == M)
            record[i] = 1;
        if (postfix_z_table[i] != 0 && i + 1 < N) {
            reverse_cnt = postfix_z_table[i] + prefix_z_table[i + 1] - M + 1;
            idx = i - postfix_z_table[i] + 1;
            while (reverse_cnt > 0 && idx <= i) {
                record[idx] = 1;
                idx += 1;
                reverse_cnt -= 1;
            }
        }
    }
    for (int i = 0; i <= N - M; i++) {
        cnt += record[i];
    }
    return cnt;
}