#include <glad/glad.h>
#include <GLFW/glfw3.h>	

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
char press = 0;
char press2 = 0;
char press3 = 0;

// Exemple number
int example = 0;
unsigned int models = 0;

class Example1 
{
	public:
		glm::mat4 model;
		char mode = 't';

		Example1()
		{
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f)); // translate it down so it's at the center of the scene
		}

		glm::mat4 getModel() {
			return model;
		}

		void scale(char dir, float speed)
		{
			speed *= 5;
			switch (dir) {
			case 'l': // Left
				model = glm::scale(model, glm::vec3(1.0f-speed, 1.0f, 1.0f));
				break;
			case 'r': // Right
				model = glm::scale(model, glm::vec3(1.0f+speed, 1.0f, 1.0f));
				break;
			case 'u': // up
				model = glm::scale(model, glm::vec3(1.0f, 1.0f+speed, 1.0f));
				break;
			case 'd': // down
				model = glm::scale(model, glm::vec3(1.0f, 1.0f-speed, 1.0f));
				break;
			case 'i': // in
				model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f+speed));
				break;
			case 'o': // out
				model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f-speed));
				break;
			default:
				break;
			}
		}

		void rotate(char dir, float speed)
		{
			speed *= 5;
			switch (dir) {
			case 'l': // Left
				model = glm::rotate(model, speed, glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 'r': // Right
				model = glm::rotate(model, -speed, glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 'u': // up
				model = glm::rotate(model, speed, glm::vec3(1.0f, 0.0f, 0.0f));
				break;
			case 'd': // down
				model = glm::rotate(model, -speed, glm::vec3(1.0f, 0.0f, 0.0f));
				break;
			case 'i': // in
				model = glm::rotate(model, speed, glm::vec3(0.0f, 0.0f, 1.0f));
				break;
			case 'o': // out
				model = glm::rotate(model, -speed, glm::vec3(0.0f, 0.0f, 1.0f));
				break;
			default:
				break;
			}

		}

		void translate(char dir, float speed)
		{
			speed *= 5;
			switch (dir) {
			case 'l': // Left
				model = glm::translate(model, glm::vec3(-speed, 0.0f, 0.0f));
				break;
			case 'r': // Right
				model = glm::translate(model, glm::vec3(speed, 0.0f, 0.0f));
				break;
			case 'u': // up
				model = glm::translate(model, glm::vec3(0.0f, speed, 0.0f));
				break;
			case 'd': // down
				model = glm::translate(model, glm::vec3(0.0f, -speed, 0.0f));
				break;
			case 'i': // in
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, speed));
				break;
			case 'o': // out
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, -speed));
				break;
			default:
				break;
			}
		}
};

class Example2 
{
	public:
		glm::mat4 modelPlanet;

		Example2()
		{
			modelPlanet = glm::translate(modelPlanet, glm::vec3(0.0f, 0.0f, -20.0f));
		}

		glm::mat4 getPlanet(float speed)
		{
			modelPlanet = glm::rotate(modelPlanet, speed, glm::vec3(0.0f, 1.0f, 0.0f));
			
			return modelPlanet;
		}
};

// Examples
Example1 ex1;
Example2 ex2;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader ourShader(FileSystem::getPath("resources/cg_ufpel.vs").c_str(), FileSystem::getPath("resources/cg_ufpel.fs").c_str());

	// Models
	Model rock(FileSystem::getPath("resources/objects/rock/rock.obj"));
	Model planet(FileSystem::getPath("resources/objects/planet/planet.obj"));
	Model spacecraft(FileSystem::getPath("resources/objects/rock/rock.obj"));


	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		
		if (example == 0) {
			glm::mat4 modelPlanet;
			modelPlanet = glm::translate(modelPlanet, glm::vec3(0.0f, 0.0f, -20.0f));
			ourShader.setMat4("model", modelPlanet);
			planet.Draw(ourShader);

			glm::mat4 modelSpacecraft;
			modelSpacecraft = ex1.getModel();

			ourShader.setMat4("model", modelSpacecraft);
			spacecraft.Draw(ourShader);
		}
		else {

			glm::mat4 modelPlanet2;
			modelPlanet2 = ex2.getPlanet(deltaTime);

			ourShader.setMat4("model", modelPlanet2);
			planet.Draw(ourShader);
			
			glm::mat4 modelRock;
			modelRock = glm::translate(modelRock, glm::vec3(0.0f, 0.8f, -20.0f)); // translate it down so it's at the center of the scene
			modelRock = glm::scale(modelRock, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

			glm::mat4 trans;
			glm::mat4 aux;
		
			for (unsigned int i = 0; i <= models; i++) {

				trans = aux * modelRock;
				trans = glm::rotate(trans, (float)currentFrame, glm::vec3(0.0f, 1.0f, 0.0f));
				trans = glm::translate(trans, glm::vec3(20.0 + (i*5), 0.0f, 0.0f));
				trans = glm::rotate(trans, (float)currentFrame, glm::vec3(0.0f, 1.0f, 0.0f));


				ourShader.setMat4("model", trans);
				rock.Draw(ourShader);
			}

			for (unsigned int i = 0; i <= models; i++) {

				trans = aux * modelRock;
				trans = glm::rotate(trans, (float)currentFrame, glm::vec3(1.0f, 0.0f, 0.0f));
				trans = glm::translate(trans, glm::vec3(0.0f, 25.0 + (i * 5), 0.0f));
				trans = glm::rotate(trans, (float)currentFrame, glm::vec3(1.0f, 0.0f, 0.0f));


				ourShader.setMat4("model", trans);
				rock.Draw(ourShader);
			}

		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		if (press == 0) {
			press = 1;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
		if (press == 1) {
			press = 0;
			example = (example + 1) % 2;
		}
	}

	if (example == 0) {
		if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
			ex1.translate('r', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
			ex1.translate('l', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
			ex1.translate('u', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
			ex1.translate('d', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
			ex1.translate('i', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
			ex1.translate('o', deltaTime);

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			ex1.rotate('r', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			ex1.rotate('l', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			ex1.rotate('u', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			ex1.rotate('d', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			ex1.rotate('i', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			ex1.rotate('o', deltaTime);

		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			ex1.scale('r', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			ex1.scale('l', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			ex1.scale('u', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			ex1.scale('d', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
			ex1.scale('i', deltaTime);
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			ex1.scale('o', deltaTime);
	}
	else{

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			if (press2 == 0) {
				press2 = 1;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
			if (press2 == 1) {
				press2 = 0;
				models++;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			if (press3 == 0) {
				press3 = 1;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
			if (press3 == 1) {
				press3 = 0;
				if (models > 0)
					models--;
			}
		}
	}


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
