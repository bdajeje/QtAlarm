#include "clock_widget.hpp"

#include <iostream>

#include <QTime>

ClockWidget::ClockWidget(QWidget *parent)
  : TimeWidget(parent)
{
  m_widget_hours_input->setMaximum(24);
  m_widget_mins_input->setMaximum(60);
  m_widget_secs_input->setMaximum(60);
}

int ClockWidget::timeToWait()
{
  const auto& current_time  = QTime::currentTime();
  const auto& selected_time = QTime(m_widget_hours_input->value(), m_widget_mins_input->value(), m_widget_secs_input->value());

  if( selected_time > current_time )
    return current_time.secsTo(selected_time);
  else
    return secs_in_one_day - selected_time.secsTo(current_time);

//  if( current_time.hour() > selected_hour  &&
//      current_time.minute() > selected_min &&
//      current_time.second() > selected_sec )
//    return (current_time.hour() - selected_hour) * secs_in_one_hour +
//           (current_time.minute() - selected_min) * secs_in_one_minute +
//           current_time.second() - selected_sec;
//  else
//  {
//    const auto hour_diff = selected_hour + 24 - current_time.hour();

//    return hour_diff * secs_in_one_hour +
//           ;
//  }
}
