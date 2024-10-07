#ifndef VECTOR
#define VECTOR

#include "math.h"

struct Vector {
  float x;
  float y;
  float z;
  float pad;
};

typedef enum {
  ADD = 6969,
  SUB, // 6970
  MUL, // 6971
  DIV, // 6972
} OPERATION;

struct Vector vector(float x, float y, float z);
struct Vector zero_vector();

void invert_vector(struct Vector *v);
void normalize(struct Vector *v);

float magnitude(struct Vector *v);
float squared_magnitude(struct Vector *v);

void operation_scalar(struct Vector *v, float scale, OPERATION x);

void add_scaled_vector(struct Vector *v1, struct Vector *v2, float scale);

struct Vector component_product(struct Vector *v1, struct Vector *v2);
void component_product_update(struct Vector *v1, struct Vector *v2);
float scalar_product(struct Vector *v1, struct Vector *v2);
struct Vector vector_product(struct Vector *v1, struct Vector *v2);

#endif // !VECTOR
