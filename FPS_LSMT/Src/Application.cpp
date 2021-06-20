#include "Application.h"
#include "Graph.h"
#include "InputManager.h"
#include "Camera.h"
#include "Debug.h"

using namespace Core::API;
using namespace Core::Debug;

Application* Application::instance = 0;

Application* Application::getInstance()
{
	if (instance == 0)
	{

		instance = new Application();
	}

	return instance;
}

Application::Application() : graph(nullptr), inputManager(Core::API::InputManager::getInstance()), cameraManager(nullptr)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PlatformerGL", NULL, NULL);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		Graph* graph = Core::API::Application::getInstance()->graph;

		//if (graph->scene == graph->resourcesManager->getResource("mainMenu.scn"))
		Core::API::Application::getInstance()->graph->loadScene("mainMenu.scn");

		// put the game in pause
		//Application::getInstance()->time.timeScale() = fabsf(Application::getInstance()->time.timeScale() - 1.f);
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

int Application::init()
{
	Log* log(Log::getInstance());
	// glfw window creation
	// --------------------
	if (window == NULL)
	{
		std::string statement = "Failed to create GLFW window";
		log->print(statement, LogType::ERROR);
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::string statement = "Failed to initialize GLAD";
		log->print(statement, LogType::ERROR);
		return -2;
	}

	// FreeType: load font library
	if (font.init() != 0)
	{
		std::string statement = "Error loading font library";
		log->print(statement, LogType::ERROR);
		return -3;
	}
		

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

	ImGui::StyleColorsClassic();
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	
	graph = new Graph;
	cameraManager = CameraManager::getInstance();

	return 0;
}

void newFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void endFrame(GLFWwindow* window)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Application::update()
{
	//graph->loadScene("level.scn");
	graph->loadScene("mainMenu.scn");

	while (!glfwWindowShouldClose(window))
	{
		newFrame();

		time.update();
		processInput(window);
		inputManager->updateInput(window);
		if (inputManager->key8_down)
		{
			editMode = !editMode;
			cameraManager->setActiveCamera();
		}

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_FRAMEBUFFER_SRGB); // gamma correction
		glEnable(GL_DEPTH_TEST); //Z buffer

		if (editMode)
			cameraManager->updateEditorCamera();

		graph->graphUpdate();

		graph->graphIMGUI(editMode);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		endFrame(window);
	}
}

Application::~Application()
{
	Log* log(Log::getInstance());
	

	delete graph;
	log->print("graph deleted", LogType::DEBUG);
	graph = nullptr;


	ResourcesManager* resourcesManager = ResourcesManager::getInstance();
	delete resourcesManager;
	log->print("resources manager deleted", LogType::DEBUG);

	delete cameraManager;
	log->print("camera manager deleted", LogType::DEBUG);

	delete inputManager;
	log->print("input manager deleted", LogType::DEBUG);

	delete Core::Debug::Log::getInstance();

	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

}
