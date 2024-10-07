#include "./particle/particle.h"
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <math.h>

#define MAX_PARTICLES 1000
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Particle particles[MAX_PARTICLES];
int particleCount = 0;

float cameraDistance = 20.0f;
float cameraAngleX = 30.0f;
float cameraAngleY = 45.0f;
int lastMouseX = 0, lastMouseY = 0;
int isRotating = 0;
int isDragging = 0;

SDL_Window *window = NULL;
SDL_GLContext context;

void init_SDL_GL() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

  window = SDL_CreateWindow("Particle Simulation", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (!window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  context = SDL_GL_CreateContext(window);
  if (!context) {
    printf("OpenGL context could not be created! SDL Error: %s\n",
           SDL_GetError());
    exit(1);
  }

  SDL_GL_SetSwapInterval(1);
  glutInit(&(int){1}, (char *[]){""}); // Initialize GLUT

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT, 0.1,
                 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_DEPTH_TEST);
}

void render_grid() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_LINES);
  glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
  for (int i = -20; i <= 20; i++) {
    glVertex3f(i, 0, -20);
    glVertex3f(i, 0, 20);
    glVertex3f(-20, 0, i);
    glVertex3f(20, 0, i);
  }
  glEnd();

  glDisable(GL_BLEND);
}

void render_shadow(struct Particle *particle) {
  float height = particle->position.y;
  float shadowSize =
      0.3f * (1.0f + height / 10.0f); // Adjust shadow size based on height
  float alpha = fmaxf(
      0.1f,
      0.5f - height / 20.0f); // Adjust shadow transparency based on height

  glPushMatrix();
  glTranslatef(particle->position.x, 0.01, particle->position.z);
  glScalef(shadowSize, 0.1, shadowSize);
  glColor4f(0.2f, 0.2f, 0.2f, alpha);
  glutSolidSphere(1.0, 20, 20);
  glPopMatrix();
}

void render_particles() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (int i = 0; i < particleCount; i++) {
    render_shadow(&particles[i]);

    glPushMatrix();
    glTranslatef(particles[i].position.x, particles[i].position.y,
                 particles[i].position.z);
    glColor4f(1.0f, 0.0f, 0.0f, 0.8f);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();
  }

  glDisable(GL_BLEND);
}

void render_scene() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  // Apply camera transformations
  float camX = cameraDistance * sinf(cameraAngleY * M_PI / 180.0f) *
               cosf(cameraAngleX * M_PI / 180.0f);
  float camY = cameraDistance * sinf(cameraAngleX * M_PI / 180.0f);
  float camZ = cameraDistance * cosf(cameraAngleY * M_PI / 180.0f) *
               cosf(cameraAngleX * M_PI / 180.0f);
  gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  render_grid();
  render_particles();
}

void spawn_particle(float x, float y, float z) {
  if (particleCount < MAX_PARTICLES) {
    particles[particleCount].position = vector(x, y, z);
    particles[particleCount].velocity =
        vector((float)rand() / RAND_MAX * 4.0f - 2.0f,
               (float)rand() / RAND_MAX * 2.0f + 5.0f,
               (float)rand() / RAND_MAX * 4.0f - 2.0f);
    particles[particleCount].acceleration = vector(0.0f, -9.8f, 0.0f);
    particles[particleCount].damping = 0.99f;
    particles[particleCount].force_accum = zero_vector();
    particleCount++;
  }
}

void handle_mouse_motion(int x, int y) {
  int deltaX = x - lastMouseX;
  int deltaY = y - lastMouseY;

  if (isRotating) {
    cameraAngleY += deltaX * 0.5f;
    cameraAngleX += deltaY * 0.5f;

    if (cameraAngleX > 89.0f)
      cameraAngleX = 89.0f;
    if (cameraAngleX < -89.0f)
      cameraAngleX = -89.0f;
  }

  if (isDragging) {
    float worldX, worldY, worldZ;
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, (GLint)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX,
                 &posY, &posZ);

    worldX = (float)posX;
    worldY = (float)posY;
    worldZ = (float)posZ;

    spawn_particle(worldX, worldY, worldZ);
  }

  lastMouseX = x;
  lastMouseY = y;
}

int main(void) {
  init_SDL_GL();

  int quit = 0;
  SDL_Event e;
  Uint32 last_time = SDL_GetTicks();

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      case SDL_MOUSEBUTTONDOWN:
        if (e.button.button == SDL_BUTTON_MIDDLE) {
          isRotating = 1;
        } else if (e.button.button == SDL_BUTTON_LEFT) {
          isDragging = 1;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if (e.button.button == SDL_BUTTON_MIDDLE) {
          isRotating = 0;
        } else if (e.button.button == SDL_BUTTON_LEFT) {
          isDragging = 0;
        }
        break;
      case SDL_MOUSEMOTION:
        handle_mouse_motion(e.motion.x, e.motion.y);
        break;
      case SDL_MOUSEWHEEL:
        cameraDistance += e.wheel.y * -0.5f;
        if (cameraDistance < 5.0f)
          cameraDistance = 5.0f;
        if (cameraDistance > 50.0f)
          cameraDistance = 50.0f;
        break;
      }
    }

    Uint32 current_time = SDL_GetTicks();
    float duration = (current_time - last_time) / 1000.0f;
    last_time = current_time;

    for (int i = 0; i < particleCount; i++) {
      integrate(&particles[i], duration);

      // Bounce off the ground
      if (particles[i].position.y < 0.3f) {
        particles[i].position.y = 0.3f;
        particles[i].velocity.y = -particles[i].velocity.y * 0.8f;
      }

      // Remove particle if it goes too far
      if (magnitude(&particles[i].position) > 20.0f) {
        particles[i] = particles[particleCount - 1];
        particleCount--;
        i--;
      }
    }

    render_scene();
    SDL_GL_SwapWindow(window);

    SDL_Delay(16); // Cap at roughly 60 FPS
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
