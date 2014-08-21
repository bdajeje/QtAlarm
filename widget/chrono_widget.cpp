#include "chrono_widget.hpp"

ChronoWidget::ChronoWidget(QWidget *parent)
  : TimeWidget(parent)
{}

int ChronoWidget::timeToWait()
{
  return m_widget_hours_input->value() * secs_in_one_hour +
         m_widget_mins_input->value() * secs_in_one_minute +
         m_widget_secs_input->value();
}
