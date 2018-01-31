#include "clock_widget.hpp"

#include <iostream>

#include <QLabel>
#include <QTime>

#include "widget/jlabel.hpp"

const std::array<utils::Property, 7> ClockWidget::day_property { {
																	 utils::Property::ClockRepeatMonday,
																   utils::Property::ClockRepeatTuesday,
																   utils::Property::ClockRepeatWednesday,
																   utils::Property::ClockRepeatThursday,
																   utils::Property::ClockRepeatFriday,
																   utils::Property::ClockRepeatSaturday,
																   utils::Property::ClockRepeatSunday } };

ClockWidget::ClockWidget()
{
  // Create objects
  m_snooze_button = new QPushButton("Snooze");
  m_snooze_button->setEnabled(false);
	m_snooze_text    = new QLabel;
  auto days_layout = new QHBoxLayout();

  const char* days[] = {"M", "T", "W", "T", "F", "S", "S"};
  for( size_t i = 0; i < number_days; ++i )
  {
		auto day_layout  = new QVBoxLayout();
		auto repeat_day  = new QCheckBox();
		auto label       = new JLabel(days[i]);

		m_widget_days[i] = repeat_day;
		day_layout->addWidget( label );
		day_layout->addWidget(repeat_day);
		days_layout->addLayout(day_layout);

		connect(label, SIGNAL(clicked()), repeat_day, SLOT(click()));
  }

  m_main_layout->insertLayout(2, days_layout);
  m_main_layout->addWidget(m_snooze_button);
  m_main_layout->addWidget(m_snooze_text);

  // Set maximum values
  m_widget_hours_input->setMaximum(24);
  m_widget_mins_input->setMaximum(60);
  m_widget_secs_input->setMaximum(60);

  // Get last used values
	const QString& hour_str = utils::Properties::get( utils::Property::ClockHour );
	const QString& min_str  = utils::Properties::get( utils::Property::ClockMin );
	const QString& sec_str  = utils::Properties::get( utils::Property::ClockSec );

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

  m_snooze_timer.setSingleShot(true);

  connect(&m_snooze_timer, SIGNAL(timeout()), this, SLOT(updateSnooze()));
  connect(m_snooze_button, SIGNAL(pressed()), this, SLOT(snooze()));

  // Restart alarm if application has been shutdown while alarm was running
  restart();
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

void ClockWidget::startState()
{
  // Save currently running state so if computer shutdowns, we start time again on application start up
  utils::Properties::save( utils::Property::ClosedWhileRunning, "1" );

  TimeWidget::startState();
}

void ClockWidget::cancelState()
{
  m_snooze_button->setEnabled(false);
  utils::Properties::save( utils::Property::ClosedWhileRunning, "0" );
  TimeWidget::cancelState();
}

QDateTime ClockWidget::nextDateTime() const
{
  // Get next time, but date is not set
  auto next_date = selectedDateTime();
  bool found = false;

  // Try to find next day to trigger
  if( next_date < QDateTime::currentDateTime() )
		next_date = next_date.addDays(1);

  for( int i = 0; i < number_days; ++i )
  {
		int day_in_week = next_date.date().dayOfWeek() - 1; // Day in week starts at 1 so minus 1
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

void ClockWidget::restart()
{
  if( utils::Properties::get( utils::Property::ClosedWhileRunning).toInt() )
		startState();
}

void ClockWidget::countdownReached()
{
	m_snooze_button->setEnabled(true);
	TimeWidget::countdownReached();
}

void ClockWidget::snooze()
{
	m_snooze_button->setEnabled(false);
	m_remaining_snooze_time = utils::Properties::get(utils::Property::SnoozeTime).toInt() * 60 + 1;
	updateSnooze();
}

void ClockWidget::updateSnooze()
{
	m_remaining_snooze_time--;
	QString snooze_text;

	if(m_remaining_snooze_time > 0)
	{
		m_snooze_timer.setInterval(60000); // Wake up every second to update snooze text
		m_snooze_timer.start();
		int remaining_mins = m_remaining_snooze_time / 60.f;
		snooze_text = "Snooze finishes in " + QString::number(remaining_mins) + " mins";
	}
	else countdownReached();

	m_snooze_text->setText(snooze_text);
}
