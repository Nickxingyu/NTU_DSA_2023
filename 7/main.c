#include <stdio.h>
#include <stdlib.h>
typedef long long int ll_int;

typedef struct SalesEvent {
    ll_int price;
    int expired_day;
} SalesEvent;

typedef struct PriceSchedule {
    int length;
    int tail;
    SalesEvent* sales_event_heap;
} PriceSchedule;

typedef struct Company {
    ll_int total_price;
    int total_number_of_melon;
    int upstream;
    PriceSchedule* price_schedule;
} Company;

PriceSchedule* makePriceSchedule(int length)
{
    PriceSchedule* price_schedule = (PriceSchedule*)calloc(1, sizeof(PriceSchedule));
    SalesEvent* sales_event_heap = (SalesEvent*)calloc(length, sizeof(SalesEvent));
    price_schedule->length = length;
    price_schedule->sales_event_heap = sales_event_heap;
    return price_schedule;
}

void swapSalesEvent(SalesEvent* target1, SalesEvent* target2)
{
    int tmp_expired_day = target2->expired_day;
    ll_int tmp_price = target2->price;
    target2->expired_day = target1->expired_day;
    target2->price = target1->price;
    target1->expired_day = tmp_expired_day;
    target1->price = tmp_price;
}

void insert_PS(PriceSchedule* ps, ll_int price, int expired_day, int day)
{
    if (ps->tail == ps->length)
        return;

    int tmp = ps->tail;
    ps->tail += 1;
    SalesEvent* target = &(ps->sales_event_heap[tmp]);
    target->price = price;
    target->expired_day = expired_day;
    while (tmp > 0) {
        tmp = (tmp - 1) / 2;
        SalesEvent* parent = &(ps->sales_event_heap[tmp]);
        if (parent->expired_day >= day && parent->price <= target->price)
            break;
        swapSalesEvent(target, parent);
        target = parent;
    }
}

void pop_PS(PriceSchedule* ps)
{
    if (ps->tail == 0)
        return;
    int root = 0, tmp = ps->tail - 1, left = root * 2 + 1, right = root * 2 + 2, min;
    SalesEvent* heap = ps->sales_event_heap;
    ps->tail = ps->tail - 1;
    while (root < tmp) {
        min = tmp;
        if (left < tmp) {
            if (heap[left].price < heap[min].price)
                min = left;
        }
        if (right < tmp) {
            if (heap[right].price < heap[min].price)
                min = right;
        }
        if (min != tmp) {
            swapSalesEvent(&heap[min], &heap[root]);
            root = min;
            left = root * 2 + 1;
            right = root * 2 + 2;
        }
        if (min == tmp) {
            swapSalesEvent(&heap[min], &heap[root]);
            break;
        }
    }
}

int getPrice(PriceSchedule* ps, int day)
{
    while (ps->sales_event_heap[0].expired_day < day) {
        pop_PS(ps);
    }
    return ps->sales_event_heap->price;
}

Company* initCompanyArray(int N, int M)
{
    Company *company_arr = (Company*)calloc(N, sizeof(Company)), *target, *upstream;
    for (int n = 1; n < N; n++) {
        int tmp;
        target = &company_arr[n];
        scanf("%d", &tmp);
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

void purchase(int N, ll_int C, Company* company_arr, int day)
{
    ll_int price;
    int total_number_of_melons = 0;
    Company *target, *upstream;
    for (int n = N - 1; n >= 0; n--) {
        company_arr[n].total_price = 0;
    }
    for (int n = N - 1; n > 0; n--) {
        target = &company_arr[n];
        upstream = &company_arr[target->upstream];
        price = getPrice(target->price_schedule, day);
        target->total_price += price;
        upstream->total_price += target->total_price;
        if (target->total_price <= C && target->total_number_of_melon > total_number_of_melons)
            total_number_of_melons = target->total_number_of_melon;
    }
    target = &company_arr[0];
    price = getPrice(target->price_schedule, day);
    target->total_price += price;
    if (target->total_price <= C && target->total_number_of_melon > total_number_of_melons)
        total_number_of_melons = target->total_number_of_melon;
    printf("%d\n", total_number_of_melons);
}

void startPurchasePlan(int N, int M, ll_int C, Company* company_arr)
{
    int expired_day;
    ll_int price;
    for (int m = 0; m < M; m++) {

        // Update price schedule
        for (int n = 0; n < N; n++) {
            scanf("%lld %d", &price, &expired_day);
            expired_day += m;
            insert_PS(company_arr[n].price_schedule, price, expired_day, m);
        }

        // Purchase
        purchase(N, C, company_arr, m);
    }
}

int main()
{
    int N, M;
    ll_int C;
    scanf("%d %d %lld", &N, &M, &C);
    Company* company_arr = initCompanyArray(N, M);
    startPurchasePlan(N, M, C, company_arr);
}