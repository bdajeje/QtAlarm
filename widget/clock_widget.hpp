#ifndef CLOCK_WIDGET_HPP
#define CLOCK_WIDGET_HPP

#include <array>

#include "utils/properties.hpp"
#include "widget/time_widget.hpp"

class ClockWidget final : public TimeWidget
{
  public:

    ClockWidget(QWidget *parent = 0);

  protected:

    std::array<QCheckBox*, 7> m_widget_days;
    static constexpr short number_days {7};
    static const std::array<utils::Property, 7> day_property;

  protected:

    virtual int timeToWait() const override;
    virtual void saveValues() const override;
    QDateTime nextDateTime() const;
    QDateTime selectedDateTime() const;
};

#endif // CLOCK_WIDGET_HPP
