#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <shaders/shaders.h>
#include <texture/textures.h>
#include <transform/matrix.h>
#include <transform/vector3.h>
#include <transform/TRS.h>
#include <camera/camera.h>
#include <objects/object.h>
#include <objects/shapes.h>
#include <objects/light.h>
#include <objects/material.h>
#include <physics/physics.h>

#define true 1
#define false 0

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);

unsigned int const SCR_WIDTH = 1920;
unsigned int const SCR_HEIGHT = 1080;

Vector3 const worldUp = {0, 1, 0};
Vector3 cameraPosition = {0, 150, 0};
Vector3 cameraFront = {0, -1.0, 0};

int firstMouse = true;
double yaw   = -90.0;
double pitch =  0.0;
double lastX =  SCR_WIDTH / 2.0;
double lastY =  SCR_HEIGHT / 2.0;
double fov   =  45.0;

double deltaTime = 0.0;
double lastFrame = 0.0;

PhysicsObject allCelestial[3];

GLFWwindow * setupOpenglContexWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OPEN_GL", NULL, NULL);
  if (window == NULL)
  {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);

  //Resize
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  //Mouse input
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  //Vsync
  glfwSwapInterval(1);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    printf("Failed to initialize GLAD\n");
    return NULL;
  }    

  glEnable(GL_DEPTH_TEST);

  return window;
};

