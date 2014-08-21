#include "clock_widget.hpp"

#include <iostream>

#include <QTime>

#include <utils/properties.hpp>

ClockWidget::ClockWidget(QWidget *parent)
  : TimeWidget(parent)
{
  // Set maximum values
  m_widget_hours_input->setMaximum(24);
  m_widget_mins_input->setMaximum(60);
  m_widget_secs_input->setMaximum(60);

  // Set last state
  const QString& hour_str = utils::Properties::get( utils::Property::ClockHour );
  const QString& min_str  = utils::Properties::get( utils::Property::ClockMin );
  const QString& sec_str  = utils::Properties::get( utils::Property::ClockSec );

  if( !hour_str.isEmpty() )
    m_widget_hours_input->setValue( hour_str.toInt() );
  if( !min_str.isEmpty() )
    m_widget_mins_input->setValue( min_str.toInt() );
  if( !sec_str.isEmpty() )
    m_widget_secs_input->setValue( sec_str.toInt() );
}

int ClockWidget::timeToWait()
{
  const auto& current_time  = QTime::currentTime();
  const auto& selected_time = QTime(m_widget_hours_input->value(), m_widget_mins_input->value(), m_widget_secs_input->value());

  if( selected_time > current_time )
    return current_time.secsTo(selected_time);
  else
    return secs_in_one_day - selected_time.secsTo(current_time);
}

void ClockWidget::saveValues()
{
  utils::Properties::save( utils::Property::ClockHour, QString::number(m_widget_hours_input->value()) );
  utils::Properties::save( utils::Property::ClockMin, QString::number(m_widget_mins_input->value()) );
  utils::Properties::save( utils::Property::ClockSec, QString::number(m_widget_secs_input->value()) );
}
