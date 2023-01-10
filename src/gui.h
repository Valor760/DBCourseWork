#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>


namespace gui {
void RenderBegin();
void RenderEnd(GLFWwindow* window, int width, int height);
void Init(GLFWwindow* window);
} // namespace gui