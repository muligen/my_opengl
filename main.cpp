#include <common.h>
#include <stb_image.h>

#include <shaders/shader_m.h>
#include <camera.h>
#include <input.h>
#include <texture.h>
#include <model.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void onMouseMove(GLFWwindow* window, double xpos, double ypos);
void onMouseDown(GLFWwindow* window, int button, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool bMouseMove = false;

// input
auto input = new CustomInput();

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main(int argc, char* argv[])
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "gl", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    input->init(window);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetCursorPosCallback(window, onMouseMove);
    glfwSetMouseButtonCallback(window, onMouseDown);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    // 创建对象
    zModel* cube = new zModel();
    cube->to_a_box();

    // 创建imgui上下文
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // 用户输入
    input->BindPress(GLFW_KEY_W, [](float dt, GLFWwindow* w) {
        camera.ProcessKeyboard(FORWARD, dt);
        });
    input->BindPress(GLFW_KEY_S, [](float dt, GLFWwindow* w) {
        camera.ProcessKeyboard(BACKWARD, dt);
        });
    input->BindPress(GLFW_KEY_A, [](float dt, GLFWwindow* w) {
        camera.ProcessKeyboard(LEFT, dt);
        });
    input->BindPress(GLFW_KEY_D, [](float dt, GLFWwindow* w) {
        camera.ProcessKeyboard(RIGHT, dt);
        });
    input->BindPress(GLFW_KEY_ESCAPE, [](float dt, GLFWwindow* w) {
        glfwSetWindowShouldClose(w, true);
        });
    input->BindPress(GLFW_KEY_Z, [](float dt, GLFWwindow* w) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        });
    input->BindPress(GLFW_KEY_X, [](float dt, GLFWwindow* w) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        });


    // ----------- 主渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        input->Input_loop(deltaTime, window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.f, 1.0f, 0.0f));
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        cube->update_mvp(model, view, projection);
        cube->draw();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Edit");                          // Create a window called "Hello, world!" and append into it.
            //自定义内容
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("CamPosition: %.2f,%.2f,%.2f", camera.Position.x, camera.Position.y, camera.Position.z);
            ImGui::End();
        }
        ImGui::Render();
        // 渲染 gui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete input;
    delete cube;

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos) {
    if (bMouseMove) {
        int deltaX = xpos - lastX;
        int deltaY = ypos - lastY;
        lastX = xpos;
        lastY = ypos;
        camera.ProcessMouseMovement(deltaX, deltaY);
    }
}

void onMouseDown(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        bMouseMove = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastX = xpos;
        lastY = ypos;
    }
    else {
        bMouseMove = false;
    }
}
