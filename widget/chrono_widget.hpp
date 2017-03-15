#ifndef CHRONO_WIDGET_HPP
#define CHRONO_WIDGET_HPP

#include "widget/time_widget.hpp"

class ChronoWidget final : public TimeWidget
{
  public:

	ChronoWidget();

  protected:

	virtual int timeToWait() const override;
	virtual void saveValues() const override;
};

#endif // CHRONO_WIDGET_HPP