int main() {
  GLFWwindow * window = setupOpenglContexWindow();
  if (window == NULL)
    return -1;

  Shaders shadersPlanet = cgShadersLoad("include/shaders/vertex.glsl", "include/shaders/fragment_planet.glsl");
  Shaders shadersMoon = cgShadersLoad("include/shaders/vertex.glsl", "include/shaders/fragment_moon.glsl");
  Shaders shadersLight = cgShadersLoad("include/shaders/vertex.glsl", "include/shaders/fragment_sun.glsl");

  cgTextureInit();
  Texture textureEarth = cgTextureLoad("assets/2k_earth_daymap.jpg", false);
  Texture textureSun = cgTextureLoad("assets/2k_sun.jpg", false);
  cgShadersUniformSetInt(&shadersPlanet, "texture0Data", 0);
  cgShadersUniformSetInt(&shadersLight, "texture0Data", 0);

  Vector3 cameraTarget = cgVector3Add(&cameraPosition, &cameraFront);
  Camera camera = cgCameraCreate(&cameraPosition, &cameraTarget, &worldUp);

  Matrix4 projection = cgMatrixPerspective(45.0, (double)SCR_WIDTH/(double)SCR_HEIGHT, 0.1, 4000.0);
  cgShadersUniformSetMatrix(&shadersPlanet, "projection", &projection);
  cgShadersUniformSetMatrix(&shadersMoon, "projection", &projection);
  cgShadersUniformSetMatrix(&shadersLight, "projection", &projection);

  Vector3 pos0 = {0.0, 0.0, 0.0};
  Vector3 speed0 = {0.0, 0.0, 0.0};
  Light lightSource = {
    .color = {0.95, 1.0, 0.8},
    .position = pos0,
    .radius = 25.0
  };
  allCelestial[0] = cgPhysicsCreateLight(25.0, 10000000000.0, &pos0, &speed0, &lightSource);

  Vector3 pos1 = {1000.0, 9.0, 0.0};
  Vector3 speed1 = {0.0, 0.0, -20.0};
  Material material1 = {
    .ambient = {0.1f, 0.1f, 0.1f},
    .diffuse = {1.0f, 1.0f, 1.0f},
    .specular = {0.1f, 0.1f, 0.1f},
    .shininess = 1.0f
  };
  allCelestial[1] = cgPhysicsCreateMaterial(1.0, 1000.0, &pos1, &speed1, &material1);
  cgShadersUniformSetMaterial(&shadersPlanet, "material", &material1);

  Vector3 pos2 = {1002.0, 5.0, 0.0};
  Vector3 speed2 = {0.0, 0.0, -20.0};
  Material material2 = {
    .ambient = {0.1f, 0.1f, 0.1f},
    .diffuse = {0.8f, 0.8f, 0.8f},
    .specular = {0.3f, 0.3f, 0.3f},
    .shininess = 8
  };
  allCelestial[2] = cgPhysicsCreateMaterial(0.5, 1.0, &pos2, &speed2, &material2);
  cgShadersUniformSetMaterial(&shadersMoon, "material", &material2);

  //MAIN LOOP
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    double currFrame = glfwGetTime();
    deltaTime = currFrame - lastFrame;
    lastFrame = currFrame;

    processInput(window);

    cameraTarget = cgVector3Add(&cameraPosition, &cameraFront);
    cgCameraUpdate(&camera, &cameraPosition, &cameraTarget, &worldUp);
    Matrix4 view = cgCameraLookAtMatrix(&camera);
    cgShadersUniformSetMatrix(&shadersPlanet, "view", &view);
    cgShadersUniformSetMatrix(&shadersMoon, "view", &view);
    cgShadersUniformSetMatrix(&shadersLight, "view", &view);
    cgShadersUniformSetVector3(&shadersPlanet, "viewPos", &cameraPosition);
    cgShadersUniformSetVector3(&shadersMoon, "viewPos", &cameraPosition);

    cgPhysicsUpdateForces(allCelestial, 3);
    cgPhysicsUpdatePosition(allCelestial, 3, deltaTime);
    allCelestial[0].type.light.position = allCelestial[0].obj.transform.postion;
    cgShadersUniformSetLight(&shadersPlanet, "light", &(allCelestial[0].type.light));
    cgShadersUniformSetLight(&shadersMoon, "light", &(allCelestial[0].type.light));

    cgShadersUse(&shadersLight);
    Matrix4 model = cgTransformGetMatrix(&(allCelestial[0].obj.transform));
    cgShadersUniformSetMatrix(&shadersLight, "model", &model);
    cgTextureBind(&textureSun, GL_TEXTURE_2D, GL_TEXTURE0);
    cgObjectDraw(&(allCelestial[0].obj));

    cgShadersUse(&shadersPlanet);
    model = cgTransformGetMatrix(&(allCelestial[1].obj.transform));
    cgShadersUniformSetMatrix(&shadersPlanet, "model", &model);
    cgTextureBind(&textureEarth, GL_TEXTURE_2D, GL_TEXTURE0);
    cgObjectDraw(&(allCelestial[1].obj));

    cgShadersUse(&shadersMoon);
    model = cgTransformGetMatrix(&(allCelestial[2].obj.transform));
    cgShadersUniformSetMatrix(&shadersMoon, "model", &model);
    cgObjectDraw(&(allCelestial[2].obj));

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  //Free resources
  cgShadersFree(&shadersPlanet);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);

  double cameraSpeed = 100.0 * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Vector3 movement = cgVector3ScalarProduct(cameraSpeed, &cameraFront);
    cameraPosition = cgVector3Add(&cameraPosition, &movement);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Vector3 movement = cgVector3ScalarProduct(cameraSpeed, &cameraFront);
    movement = cgVector3Negative(&movement);
    cameraPosition = cgVector3Add(&cameraPosition, &movement);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Vector3 direction = cgVector3CrossProduct(&cameraFront, &worldUp);
    direction = cgVector3Normalize(&direction);
    Vector3 movement = cgVector3ScalarProduct(cameraSpeed, &direction);
    movement = cgVector3Negative(&movement);
    cameraPosition = cgVector3Add(&cameraPosition, &movement);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Vector3 direction = cgVector3CrossProduct(&cameraFront, &worldUp);
    direction = cgVector3Normalize(&direction);
    Vector3 movement = cgVector3ScalarProduct(cameraSpeed, &direction);
    cameraPosition = cgVector3Add(&cameraPosition, &movement);
  }
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    Vector3 posNeg = cgVector3Negative(&cameraPosition);
    Vector3 direction = cgVector3Add(&(allCelestial[0].obj.transform.postion), &posNeg);
    cameraFront = cgVector3Normalize(&direction);
    Vector3 frontNeg = cgVector3Negative(&cameraFront);
    frontNeg = cgVector3ScalarProduct(100.0, &frontNeg);
    cameraPosition = cgVector3Add(&(allCelestial[0].obj.transform.postion), &frontNeg);
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    Vector3 posNeg = cgVector3Negative(&cameraPosition);
    Vector3 direction = cgVector3Add(&(allCelestial[1].obj.transform.postion), &posNeg);
    cameraFront = cgVector3Normalize(&direction);
    Vector3 frontNeg = cgVector3Negative(&cameraFront);
    frontNeg = cgVector3ScalarProduct(5.0, &frontNeg);
    cameraPosition = cgVector3Add(&(allCelestial[1].obj.transform.postion), &frontNeg);
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
    Vector3 posNeg = cgVector3Negative(&cameraPosition);
    Vector3 direction = cgVector3Add(&(allCelestial[2].obj.transform.postion), &posNeg);
    cameraFront = cgVector3Normalize(&direction);
    Vector3 frontNeg = cgVector3Negative(&cameraFront);
    frontNeg = cgVector3ScalarProduct(2.0, &frontNeg);
    cameraPosition = cgVector3Add(&(allCelestial[2].obj.transform.postion), &frontNeg);
  }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; 
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw   += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  const double yaw_rad = PI * yaw / 180.0;
  const double pitch_rad = pitch * PI / 180.0;
  Vector3 direction = {
    .x = cos(yaw_rad) * cos(pitch_rad),
    .y = sin(pitch_rad),
    .z = sin(yaw_rad) * cos(pitch_rad)
  };
  cameraFront = cgVector3Normalize(&direction);
}  
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
