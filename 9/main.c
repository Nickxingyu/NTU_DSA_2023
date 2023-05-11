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

void z_algorithm(char* str, int* z_table, int len)
{
    int K, L = 0, R = 0;
    for (int i = 1; i < len; i++) {
        if (i > R) {
            L = i;
            R = i;
            while (R < len && str[R - L] == str[R])
                R += 1;
            z_table[i] = R - L;
            R -= 1;
        } else {
            K = i - L;
            if (z_table[K] < R - i + 1)
                z_table[i] = z_table[K];
            else {
                L = i;
                while (R < len && str[R - L] == str[R])
                    R += 1;
                z_table[i] = R - L;
                R -= 1;
            }
        }
    }
}

void reverseStr(char* str, int len)
{
    reverse(char, str, len);
}

void reverseIntArray(int* arr, int len)
{
    reverse(int, arr, len);
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
    int cnt = 0, reverse_cnt = 0, idx_l = 0, idx_r, flag = 0;
    char* record = (char*)calloc(N, sizeof(char));
    int* interval = (int*)calloc(N, sizeof(int));
    record[0] = (prefix_z_table[0] == M);
    for (int i = 1; i < N; i++) {
        reverse_cnt = postfix_z_table[i - 1] + prefix_z_table[i] - M;
        if (reverse_cnt < 0)
            continue;
        idx_l = i - postfix_z_table[i - 1];
        idx_r = idx_l + reverse_cnt;
        record[idx_l] = 1;
        record[idx_r] = 1;
        interval[idx_l] += 1;
        interval[idx_r] -= 1;
    }
    for (int i = 0; i <= N - M; i++) {
        flag += interval[i];
        cnt += ((record[i] != 0) || (flag > 0));
    }
    return cnt;
}