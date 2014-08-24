#ifndef CHRONO_WIDGET_HPP
#define CHRONO_WIDGET_HPP

#include "widget/time_widget.hpp"

class ChronoWidget final : public TimeWidget
{
  public:

    ChronoWidget(QWidget *parent = 0);

    virtual bool isRepeat() const;

  protected:

    int timeToWait();
    void saveValues();

  protected:

    QCheckBox* m_widget_repeat_box;
};

#endif // CHRONO_WIDGET_HPP
