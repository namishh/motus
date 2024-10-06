#include "vector.h"
#include <linux/limits.h>

struct Vector vector(float x, float y, float z) {
  struct Vector vect;
  vect.x = x;
  vect.y = y;
  vect.z = z;

  return vect;
}

struct Vector zero_vector() {
  struct Vector vect;
  vect.x = 0;
  vect.y = 0;
  vect.z = 0;

  return vect;
}

void invert_vector(struct Vector *v) {
  v->x = -v->x;
  v->y = -v->y;
  v->z = -v->z;
}

float magnitude(struct Vector *v) {
  return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

float squared_magnitude(struct Vector *v) {
  return v->x * v->x + v->y * v->y + v->z * v->z;
}

void normalize(struct Vector *v) {
  float mag = magnitude(v);

  if (mag > 0.0f) {
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
  }
}

void operation_scalar(struct Vector *v, int factor, OPERATION x) {
  if (x == 6969) {
    v->x += factor;
    v->y += factor;
    v->x += factor;
  } else if (x == 6970) {
    v->x -= factor;
    v->y -= factor;
    v->x -= factor;
  } else if (x == 6971) {
    v->x *= factor;
    v->y *= factor;
    v->x *= factor;
  } else if (x == 6972) {
    v->x /= factor;
    v->y /= factor;
    v->x /= factor;
  }
}

void add_scaled_vector(struct Vector *v1, struct Vector *v2, int factor) {
  v1->x = v2->x * factor;
  v1->y = v2->y * factor;
  v1->z = v2->z * factor;
}

struct Vector component_product(struct Vector *v1, struct Vector *v2) {
  struct Vector vect;
  vect.x = v1->x * v2->x;
  vect.y = v1->y * v2->y;
  vect.z = v1->z * v1->z;

  return vect;
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
  struct Vector vect;
  vect.x = v1->y * v2->z - v1->z * v2->y;
  vect.y = v1->z * v2->x - v1->x * v2->z;
  vect.z = v1->x * v2->y - v1->y * v2->x;

  return vect;
}
