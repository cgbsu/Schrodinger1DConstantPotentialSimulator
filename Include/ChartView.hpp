#include <Data.hpp>
#include "imgui.h"
#include <bindings/imgui_impl_glfw.h>
#include <bindings/imgui_impl_opengl3.h>
#include "implot.h"
#include <GLFW/glfw3.h>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

template<auto ProfileTagParamterConstant = defaultProfile>
struct ChartBuffers
{
	using ConstantsType = Constants<ProfileTagParamterConstant>;
	using ScalarType = typename ConstantsType::ScalarType;
	std::vector<ScalarType> xBuffer, yBuffer;
};

template<auto ProfileTagParamterConstant = defaultProfile>
static auto consolidateData(std::vector<Data<ProfileTagParamterConstant>>& regions)
		-> ChartBuffers<ProfileTagParamterConstant> 
{
	using ConstantsType = Constants<ProfileTagParamterConstant>;
	using ScalarType = typename ConstantsType::ScalarType;
	std::vector<ScalarType> xBuffer, yBuffer;
	for(const auto& region : regions) {
		xBuffer.insert(xBuffer.end(), region.x.begin(), region.x.end());
		yBuffer.insert(yBuffer.end(), region.y.begin(), region.y.end());
	}
	return ChartBuffers{xBuffer, yBuffer};
}

<<<<<<< HEAD
//template<auto ProfileTagParamterConstant = defaultProfile>
//static void plot(const ChartBuffers<ProfileTagParamterConstant>& buffers)
//{
//	ImGui::NewFrame();	
//	if(ImGui::Begin("1D Schrodinger Constant Potential Simulation"))
//	{
//		if(ImPlot::BeginPlot("Wave Function"))
//		{
//			ImPlot::PlotLine(
//					"All Regions", 
//					buffers.xBuffer.data(), 
//					buffers.yBuffer.data(), 
//					buffers.xBuffer.size()
//				);
//			ImPlot::EndPlot();
//		}
//		else
//			std::cerr << "Something went wrong while plotting.\n";
//	}
//	else
//		std::cerr << "ImGui failed to begin!\n";
//	ImGui::End();
//	ImGui::Render();
//}
=======
template<auto ProfileTagParamterConstant = defaultProfile>
static void plot(const std::vector<Data<ProfileTagParamterConstant>>& data)//const ChartBuffers<ProfileTagParamterConstant>& buffers)
{
	if(ImGui::Begin("1D Schrodinger Constant Potential Simulation"))
	{
		if(ImPlot::BeginPlot("Wave Function"))
		{
			size_t region = 0;
			for(const auto& buffer : data)
			{
				ImPlot::PlotScatter(
						allToString("Region ", region).c_str(), 
						buffer.x.data(), 
						buffer.y.data(), 
						buffer.x.size()
					);
				++region;
			}
			ImPlot::EndPlot();
		}
		else
			std::cerr << "Something went wrong while plotting.\n";
	}
	else
		std::cerr << "ImGui failed to begin!\n";
	ImGui::End();
}

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static auto* initializeGUI()
{
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) {
		std::cerr << "GLFW failed to initialize!\n";
	}
	auto* window = glfwCreateWindow(1800, 800, "Graph", NULL, NULL);
	if(!window) {
		glfwTerminate();
		std::cout << "GLFW failed to create a window!\n";
		return window;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImPlot::CreateContext();
	//ImGui::GetIO().DisplaySize = ImVec2(400.f, 400.f);
    ImGui::GetIO().Fonts->Build();
	return window;
}

static void cleanUpGUI(auto* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void renderLoop(auto* window, auto job)
{
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();	
		job();
        ImGui::Render();
		int display_w, display_h;
        //glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);
        glClearColor(255, 255, 255, 255);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
}
>>>>>>> de9ea2b2fba2a408001843619ec49038db3b60ed

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

