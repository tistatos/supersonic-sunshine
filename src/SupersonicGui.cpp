#include "SupersonicGui.h"

SupersonicGUI::SupersonicGUI(GLFWwindow* window){
// Create a nanogui screen and pass the glfw pointer to initialize

  Screen* screen  = new Screen();
  screen->initialize(window, true);

  gui = new FormHelper(screen);
  ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Supersonic Sunshine");

  gui->addVariable("FPS ", fps)->setEditable(false);
  
  screen->setVisible(true);
  screen->performLayout();
  nanoguiWindow->center();

  glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));


  /** Callback lambda functions **/
  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
    auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
    thiz->mScreen->cursorPosCallbackEvent(x, y);
  });

  glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int modifiers) {
    auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
    thiz->mScreen->mouseButtonCallbackEvent(button, action, modifiers);
  });

  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
    thiz->mScreen->keyCallbackEvent(key, scancode, action, mods);
  });

  glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) {
    auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
    thiz->mScreen->charCallbackEvent(codepoint);
  });

  glfwSetDropCallback(window, [](GLFWwindow* window, int count, const char **filenames) {
    auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
    thiz->mScreen->dropCallbackEvent(count, filenames);
  });

  glfwSetScrollCallback(window, [](GLFWwindow *window, double x, double y) {
    auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
    thiz->mScreen->scrollCallbackEvent(x, y);
  });
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
    auto thiz = reinterpret_cast<SupersonicGUI*>(glfwGetWindowUserPointer(window));
    thiz->mScreen->resizeCallbackEvent(width, height);
  });

  mScreen = screen;
}
void SupersonicGUI::refresh(){
  gui->refresh();
}
void SupersonicGUI::draw(){
  mScreen->drawContents();
  mScreen->drawWidgets();
}