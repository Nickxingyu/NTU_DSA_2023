#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SLOT 100000007
#define H1 100000000000000007
#define MAX_LENGTH 1000000
#define ll long long int

char buffer[1000001];

typedef struct StringInfo {
    ll cnt;
    char* s;
} StringInfo;

typedef struct StringCode {
    int h1;
    int h2;
    char* s;
} StringCode;

typedef struct MultiSet {
    StringInfo* hash_table[SLOT];
    ll pairs_cnt;
    int string_len;
} MultiSet;
void init_ms(MultiSet** ms, char* s);
void insert_ms(MultiSet* ms, char* s);
void remove_ms(MultiSet* ms, char* s);
ll get_pairs_cnt(MultiSet* ms);

int main()
{
    int N, Q, op;
    MultiSet* ms = NULL;
    scanf("%d %d", &N, &Q);
    scanf("%s", buffer);
    init_ms(&ms, buffer);
    for (int i = 1; i < N; i++) {
        scanf("%s", buffer);
        insert_ms(ms, buffer);
    }
    printf("%lld\n", get_pairs_cnt(ms));
    for (int i = 0; i < Q; i++) {
        scanf(" %d %s", &op, buffer);
        if (op == 1)
            insert_ms(ms, buffer);
        else
            remove_ms(ms, buffer);
        printf("%lld\n", get_pairs_cnt(ms));
    }
}

struct Encoder {
    int d;
    int len;
    ll c1_d_n;
} encoder;
StringCode* encode(struct Encoder* encoder, char* s);
ll rotate_1(struct Encoder* encoder, ll code, char* s, int head);
int hash_1(ll c1);
int hash_2(ll c1);

StringInfo* make_string_info(int len, StringCode* sc)
{
    StringInfo* string_info = (StringInfo*)calloc(1, sizeof(StringInfo));
    string_info->cnt = 1;
    string_info->s = (char*)calloc(len, sizeof(char));
    strncpy(string_info->s, sc->s, len);

    return string_info;
}

int find(MultiSet* ms, StringCode* sc)
{
    int h1 = sc->h1, h2 = sc->h2;
    StringInfo* target;
    for (int step = 0; step < SLOT; step++) {
        target = ms->hash_table[h1];
        if (target == NULL)
            return h1;
        if (strncmp(sc->s, target->s, ms->string_len) == 0)
            return h1;
        h1 = (h1 + h2) % SLOT;
    }
    return -1;
}

void init_encoder(struct Encoder* e, int len)
{
    e->d = 26;
    e->c1_d_n = 1;
    e->len = len;
    for (int i = 1; i < e->len; i++) {
        e->c1_d_n = (e->c1_d_n * 26) % H1;
    }
}

void init_ms(MultiSet** ms, char* s)
{
    *ms = (MultiSet*)calloc(1, sizeof(MultiSet));
    (*ms)->string_len = strnlen(s, MAX_LENGTH + 1);
    init_encoder(&encoder, (*ms)->string_len);
    insert_ms(*ms, s);
}

void insert_ms(MultiSet* ms, char* s)
{
    StringCode* sc = encode(&encoder, s);
    int idx = find(ms, sc);
    if (idx == -1) {
        printf("Not Found !");
        return;
    }
    StringInfo* target = ms->hash_table[idx];
    if (target == NULL) {
        ms->hash_table[idx] = make_string_info(ms->string_len, sc);
    } else {
        ms->pairs_cnt += target->cnt;
        target->cnt += 1;
    }
}

void remove_ms(MultiSet* ms, char* s)
{
    StringCode* sc = encode(&encoder, s);
    int idx = find(ms, sc);
    if (idx == -1) {
        printf("Not Found !");
        return;
    }
    StringInfo* target = ms->hash_table[idx];
    if (target != NULL) {
        target->cnt -= 1;
        ms->pairs_cnt -= target->cnt;
    }
}

ll get_pairs_cnt(MultiSet* ms)
{
    return ms->pairs_cnt;
}

StringCode* encode(struct Encoder* encoder, char* s)
{
    int len = encoder->len, d = encoder->d, min_head = 0;
    ll c1 = 0, w1 = 1, min;
    StringCode* newCode = (StringCode*)calloc(1, sizeof(StringCode));
    newCode->s = (char*)calloc(len, sizeof(char));
    for (int i = 0; i < len; i++) {
        c1 = (c1 + (s[i] - 'a') * w1) % H1;
        w1 = (w1 * d) % H1;
    }
    min = c1;
    for (int i = 0; i < len; i++) {
        c1 = rotate_1(encoder, c1, s, i);
        if (c1 < min) {
            min = c1;
            min_head = i + 1;
        }
    }
    for (int i = 0; i < len; i++) {
        newCode->s[i] = s[(min_head + i) % len];
    }

    newCode->h1 = hash_1(min);
    newCode->h2 = hash_2(min);
    return newCode;
}

ll rotate_1(struct Encoder* encoder, ll code, char* s, int head)
{
    ll hash = code;
    hash = (hash + H1 - (s[head] - 'a')) % H1;
    hash /= encoder->d;
    hash = (hash + (s[head] - 'a') * encoder->c1_d_n) % H1;
    return hash;
}

int hash_1(ll c1)
{
    c1 %= SLOT;
    return (int)(c1 % SLOT);
}

int hash_2(ll c1)
{
    c1 %= SLOT - 1;
    return (int)(1 + (3 * c1) % (SLOT - 1));
}