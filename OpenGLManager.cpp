#include "OpenGLManager.hpp"

void					key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Control*			control;

	(void)scancode;
	(void)mods;
	control = reinterpret_cast<Control *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	control->processInput(key, action);
}

static void				cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	Control*			control;

	control = reinterpret_cast<Control *>(glfwGetWindowUserPointer(window));
	control->processMouse(xPos, yPos);
}

void 					error_callback(int error, const char* description)
{
	std::cout << "Error #" << error << ": " << description << std::endl;
}



OpenGLManager::OpenGLManager()
{
	this->_setWindowInfo(512, 512, "Untitled");
	this->_setClippingInfo(0.785f, 1, 0.1, 10000.f);
	this->_initOpenGl();
	this->_createProjectionMatrix();
}

OpenGLManager::OpenGLManager(GLfloat width, GLfloat height, std::string winName)
{
	this->_setWindowInfo(width, height, winName);
	this->_setClippingInfo(0.785f, width / height, 0.1, 100000.f);
	this->_initOpenGl();
	this->_createProjectionMatrix();
}

/*
OpenGLManager::OpenGLManager(const OpenGLManager & rhs)
{
	*this = rhs;
}
*/

OpenGLManager::~OpenGLManager()
{
    glfwDestroyWindow(this->_window);
    glfwTerminate();
    // delete this->_window ???
}

/*
OpenGLManager&			OpenGLManager::operator=(const OpenGLManager & rhs)
{
	this->_window = 
	
	return (*this);
}
*/

void					OpenGLManager::_setWindowInfo(GLfloat width, GLfloat height, std::string name)
{
	this->_winInfo.width = width;
	this->_winInfo.height = height;
	this->_winInfo.winName = name;
}

void					OpenGLManager::_setClippingInfo(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	this->_clipInfo.fov = fov;
	this->_clipInfo.aspect = aspect;
	this->_clipInfo.zNear = zNear;
	this->_clipInfo.zFar = zFar;
}

void					OpenGLManager::_createProjectionMatrix(void)
{
	this->_clipInfo.aspect = this->_winInfo.width / this->_winInfo.height;
	this->_projectionMatrix.computeProjectionMatrix(this->_clipInfo.fov, this->_clipInfo.aspect, this->_clipInfo.zNear, this->_clipInfo.zFar);
}

void					OpenGLManager::_initOpenGl( void )
{
    int     			width, height;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(0);
	glfwWindowHint(GLFW_SAMPLES, 4);
  	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->_window = glfwCreateWindow(this->_winInfo.width, this->_winInfo.height, this->_winInfo.winName.c_str(), NULL, NULL);
    if (! this->_window )
    {
        glfwTerminate();
        exit(0);
    }
    
    glfwSetKeyCallback(this->_window, key_callback);
    glfwSetCursorPosCallback(this->_window, cursor_position_callback);
    glfwMakeContextCurrent(this->_window);
    glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDepthFunc(GL_LESS);
	glfwGetFramebufferSize(this->_window, &(width), &(height));
    glViewport(0, 0, width, height);
}

int						OpenGLManager::shouldClose()
{
	return (glfwWindowShouldClose(this->_window));
}

void					OpenGLManager::swap()
{
	glfwSwapBuffers(this->_window);
}

OpenGLMatrix			OpenGLManager::getProjMat()
{
	return this->_projectionMatrix;
}

void					OpenGLManager::setControl(Control *control)
{
	glfwSetWindowUserPointer(this->_window, static_cast<void *>(control));
}

void					OpenGLManager::setWindowName(std::string name)
{
	glfwSetWindowTitle(this->_window, name.c_str());
}
