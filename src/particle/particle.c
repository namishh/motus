#include "./particle.h"
#include <math.h>

void integrate(struct Particle *p, float duration) {
  struct Vector velocity_step = p->velocity;
  operation_scalar(&velocity_step, duration, MUL);
  add_scaled_vector(&p->position, &p->velocity, duration);

  add_scaled_vector(&p->velocity, &p->acceleration, duration);

  operation_scalar(&p->velocity, powf(p->damping, duration), MUL);

  p->force_accum = zero_vector();
}
