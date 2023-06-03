#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SLOT 100000007
#define H1 10000000000000007
#define H2 100000000000000007
#define H3 1000000000000000007
#define MAX_LENGTH 1000000
#define ll long long int

char buffer[1000001];

typedef struct StringInfo {
    ll cnt;
    ll c1;
    ll c2;
    ll c3;
} StringInfo;

typedef struct StringCode {
    int h1;
    int h2;
    ll c1;
    ll c2;
    ll c3;
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
    ll c2_d_n;
    ll c3_d_n;
} encoder;
StringCode* encode(struct Encoder* encoder, char* s);
ll rotate_1(struct Encoder* encoder, ll code, char* s, int head);
ll rotate_2(struct Encoder* encoder, ll code, char* s, int head);
ll rotate_3(struct Encoder* encoder, ll code, char* s, int head);
int hash_1(ll c1, ll c2, ll c3);
int hash_2(ll c1, ll c2, ll c3);

StringInfo* make_string_info(int len, StringCode* sc)
{
    StringInfo* string_info = (StringInfo*)calloc(1, sizeof(StringInfo));
    string_info->cnt = 1;
    string_info->c1 = sc->c1;
    string_info->c2 = sc->c2;
    string_info->c3 = sc->c3;

    return string_info;
}

int find(MultiSet* ms, StringCode* sc)
{
    // printf("%lld %lld %d\n", sc->c1, sc->c2, sc->h1);
    int h1 = sc->h1, h2 = sc->h2;
    ll c1 = sc->c1, c2 = sc->c2, c3 = sc->c3;
    StringInfo* target;
    for (int step = 0; step < SLOT; step++) {
        target = ms->hash_table[h1];
        if (target == NULL)
            return h1;
        if (target->c1 == c1 && target->c2 == c2 && target->c3 == c3)
            return h1;
        h1 = (h1 + h2) % SLOT;
    }
    return -1;
}

void init_encoder(struct Encoder* e, int len)
{
    e->d = 26;
    e->c1_d_n = 1;
    e->c2_d_n = 1;
    e->c3_d_n = 1;
    e->len = len;
    for (int i = 1; i < e->len; i++) {
        e->c1_d_n = (e->c1_d_n * 26) % H1;
        e->c2_d_n = (e->c2_d_n * 26) % H2;
        e->c3_d_n = (e->c3_d_n * 26) % H2;
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
        // printf("%d\n", idx);
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
    int len = encoder->len, d = encoder->d;
    ll c1, c2, c3, w1 = 1, w2 = 1, w3 = 1;
    StringCode* newCode = (StringCode*)calloc(1, sizeof(StringCode));
    for (int i = 0; i < len; i++) {
        newCode->c1 = (newCode->c1 + (s[i] - 'a') * w1) % H1;
        newCode->c2 = (newCode->c2 + (s[i] - 'a') * w2) % H2;
        newCode->c3 = (newCode->c3 + (s[i] - 'a') * w3) % H3;
        w1 = (w1 * d) % H1;
        w2 = (w2 * d) % H2;
        w3 = (w3 * d) % H3;
    }
    c1 = newCode->c1;
    c2 = newCode->c2;
    c3 = newCode->c3;
    for (int i = 0; i < len; i++) {
        c1 = rotate_1(encoder, c1, s, i);
        c2 = rotate_2(encoder, c2, s, i);
        c3 = rotate_3(encoder, c3, s, i);
        if (c1 < newCode->c1) {
            newCode->c1 = c1;
            newCode->c2 = c2;
            newCode->c3 = c3;
        }
    }
    newCode->h1 = hash_1(newCode->c1, newCode->c2, newCode->c3);
    newCode->h2 = hash_2(newCode->c1, newCode->c2, newCode->c3);
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

ll rotate_2(struct Encoder* encoder, ll code, char* s, int head)
{
    ll hash = code;
    hash = (hash + H2 - (s[head] - 'a')) % H2;
    hash /= encoder->d;
    hash = (hash + (s[head] - 'a') * encoder->c2_d_n) % H2;
    return hash;
}

ll rotate_3(struct Encoder* encoder, ll code, char* s, int head)
{
    ll hash = code;
    hash = (hash + H3 - (s[head] - 'a')) % H3;
    hash /= encoder->d;
    hash = (hash + (s[head] - 'a') * encoder->c3_d_n) % H3;
    return hash;
}

int hash_1(ll c1, ll c2, ll c3)
{
    c1 %= SLOT;
    c2 %= SLOT;
    c3 %= SLOT;
    return (int)((c1 + c2 + c3) % SLOT);
}

int hash_2(ll c1, ll c2, ll c3)
{
    c1 %= SLOT - 1;
    c2 %= SLOT - 1;
    c3 %= SLOT - 1;
    return (int)(1 + (c1 + 3 * c2) % (SLOT - 1));
}