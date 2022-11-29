#include <iostream>
// Sample app built with Dear ImGui and ImPlot
// This app uses implot and imgui, but does not output to any backend! It only serves as a proof that an app can be built, linked, and run.

#include "imgui.h"
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"
#include "implot.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{    
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) {
		std::cerr << "GLFW failed to initialize!\n";
		return 1;
	}
	auto* window = glfwCreateWindow(400, 400, "Graph", NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		std::cout << "GLFW failed to create a window!\n";
		return 2;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
    printf("sample_implot: start\n");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
    ImPlot::CreateContext();

    // Additional imgui initialization needed when no backend is present
    //ImGui::GetIO().DisplaySize = ImVec2(400.f, 400.f);
    //ImGui::GetIO().Fonts->Build();


	while(!glfwWindowShouldClose(window))
    {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Hello, world!"))
        {
            ImGui::Text("Hello again");

            if (ImPlot::BeginPlot("My Plot"))
            {
                static double values[] = {1., 3., 5.};
                ImPlot::PlotLine("Values", values, 3);
                ImPlot::EndPlot();
            }

            #ifdef IMPLOT_INSTANTIATE_ALL_NUMERIC_TYPES
            if (ImPlot::BeginPlot("My Plot (long double)"))
            {
                static long double values[] = {1., 3., 5.};
                ImPlot::PlotLine("Values", values, 3);
                ImPlot::EndPlot();
            }
            #endif

            ImGui::End();
        }

        ImGui::Render();
		int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(255, 255, 255, 255);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
    }

    ImPlot::DestroyContext();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
    printf("sample_implot: end\n");
    return 0;
}
