#include <stdio.h>
#include <stdlib.h>
typedef long long int ll_int;

typedef struct SalesEvent {
    ll_int price;
    ll_int expired_day;
} SalesEvent;

typedef struct PriceSchedule {
    ll_int length;
    ll_int tail;
    SalesEvent* sales_event_heap;
} PriceSchedule;

typedef struct Company {
    ll_int total_price;
    ll_int total_number_of_melon;
    ll_int upstream;
    PriceSchedule* price_schedule;
} Company;

PriceSchedule* makePriceSchedule(ll_int length)
{
    PriceSchedule* price_schedule = (PriceSchedule*)calloc(1, sizeof(PriceSchedule));
    SalesEvent* sales_event_heap = (SalesEvent*)calloc(length, sizeof(SalesEvent));
    price_schedule->length = length;
    price_schedule->sales_event_heap = sales_event_heap;
    return price_schedule;
}

void swapSalesEvent(SalesEvent* target1, SalesEvent* target2)
{
    ll_int tmp_expired_day = target2->expired_day;
    ll_int tmp_price = target2->price;
    target2->expired_day = target1->expired_day;
    target2->price = target1->price;
    target1->expired_day = tmp_expired_day;
    target1->price = tmp_price;
}

void insert_PS(PriceSchedule* ps, ll_int price, ll_int expired_day, ll_int day)
{
    if (ps->tail == ps->length)
        return;

    ll_int tmp = ps->tail;
    ps->tail += 1;
    SalesEvent *target, *parent;
    while (tmp > 0) {
        target = &(ps->sales_event_heap[tmp]);
        parent = &(ps->sales_event_heap[(tmp - 1) / 2]);
        if (parent->expired_day >= day && parent->price <= target->price)
            break;
        swapSalesEvent(target, parent);
        target = parent;
        tmp = (tmp - 1) / 2;
    }
    target->expired_day = expired_day;
    target->price = price;
}

void pop_PS(PriceSchedule* ps)
{
    if (ps->tail == 0)
        return;
    ll_int root = 0, tmp = ps->tail - 1, left, right, min;
    SalesEvent* heap = ps->sales_event_heap;
    ps->tail = ps->tail - 1;
    while (root < tmp) {
        left = root * 2 + 1;
        right = root * 2 + 2;
        min = tmp;
        if (left < tmp) {
            if (heap[left].price < heap[min].price)
                min = left;
        }
        if (right < tmp) {
            if (heap[right].price < heap[min].price)
                min = right;
        }
        swapSalesEvent(&heap[min], &heap[root]);
        if (min != tmp)
            root = min;
        else
            break;
    }
}

ll_int getPrice(PriceSchedule* ps, ll_int day)
{
    while (ps->sales_event_heap->expired_day < day) {
        pop_PS(ps);
    }
    return ps->sales_event_heap->price;
}

Company* initCompanyArray(ll_int N, ll_int M)
{
    Company *company_arr = (Company*)calloc(N, sizeof(Company)), *target, *upstream;
    ll_int tmp;
    for (int n = 1; n < N; n++) {
        target = &company_arr[n];
        scanf("%lld", &tmp);
        target->total_number_of_melon = 1;
        target->upstream = tmp - 1;
        target->price_schedule = makePriceSchedule(M);
    }
    for (int n = N - 1; n > 0; n--) {
        target = &company_arr[n];
        upstream = &company_arr[target->upstream];
        upstream->total_number_of_melon += target->total_number_of_melon;
    }
    company_arr[0].upstream = -1;
    company_arr[0].total_number_of_melon += 1;
    company_arr[0].price_schedule = makePriceSchedule(M);
    return company_arr;
}

void purchase(ll_int N, ll_int C, Company* company_arr, ll_int day)
{
    ll_int price;
    ll_int total_number_of_melon = 0;
    Company *target, *upstream;
    for (int n = N - 1; n >= 0; n--) {
        company_arr[n].total_price = 0;
    }
    for (int n = N - 1; n >= 0; n--) {
        target = &company_arr[n];
        price = getPrice(target->price_schedule, day);
        target->total_price += price;
        if (target->total_price <= C && target->total_number_of_melon > total_number_of_melon)
            total_number_of_melon = target->total_number_of_melon;
        if (n == 0)
            break;
        upstream = &company_arr[target->upstream];
        upstream->total_price += target->total_price;
    }
    printf("%lld\n", total_number_of_melon);
}

void startPurchasePlan(ll_int N, ll_int M, ll_int C, Company* company_arr)
{
    ll_int expired_day;
    ll_int price;
    for (int m = 0; m < M; m++) {

        // Update price schedule
        for (int n = 0; n < N; n++) {
            scanf("%lld %lld", &price, &expired_day);
            expired_day += m;
            insert_PS(company_arr[n].price_schedule, price, expired_day, m);
        }

        // Purchase
        purchase(N, C, company_arr, m);
    }
}

int main()
{
    ll_int N, M;
    ll_int C;
    scanf("%lld %lld %lld", &N, &M, &C);
    Company* company_arr = initCompanyArray(N, M);
    startPurchasePlan(N, M, C, company_arr);
}