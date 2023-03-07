#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 256
#define Bigger 1
#define Smaller -1
#define Equal 0

typedef struct BigInt {
  int size;
  int8_t number[MAX_LENGTH];
} BigInt;

void freeBigInt(BigInt *bigInt) {
  memset(bigInt, 0, sizeof(BigInt));
  free(bigInt);
}

BigInt *make_BigInt(char *inputValue) {
  BigInt *bigInt = (BigInt *)calloc(1, sizeof(BigInt));
  bigInt->size = strnlen(inputValue, MAX_LENGTH);
  for (int i = bigInt->size - 1; i >= 0; i--) {
    bigInt->number[bigInt->size - 1 - i] = inputValue[i] - '0';
  }
  return bigInt;
}

int compare(BigInt *a, BigInt *b) {
  if (a->size > b->size) return Bigger;
  if (a->size < b->size) return Smaller;
  for (int head = a->size - 1; head >= 0; head--) {
    if (a->number[head] == b->number[head]) continue;
    if (a->number[head] > b->number[head])
      return Bigger;
    else
      return Smaller;
  }
  return Equal;
}

void multiplyByTwo(BigInt *bigInt) {
  int8_t *digit = bigInt->number;
  int8_t carry = 0;
  for (int i = 0; i < bigInt->size; i++) {
    digit[i] = (bigInt->number[i] << 1) + carry;
    carry = 0;
    if (digit[i] >= 10) {
      digit[i] -= 10;
      carry = 1;
    }
  }
  if (carry == 1) {
    bigInt->size += 1;
    bigInt->number[bigInt->size - 1] = 1;
  }
}

void subtract(BigInt *bigger, BigInt *smaller) {
  int8_t *digit = bigger->number;
  int8_t carry = 0;
  for (int i = 0; i < bigger->size; i++) {
    digit[i] = digit[i] - carry - smaller->number[i];
    carry = 0;
    if (digit[i] < 0) {
      digit[i] += 10;
      carry = 1;
    }
  }
  for (int i = MAX_LENGTH - 1; i >= 0; i--) {
    if (digit[i] != 0) {
      bigger->size = i + 1;
      break;
    }
    bigger->size = 1;
  }
}

void dividedByTwo(BigInt *bigInt) {
  int newSize = bigInt->size - 1 * (bigInt->number[bigInt->size - 1] == 1);
  int8_t carry = 0;
  for (int i = bigInt->size - 1; i >= 0; i--) {
    bigInt->number[i] += carry * 10;
    carry = bigInt->number[i] & 1;
    bigInt->number[i] = bigInt->number[i] >> 1;
  }
  bigInt->size = newSize;
}

char *BigIntToString(BigInt *bigInt) {
  char *str = (char *)calloc(MAX_LENGTH, sizeof(char));
  for (int i = bigInt->size - 1; i >= 0; i--) {
    str[bigInt->size - 1 - i] = bigInt->number[i] + '0';
  }
  return str;
}

void freeBigStr(char *bigStr) {
  memset(bigStr, 0, MAX_LENGTH);
  free(bigStr);
}

int isEven(BigInt *bigInt) { return !(bigInt->number[0] & 1); }

BigInt *binaryGCD(BigInt *a, BigInt *b) {
  BigInt *zero = make_BigInt("0");
  int ans = 0;
  BigInt *n, *m, *tmp;
  int bigger = compare(a, b);
  if (bigger == 0) {
    freeBigInt(zero);
    return a;
  }
  if (bigger > 0) {
    n = b;
    m = a;
  } else {
    n = a;
    m = b;
  }
  while (compare(n, zero) != 0 && compare(m, zero) != 0) {
    if (isEven(n) && isEven(m)) {
      ans += 1;
      dividedByTwo(n);
      dividedByTwo(m);
    } else if (isEven(n)) {
      dividedByTwo(n);
    } else if (isEven(m)) {
      dividedByTwo(m);
    }
    if (compare(n, m) > 0) {
      tmp = n;
      n = m;
      m = tmp;
    }
    subtract(m, n);
  }
  for (int i = 0; i < ans; i++) {
    multiplyByTwo(n);
  }
  freeBigInt(zero);
  return n;
}

int main() {
  char *buffer = (char *)malloc(MAX_LENGTH * sizeof(char));
  scanf("%s", buffer);
  BigInt *a = make_BigInt(buffer);
  scanf("%s", buffer);
  BigInt *b = make_BigInt(buffer);
  BigInt *result = binaryGCD(a, b);
  char *result_str = BigIntToString(result);
  printf("%s\n", result_str);
  freeBigInt(a);
  freeBigInt(b);
  freeBigStr(result_str);
  freeBigStr(buffer);
}