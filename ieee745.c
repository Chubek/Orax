#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "orax.h"

struct IEEE745_Float32 {
  uint32_t fraction : 23;
  uint32_t exponent : 8;
  uint32_t sign : 1;
};

struct IEEE745_Float64 {
  uint64_t fraction : 52;
  uint64_t exponent : 11;
  uint64_t sign : 1;
};

Float32 new_float32(uint32_t fraction, uint8_t exponent, sign_t sign) {
  Float32 f32 = {0};
  f32.fraction = fraction;
  f32.exponent = exponent;
  f32.sign = sign;
  return f32;
}

Float64 new_float64(uint64_t fraction, uint16_t exponent, sign_t sign) {
  Float64 f64 = {0};
  f64.fraction = fraction;
  f64.exponent = exponent;
  f64.sign = sign;
  return f64;
}

Float32 f32_addition(Float32 a, Float32 b) {
  Float32 result = {0};

  uint32_t mantissa_a = (a.fraction | FLOAT32_FRACTION_MASK) << 7;
  uint32_t mantissa_b = (b.fraction | FLOAT32_FRACTION_MASK) << 7;
  int exponent_a = a.exponent - FLOAT32_BIAS;
  int exponent_b = b.exponent - FLOAT32_BIAS;

  if (exponent_a < exponent_b) {
    mantissa_a >>= (exponent_b - exponent_a);
    result.exponent = b.exponent;
  } else {
    mantissa_b >>= (exponent_a - exponent_b);
    result.exponent = a.exponent;
  }

  if (a.sign == b.sign) {
    result.fraction = mantissa_a + mantissa_b;
    result.sign = a.sign;
  } else {
    if (mantissa_a > mantissa_b) {
      result.fraction = mantissa_a - mantissa_b;
      result.sign = a.sign;
    } else {
      result.fraction = mantissa_b - mantissa_a;
      result.sign = b.sign;
    }
  }

  if (result.fraction & 0x80000000) {
    result.fraction >>= 1;
    result.exponent++;
  }

  return result;
}

Float32 f32_subtraction(Float32 a, Float32 b) {
  b.sign = !b.sign;
  return f32_addition(a, b);
}

Float32 f32_multiplication(Float32 a, Float32 b) {
  Float32 result = {0};

  uint32_t mantissa_a = a.fraction | FLOAT32_FRACTION_MASK;
  uint32_t mantissa_b = b.fraction | FLOAT32_FRACTION_MASK;
  int exponent_a = a.exponent - FLOAT32_BIAS;
  int exponent_b = b.exponent - FLOAT32_BIAS;

  result.fraction = (uint32_t)(((uint64_t)mantissa_a * mantissa_b) >> 23);
  result.exponent = a.exponent + b.exponent - FLOAT32_BIAS;
  result.sign = a.sign ^ b.sign;

  return result;
}

Float32 f32_division(Float32 a, Float32 b) {
  Float32 result = {0};

  uint32_t mantissa_a = a.fraction | FLOAT32_FRACTION_MASK;
  uint32_t mantissa_b = b.fraction | FLOAT32_FRACTION_MASK;
  int exponent_a = a.exponent - FLOAT32_BIAS;
  int exponent_b = b.exponent - FLOAT32_BIAS;

  result.fraction = (uint32_t)(((uint64_t)mantissa_a << 23) / mantissa_b);
  result.exponent = a.exponent - b.exponent + FLOAT32_BIAS;
  result.sign = a.sign ^ b.sign;

  return result;
}

Float32 f32_exponentiation(Float32 a, Float32 b) {
  Float32 result = {0};
  result.fraction = (uint32_t)powf((float)a.fraction, (float)b.fraction);
  result.exponent = FLOAT32_BIAS;
  result.sign = a.sign ^ b.sign;
  return result;
}

Float64 f64_addition(Float64 a, Float64 b) {
  Float64 result = {0};

  uint64_t mantissa_a = (a.fraction | FLOAT64_FRACTION_MASK);
  uint64_t mantissa_b = (b.fraction | FLOAT64_FRACTION_MASK);
  int exponent_a = a.exponent - FLOAT64_BIAS;
  int exponent_b = b.exponent - FLOAT64_BIAS;

  if (exponent_a < exponent_b) {
    mantissa_a >>= (exponent_b - exponent_a);
    result.exponent = b.exponent;
  } else {
    mantissa_b >>= (exponent_a - exponent_b);
    result.exponent = a.exponent;
  }

  if (a.sign == b.sign) {
    result.fraction = mantissa_a + mantissa_b;
    result.sign = a.sign;
  } else {
    if (mantissa_a > mantissa_b) {
      result.fraction = mantissa_a - mantissa_b;
      result.sign = a.sign;
    } else {
      result.fraction = mantissa_b - mantissa_a;
      result.sign = b.sign;
    }
  }

  if (result.fractin & FLOAT64_FRACTION_MASK) {
    result.fraction >>= 1;
    result.exponent++;
  }

  return result;
}

Float64 f64_subtraction(Float64 a, Float64 b) {
  b.sign = !b.sign;
  return f64_addition(a, b);
}

Float64 f64_multiplication(Float64 a, Float64 b) {
  Float64 result = {0};

  uint64_t mantissa_a = a.fraction | FLOAT64_FRACTION_MASK;
  uint64_t mantissa_b = b.fraction | FLOAT64_FRACTION_MASK;
  int exponent_a = a.exponent - FLOAT64_BIAS;
  int exponent_b = b.exponent - FLOAT64_BIAS;

  result.fraction = (uint64_t)(((uint64_t)mantissa_a * mantissa_b) >> 52);
  result.exponent = a.exponent + b.exponent - FLOAT64_BIAS;
  result.sign = a.sign ^ b.sign;

  V return result;
}

Float64 f64_division(Float64 a, Float64 b) {
  Float64 result = {0};

  uint64_t mantissa_a = a.fraction | FLOAT64_FRACTION_MASK;
  uint64_t mantissa_b = b.fraction | FLOAT64_FRACTION_MASK;
  int exponent_a = a.exponent - FLOAT64_BIAS;
  int exponent_b = b.exponent - FLOAT64_BIAS;

  result.fraction = (uint64_t)(((uint64_t)mantissa_a << 52) / mantissa_b);
  result.exponent = a.exponent - b.exponent + FLOAT64_BIAS;
  result.sign = a.sign ^ b.sign;

  return result;
}

Float64 f64_exponentiation(Float64 a, Float64 b) {
  Float64 result = {0};
  result.fraction = (uint64_t)pow((double)a.fraction, (double)b.fraction);
  result.exponent = FLOAT64_BIAS;
  result.sign = a.sign ^ b.sign;
  return result;
}
o
