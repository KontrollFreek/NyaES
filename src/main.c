#include "nes.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

NES nes = {0};

bool allow_step = true;
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) allow_step = true;
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    GLFWwindow* window = glfwCreateWindow(280 << 1, 240 << 1, "NyaES", NULL, NULL);

    NES* pNes = &nes;

    load_rom(pNes, "../__PatreonRoms/2_ReadWrite.nes");
    reset(pNes);

    glfwSetKeyCallback(window, glfw_key_callback);


    // TODO - add gui library for displaying info. maybe imgui?
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (!pNes->cpu.cpu_halted) {
            if (!allow_step) continue;
            allow_step = false;


            step_cpu(pNes);

            char buf[0xFFFF];
            int len = 0;

        #define __print_addr(x) len += sprintf(buf + len, "$%04x: 0x%02x\n", x, read_addr(pNes, x))
            __print_addr(0x0000);
            __print_addr(0x0001);
            __print_addr(0x0002);
            __print_addr(0x0550);
            __print_addr(nes.cpu.registers.program_counter);
            
            printf("\e[1;1H\e[2J%s", buf);
        }
    }

    glfwTerminate();
}