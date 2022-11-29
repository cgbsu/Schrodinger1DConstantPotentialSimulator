#include <Data.hpp>

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

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

