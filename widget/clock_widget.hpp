#ifndef CLOCK_WIDGET_HPP
#define CLOCK_WIDGET_HPP

#include <array>

#include "utils/properties.hpp"
#include "widget/time_widget.hpp"

class ClockWidget final : public TimeWidget
{
  public:

    ClockWidget(QWidget *parent = 0);

    virtual bool isRepeat() const;

  protected:

    std::array<QCheckBox*, 7> m_widget_days;
    static constexpr short number_days {7};
    static const std::array<utils::Property, 7> day_property;

  protected:

    int timeToWait();
    void saveValues();
};

#endif // CLOCK_WIDGET_HPP
