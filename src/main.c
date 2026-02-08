#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <shaders/shaders.h>
#include <texture/textures.h>
#include <transform/matrix.h>
#include <transform/vector3.h>
#include <camera/camera.h>
#include <objects/object.h>
#include <objects/shapes.h>

#define true 1
#define false 0

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);

unsigned int const SCR_WIDTH = 800;
unsigned int const SCR_HEIGHT = 600;

Vector3 const worldUp = {0, 1, 0};
Vector3 cameraPosition = {0, 0, 5};
Vector3 cameraFront = {0, 0, -1};

int firstMouse = true;
double yaw   = -90.0;
double pitch =  0.0;
double lastX =  SCR_WIDTH / 2.0;
double lastY =  SCR_HEIGHT / 2.0;
double fov   =  45.0;

double deltaTime = 0.0;
double lastFrame = 0.0;

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

int main()
{
  GLFWwindow * window = setupOpenglContexWindow();
  if (window == NULL)
    return -1;

  Shaders shaders = cgShadersLoad("include/shaders/vertex.glsl", "include/shaders/fragment.glsl");

  //Load texture
  cgTextureInit();
  Texture texture1 = cgTextureLoad("assets/wall.jpg", false);
  Texture texture2 = cgTextureLoad("assets/awesomeface.png", true);
  Texture texture3 = cgTextureLoad("assets/2k_earth_daymap.jpg", false);
  Texture texture4 = cgTextureLoad("assets/2k_saturn.jpg", false);

  cgShadersUniformSetInt(&shaders, "texture0Data", 0);

  //Setup camera and projection matrix
  Vector3 cameraTarget = cgVector3Add(&cameraPosition, &cameraFront);
  Camera camera = cgCameraCreate(&cameraPosition, &cameraTarget, &worldUp);

  Matrix4 projection = cgMatrixPerspective(45.0, (double)SCR_WIDTH/(double)SCR_HEIGHT, 0.1, 100.0);
  cgShadersUniformSetMatrix(&shaders, "projection", &projection);

  Object sphere = cgShapeCreateSphere(1.0f);

  Vector3 sphereColor = {1.0, 1.0, 1.0};
  cgShadersUniformSetVector3(&shaders, "objectColor", &sphereColor);

  Vector3 lightPos = {5.0, 5.0, 5.0};
  cgShadersUniformSetVector3(&shaders, "lightPos", &lightPos);
  Vector3 lightColor = {0.95, 0.95, 0.7};
  cgShadersUniformSetVector3(&shaders, "lightColor", &lightColor);

  //MAIN LOOP
  while (!glfwWindowShouldClose(window)) {
    double currFrame = glfwGetTime();
    deltaTime = currFrame - lastFrame;
    lastFrame = currFrame;

    processInput(window);

    cameraTarget = cgVector3Add(&cameraPosition, &cameraFront);
    cgCameraUpdate(&camera, &cameraPosition, &cameraTarget, &worldUp);
    cgShadersUniformSetVector3(&shaders, "viewPos", &cameraPosition);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cgShadersUse(&shaders);

    Matrix4 view = cgCameraLookAtMatrix(&camera);
    cgShadersUniformSetMatrix(&shaders, "view", &view);

    Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    Matrix4 sphereRotation = cgMatrixRotation(&rotationAxis, deltaTime*10);
    sphere.model = cgMatrixMatrixMultiplication(&sphereRotation, &(sphere.model));

    cgTextureBind(&texture4, GL_TEXTURE_2D, GL_TEXTURE0);
    cgShadersUniformSetMatrix(&shaders, "model", &(sphere.model));
    cgObjectDraw(&shaders, &sphere);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  //Free resources
  cgShadersFree(&shaders);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);

  double cameraSpeed = 2.5 * deltaTime;
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
