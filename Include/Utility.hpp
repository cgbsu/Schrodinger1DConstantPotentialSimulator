#include <Common.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__UTILITY__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__UTILITY__HPP

static const std::string allToString(auto... toStringify)
{
	std::stringstream buffer;
	(buffer << ... << toStringify);
	return buffer.str();
}

template<typename ScalarParameterType>
struct IsComplexHelper {
	constexpr static const bool isComplex = false;
	constexpr IsComplexHelper(ScalarParameterType) {}
};

template<typename ScalarParameterType>
struct IsComplexHelper<std::complex<ScalarParameterType>> {
	constexpr static const bool isComplex = true;
	constexpr IsComplexHelper(std::complex<ScalarParameterType>) {}
};

template<typename ScalarParameterType>
constexpr static const bool isComplex = IsComplexHelper<ScalarParameterType>::isComplex;

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__UTILITY__HPP

