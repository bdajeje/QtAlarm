#include "clock_widget.hpp"

#include <iostream>

#include <QLabel>
#include <QTime>

const std::array<utils::Property, 7> ClockWidget::day_property { { utils::Property::ClockRepeatMonday,
                                                                   utils::Property::ClockRepeatTuesday,
                                                                   utils::Property::ClockRepeatWednesday,
                                                                   utils::Property::ClockRepeatThursday,
                                                                   utils::Property::ClockRepeatFriday,
                                                                   utils::Property::ClockRepeatSaturday,
                                                                   utils::Property::ClockRepeatSunday } };

ClockWidget::ClockWidget(QWidget *parent)
  : TimeWidget(parent)
{
  // Create objects
  auto days_layout = new QHBoxLayout();

  const char* days[] = {"M", "T", "W", "T", "F", "S", "S"};
  for( size_t i = 0; i < number_days; ++i )
  {
    auto day_layout = new QVBoxLayout();
    auto repeat_day = new QCheckBox();
    m_widget_days[i] = new QCheckBox(repeat_day);
    day_layout->addWidget( new QLabel(days[i]) );
    day_layout->addWidget(repeat_day);
    days_layout->addLayout(day_layout);
  }

  m_main_layout->insertLayout(2, days_layout);

  // Set maximum values
  m_widget_hours_input->setMaximum(24);
  m_widget_mins_input->setMaximum(60);
  m_widget_secs_input->setMaximum(60);

  // Get last used values
  const QString& hour_str  = utils::Properties::get( utils::Property::ClockHour );
  const QString& min_str   = utils::Properties::get( utils::Property::ClockMin );
  const QString& sec_str   = utils::Properties::get( utils::Property::ClockSec );

  // Set last used values
  if( !hour_str.isEmpty() )
    m_widget_hours_input->setValue( hour_str.toInt() );
  if( !min_str.isEmpty() )
    m_widget_mins_input->setValue( min_str.toInt() );
  if( !sec_str.isEmpty() )
    m_widget_secs_input->setValue( sec_str.toInt() );
  for( size_t i = 0; i < number_days; ++i )
  {
    if( utils::Properties::get( day_property[i] ).toInt() )
      m_widget_days[i]->setChecked(true);
  }
}

int ClockWidget::timeToWait() const
{
  const auto current_date = QDateTime::currentDateTime();
  const auto next_date    = nextDateTime();
  return current_date.secsTo(next_date);
}

QDateTime ClockWidget::selectedDateTime() const
{
  return QDateTime(QDate::currentDate(), {m_widget_hours_input->value(), m_widget_mins_input->value(), m_widget_secs_input->value()});
}

QDateTime ClockWidget::nextDateTime() const
{
  // Get next time, but date is not set
  auto next_date = selectedDateTime();
  bool found = false;

  // Try to find next day to trigger
  for( int i = 0; i < number_days; ++i )
  {
    int day_in_week = next_date.date().dayOfWeek() - 1; // Day in week starts at 1
    int day_offset  = (day_in_week >= number_days) ? number_days - day_in_week : day_in_week;
    if( m_widget_days[day_offset]->isChecked() )
    {
      found = true;
      break;
    }

    next_date = next_date.addDays(1);
  }

  // Not found ? Reset selected date time
  if(!found)
    next_date = selectedDateTime();

  return (next_date < QDateTime::currentDateTime()) ? next_date.addDays(1) : next_date;
}

void ClockWidget::saveValues() const
{
  utils::Properties::save( utils::Property::ClockHour, QString::number(m_widget_hours_input->value()) );
  utils::Properties::save( utils::Property::ClockMin, QString::number(m_widget_mins_input->value()) );
  utils::Properties::save( utils::Property::ClockSec, QString::number(m_widget_secs_input->value()) );
  for( int i = 0; i < number_days; ++i )
    utils::Properties::save( day_property[i], QString::number(m_widget_days[i]->isChecked()) );
}
