#ifndef CHRONO_WIDGET_HPP
#define CHRONO_WIDGET_HPP

#include "widget/time_widget.hpp"

class ChronoWidget final : public TimeWidget
{
  public:

    ChronoWidget(QWidget *parent = 0);

  protected:

    int timeToWait();
};

#endif // CHRONO_WIDGET_HPP
