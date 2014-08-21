#ifndef CLOCK_WIDGET_HPP
#define CLOCK_WIDGET_HPP

#include "widget/time_widget.hpp"

class ClockWidget final : public TimeWidget
{
  public:

    ClockWidget(QWidget *parent = 0);

  protected:

    int timeToWait();
};

#endif // CLOCK_WIDGET_HPP
