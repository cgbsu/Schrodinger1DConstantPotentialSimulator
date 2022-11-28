#include <Common.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CONSTANTS__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CONSTANTS__HPP

template<typename ScalarParameterType>
constexpr static auto squareRoot(ScalarParameterType radicand) 
		-> ScalarParameterType {
	return std::sqrt(radicand);
}

template<typename ScalarParameterType>
constexpr static auto exponential(ScalarParameterType radicand) 
		-> ScalarParameterType {
	return std::exp(radicand);
}

template<typename ScalarParameterType>
constexpr static auto raise(ScalarParameterType base, ScalarParameterType power) 
		-> ScalarParameterType {
	return std::pow(base, power);
}

template<typename ScalarParameterType>
constexpr static auto conjugate(ScalarParameterType toConjugate) 
		-> std::complex<ScalarParameterType> {
	return std::conj<ScalarParameterType>(toConjugate);
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


enum class Profiles {
	PhysicallyInaccurateSimple
};

constexpr static const auto defaultProfile = Profiles::PhysicallyInaccurateSimple;

template<auto ProfileTagParamterConstant = defaultProfile>
struct Constants
{
	using ScalarType = float;
	constexpr const static auto squareRoot = squareRoot<ScalarType>;
	constexpr const static auto exponential = exponential<ScalarType>;
	constexpr const static auto raise = raise<ScalarType>;
	constexpr const static ScalarType reducedPlancksConstant = 1.f;
	constexpr const static ScalarType scalingFactor = 1.f;
	constexpr const static ScalarType squareRootOfTwo = 1.414213562;
	constexpr const static auto conjugate = conjugate<ScalarType>;
	constexpr const static ScalarType epsilon = std::numeric_limits<ScalarType>::epsilon();
};

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CONSTANTS__HPP

