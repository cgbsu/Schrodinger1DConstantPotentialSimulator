#include <Constants.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__DATA__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__DATA__HPP

template<auto ProfileTagParamterConstant = defaultProfile>
struct Region
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ScalarType = Constants<profileTag>::ScalarType;
	const ScalarType from;
	const ScalarType to;
};

template<auto ProfileTagParamterConstant = defaultProfile>
struct Data
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using ScalarType = Constants<profileTag>::ScalarType;
	const Region<profileTag> region;
	const std::vector<ScalarType> x;
	const std::vector<ScalarType> y;
	constexpr const ScalarType stepSize() const noexcept {
		return (region.to - region.from) / static_cast<ScalarType>(x.size());
	}
};

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__DATA__HPP

