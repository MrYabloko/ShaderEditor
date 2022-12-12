#include "global_vars.h"

GLFWwindow*& get_main_window()
{
    static GLFWwindow *window;
    return window;
}
