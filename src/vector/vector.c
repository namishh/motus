#include "vector.h"
#include <math.h>

struct Vector vector(float x, float y, float z) {
  struct Vector v = {x, y, z};
  return v;
}

struct Vector zero_vector() { return vector(0, 0, 0); }

void invert_vector(struct Vector *v) {
  v->x = -v->x;
  v->y = -v->y;
  v->z = -v->z;
}

float magnitude(struct Vector *v) {
  return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

float squared_magnitude(struct Vector *v) {
  return v->x * v->x + v->y * v->y + v->z * v->z;
}

void normalize(struct Vector *v) {
  float mag = magnitude(v);
  if (mag > 0) {
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
  }
}

void operation_scalar(struct Vector *v, float factor, OPERATION op) {
  switch (op) {
  case ADD:
    v->x += factor;
    v->y += factor;
    v->z += factor;
    break;
  case SUB:
    v->x -= factor;
    v->y -= factor;
    v->z -= factor;
    break;
  case MUL:
    v->x *= factor;
    v->y *= factor;
    v->z *= factor;
    break;
  case DIV:
    if (factor != 0) {
      v->x /= factor;
      v->y /= factor;
      v->z /= factor;
    }
    break;
  }
}

void add_scaled_vector(struct Vector *v1, struct Vector *v2, float scale) {
  v1->x += v2->x * scale;
  v1->y += v2->y * scale;
  v1->z += v2->z * scale;
}

struct Vector component_product(struct Vector *v1, struct Vector *v2) {
  return vector(v1->x * v2->x, v1->y * v2->y, v1->z * v2->z);
}

void component_product_update(struct Vector *v1, struct Vector *v2) {
  v1->x *= v2->x;
  v1->y *= v2->y;
  v1->z *= v2->z;
}

float scalar_product(struct Vector *v1, struct Vector *v2) {
  return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

struct Vector vector_product(struct Vector *v1, struct Vector *v2) {
  return vector(v1->y * v2->z - v1->z * v2->y, v1->z * v2->x - v1->x * v2->z,
                v1->x * v2->y - v1->y * v2->x);
}
