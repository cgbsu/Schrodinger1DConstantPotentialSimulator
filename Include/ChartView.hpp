#include <Data.hpp>

#ifndef SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP
#define SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

QT_USE_NAMESPACE

template<auto ProfileTagParamterConstant = defaultProfile>
struct ChartView : public QChartView
{
	constexpr static const auto profileTag = ProfileTagParamterConstant;
	using DataSetType = Data<profileTag>;
	ChartView(QWidget* parent, std::initializer_list<DataSetType> regions)
			: QChartView(new QChart(), parent), regions(scatterSeriesFromData(regions)) {
		for(QScatterSeries* region : this->regions)
			this->chart()->addSeries(region);
		//chart()->createDefaultAxes();
		setRubberBand(QChartView::RectangleRubberBand);
	}
	ChartView(QWidget* parent = nullptr) : QChartView(new QChart(), parent) {
		setRubberBand(QChartView::RectangleRubberBand);
		//chart()->createDefaultAxes();
	}
	virtual ~ChartView() {}

	auto extents() const noexcept
	{
		const auto minimum = regions[0]->at(0).x();
		const QScatterSeries* end = regions[regions.size() - 1];
		const auto maximum = end->at(end->count() - 1).x();
		return std::pair{minimum, maximum};
	}

	void createAxis()
	{
		if(regions.size() > 1)
		{
			const auto extents_ = extents();
			chart()->removeAxis(x);
			chart()->removeAxis(y);
			x = new QValueAxis;
			y = new QValueAxis;
			//x->setRange(extents_.first,  extents_.second);
			x->setRange(-100, 100);
			y->setRange(-100, 100);
			chart()->addAxis(x, Qt::AlignBottom);
			chart()->addAxis(y, Qt::AlignLeft);
			//for(QScatterSeries* region : regions) {
			//	region->attachAxis(x);
			//	region->attachAxis(y);
			//}
		}
	}
	bool replaceRegion(size_t regionIndex, const DataSetType& region)
	{
		if(regionIndex < regions.size())
		{
			chart()->removeSeries(regions[regionIndex]);
			chart()->addSeries(region.toScatterSeries());
			//createAxis();
			chart()->createDefaultAxes();
			return true;
		}
		return false;
	}
	void addRegion(const DataSetType& region)
	{
		chart()->addSeries(region.toScatterSeries());
		//createAxis();
		chart()->createDefaultAxes();
	}
	protected: 
		std::vector<QScatterSeries*> regions;
		QAbstractAxis* x;
		QAbstractAxis* y;
};

#endif // SCHRODINGER_1D__CONSTANT__POTENTIAL__SIMULATOR__INCLUDE__GUARD__CHART__VIEW__HPP

