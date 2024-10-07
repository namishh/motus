#ifndef PARTICLE
#define PARTICLE

#include "../vector/vector.h"

struct Particle {
  struct Vector position;
  struct Vector velocity;
  struct Vector acceleration;
  struct Vector force_accum;

  float damping;
  float inverseMass;
};

void integrate(struct Particle *p, float duration);
#endif // !PARTICLE
