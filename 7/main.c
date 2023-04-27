#include <stdio.h>
#include <stdlib.h>

typedef long long int ll_int;
/**
 *  Sales Event Heap
 */
typedef struct SalesEvent {
    ll_int price;
    int expired_day;
} SalesEvent;

typedef struct PriceSchedule {
    int length;
    int tail;
    SalesEvent* sales_event_heap;
} PriceSchedule;

/**
 *  Company Tree
 */
typedef struct Offspring {
    int index;
    struct Offspring* next;
} Offspring;

typedef struct Company {
    ll_int total_price;
    int total_number_of_melon;
    Offspring* offspring;
    PriceSchedule* price_schedule;
} Company;

/**
 *  Operations of Sales Event Heap
 */
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
        if (parent->price <= target->price)
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

/**
 *  Operations of Company Tree
 */
Company* initCompanyArray(int N, int M)
{
    Company* company_arr = (Company*)calloc(N, sizeof(Company));
    int upstream;
    company_arr[0].price_schedule = makePriceSchedule(M);
    for (int n = 1; n < N; n++) {
        scanf("%d", &upstream);
        upstream -= 1;
        company_arr[n].price_schedule = makePriceSchedule(M);
        Offspring* offspring = (Offspring*)calloc(1, sizeof(Offspring));
        offspring->next = company_arr[upstream].offspring;
        offspring->index = n;
        company_arr[upstream].offspring = offspring;
    }
    return company_arr;
}

void calculateNumberOfMelons(Company* company_arr, int company_idx)
{
    Company* target = &company_arr[company_idx];
    Offspring* offspring = target->offspring;
    while (offspring != NULL) {
        calculateNumberOfMelons(company_arr, offspring->index);
        target->total_number_of_melon += company_arr[offspring->index].total_number_of_melon;
        offspring = offspring->next;
    }
    target->total_number_of_melon += 1;
}

void updateCompany(Company* company_arr, int company_idx, int day)
{
    Company* target = &company_arr[company_idx];
    Offspring* offspring = target->offspring;
    target->total_price = 0;
    while (offspring != NULL) {
        updateCompany(company_arr, offspring->index, day);
        target->total_price += company_arr[offspring->index].total_price;
        offspring = offspring->next;
    }
    target->total_price += getPrice(target->price_schedule, day);
}

int purchase(int N, ll_int C, Company* company_arr)
{
    int max_number_of_melons = 0;
    Company* target;
    for (int n = 0; n < N; n++) {
        target = &company_arr[n];
        if (target->total_price <= C && target->total_number_of_melon > max_number_of_melons) {
            max_number_of_melons = target->total_number_of_melon;
        }
    }
    return max_number_of_melons;
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

        // Update company array
        updateCompany(company_arr, 0, m);

        // Purchase
        printf("%d\n", purchase(N, C, company_arr));
    }
}

int main()
{
    int N, M;
    ll_int C;
    scanf("%d %d %lld", &N, &M, &C);
    Company* company_arr = initCompanyArray(N, M);
    calculateNumberOfMelons(company_arr, 0);
    startPurchasePlan(N, M, C, company_arr);
}