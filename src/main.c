#include "nes.h"

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>


NES nes = {0};

bool allow_step = true;
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) allow_step = true;
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    GLFWwindow* window = glfwCreateWindow(280 << 1, 240 << 1, "NyaES", NULL, NULL);

    glfwMakeContextCurrent(window);
    glewInit();


    igCreateContext(NULL);
    ImGuiIO* pIO = igGetIO_Nil();
    pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    igStyleColorsDark(NULL);

    ImGuiStyle* pStyle = igGetStyle();
    ImGuiStyle_ScaleAllSizes(pStyle, 1);
    pStyle->FontScaleDpi = 1;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(NULL);


    NES* pNes = &nes;

    load_rom(pNes, "../__PatreonRoms/2_ReadWrite.nes");
    reset(pNes);

    glfwSetKeyCallback(window, glfw_key_callback);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        igNewFrame();

        if (!pNes->cpu.cpu_halted) {
            if (allow_step) {
                allow_step = false;
                step_cpu(pNes);
            }
        }

        {
            igBegin("CPU", NULL, 0);

            igSeparatorText("Registers");

            igText("PC: %04x", nes.cpu.registers.program_counter);
            igText("A:  %02x", nes.cpu.registers.a);
            igText("X:  %02x", nes.cpu.registers.x);
            igText("Y:  %02x", nes.cpu.registers.y);

            igSeparatorText("State");

            igText("Halted:             %s", nes.cpu.cpu_halted ? "yes" : "no");
            igText("Remaining Cycles:   %i", nes.cpu.remaining_cpu_cycles);

            igSeparatorText("Flags");

            igText("Carry:              %i", nes.cpu.flags.carry);
            igText("Decimal:            %i", nes.cpu.flags.decimal);
            igText("Interrupt Disable:  %i", nes.cpu.flags.interrupt_disable);
            igText("Negative:           %i", nes.cpu.flags.negative);
            igText("Overflow:           %i", nes.cpu.flags.overflow);
            igText("Zero:               %i", nes.cpu.flags.zero);

            igEnd();
        }

        igRender();


        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);


        glClearColor(0.001, 0.003, 0.002, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());


        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);

    glfwDestroyWindow(window);
    glfwTerminate();
}