#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <shaders/shaders.h>
#include <texture/textures.h>
#include <transform/matrix.h>
#include <transform/vector3.h>
#include <camera/camera.h>
#include <objects/object.h>

#define true 1
#define false 0

typedef struct Shaders Shaders;
typedef struct Matrix4 Matrix4;
typedef struct Vector3 Vector3;
typedef struct Camera Camera;
typedef struct Object Object;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const Vector3 worldUp = {0, 1, 0};
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

int main()
{
  //OPENGL SETUP
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OPEN_GL", NULL, NULL);
  if (window == NULL)
  {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(1);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    printf("Failed to initialize GLAD\n");
    return -2;
  }    

  glEnable(GL_DEPTH_TEST);

  //PROGRAM CODE
  //Load shaders
  struct Shaders * shadersPtr = malloc(sizeof(Shaders));
  int shadersRes = cgShadersLoad(shadersPtr, "include/shaders/vertex.glsl", "include/shaders/fragment.glsl");

  //Load texture
  cgTextureInit();
  struct Texture texture1, texture2;
  cgTextureLoad(&texture1, "assets/wall.jpg", false);
  cgTextureLoad(&texture2, "assets/awesomeface.png", true);

  cgShadersUniformSetInt(shadersPtr, "texture0Data", 0);
  cgShadersUniformSetInt(shadersPtr, "texture1Data", 1);

  float cubeVertices[192] = {
    //Quad1 (Z = 1)
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    //Quad2 (Z = -1)
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    //Quad3 (Y = 1)
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

    //Quad4 (Y = -1)
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    
    //Quad5 (X = 1)
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    
    //Quad6 (X = -1)
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
  };

  unsigned int cubeIndices[36] = {
    0, 1, 2,
    1, 2, 3,
    
    4, 5, 6,
    5, 6, 7,

    8, 9, 10,
    9, 10, 11,

    12, 13, 14,
    13, 14, 15,

    16, 17, 18,
    17, 18, 19,

    20, 21, 22,
    21, 22, 23
  };

  Object * cube = malloc(sizeof(Object));
  if (cube == NULL)
    return -20;
  int res = cgObjectCreate(cube, 192, cubeVertices, 36, cubeIndices);
  if (res == -1)
    return -10;
  cgObjectSendRenderData(cube);

  Vector3 cubePos[10] = {
    { 0.0f,  0.0f,  0.0f}, 
    { 2.0f,  5.0f, -15.0f}, 
    {-1.5f, -2.2f, -2.5f},  
    {-3.8f, -2.0f, -12.3f},  
    { 2.4f, -0.4f, -3.5f},  
    {-1.7f,  3.0f, -7.5f},  
    { 1.3f, -2.0f, -2.5f},  
    { 1.5f,  2.0f, -2.5f}, 
    { 1.5f,  0.2f, -1.5f}, 
    {-1.3f,  1.0f, -1.5f}
  };

  Vector3 cameraTarget = cgVector3Add(&cameraPosition, &cameraFront);
  Camera camera = cgCameraCreate(&cameraPosition, &cameraTarget, &worldUp);

  Matrix4 projection = cgMatrixPerspective(45.0, (double)SCR_WIDTH/(double)SCR_HEIGHT, 0.1, 100.0);
  cgShadersUniformSetMatrix(shadersPtr, "projection", &projection);

  //MAIN LOOP
  while (!glfwWindowShouldClose(window))
  {
    double currFrame = glfwGetTime();
    deltaTime = currFrame - lastFrame;
    lastFrame = currFrame;

    processInput(window);

    cameraTarget = cgVector3Add(&cameraPosition, &cameraFront);
    cgCameraUpdate(&camera, &cameraPosition, &cameraTarget, &worldUp);

    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cgShadersUse(shadersPtr);
  
    cgTextureBind(&texture1, GL_TEXTURE_2D, GL_TEXTURE0);
    cgTextureBind(&texture2, GL_TEXTURE_2D, GL_TEXTURE1);

    Matrix4 view = cgCameraLookAtMatrix(&camera);
    cgShadersUniformSetMatrix(shadersPtr, "view", &view);

    cgShadersUniformSetMatrix(shadersPtr, "model", &(cube->model));
    cgObjectDraw(cube);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  //Free resources
  cgShadersFree(shadersPtr);

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
