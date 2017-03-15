#include "time_widget.hpp"

#include "widget/jlabel.hpp"

#include <iostream>

#include <QMessageBox>
#include <QHBoxLayout>
#include <QSpinBox>

TimeWidget::TimeWidget()
{
  m_main_layout = new QVBoxLayout(this);

  // Time inputs
  auto widget_hours_label = new JLabel(tr("Hours"));
  m_widget_hours_input = new QSpinBox;
  m_widget_hours_input->setMinimum(0);
  auto widget_mins_label = new JLabel(tr("Minutes"));
  m_widget_mins_input = new QSpinBox;
  m_widget_mins_input->setMinimum(0);
  auto widget_secs_label = new JLabel(tr("Seconds"));
  m_widget_secs_input = new QSpinBox;
  m_widget_secs_input->setMinimum(0);

  m_widget_progress = new QProgressBar;
  m_widget_progress->setMinimum(0);

  m_widget_button = new QPushButton;

  // Widgets placement
  auto time_label_layout = new QHBoxLayout;
  time_label_layout->setAlignment(Qt::AlignTop);
  time_label_layout->addWidget(widget_hours_label);
  time_label_layout->addWidget(widget_mins_label);
  time_label_layout->addWidget(widget_secs_label);
  auto time_input_layout = new QHBoxLayout;
  time_input_layout->setAlignment(Qt::AlignTop);
  time_input_layout->addWidget(m_widget_hours_input);
  time_input_layout->addWidget(m_widget_mins_input);
  time_input_layout->addWidget(m_widget_secs_input);
  m_main_layout->addLayout(time_label_layout);
  m_main_layout->addLayout(time_input_layout);
  m_main_layout->addWidget(m_widget_progress);
  m_main_layout->addWidget(m_widget_button);

  m_timer = new QTimer(this);

  // Events
  connect(widget_hours_label, SIGNAL(clicked()), m_widget_hours_input, SLOT(setFocus()));
  connect(widget_mins_label, SIGNAL(clicked()), m_widget_mins_input, SLOT(setFocus()));
  connect(widget_secs_label, SIGNAL(clicked()), m_widget_secs_input, SLOT(setFocus()));
  connect(m_widget_button, SIGNAL(pressed()), this, SLOT(toggleState()));
  connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));

  cancelState();
}

void TimeWidget::toggleState()
{
  m_active ? cancelState() : startState();
}

void TimeWidget::cancelState()
{
  m_active = false;
  m_widget_button->setText(tr("Start"));
  m_widget_progress->setValue(0);
  m_widget_progress->setTextVisible(false);
  m_timer->stop();

  emit stopped();
}

void TimeWidget::startState()
{
  // Save selected values
  saveValues();

  // Get max value
  const auto time_to_wait = timeToWait();

  // Protection against wrong value
  if( time_to_wait <= 0 )
  {
	QMessageBox::warning(this, tr("Wrong values"), tr("You need to specify a time superior to 0"));
	return;
  }

  // Set active and update button text
  m_active = true;
  m_widget_button->setText(tr("Cancel"));

  // Update the progresss bar
  m_widget_progress->setMaximum(time_to_wait);
  m_widget_progress->setValue(time_to_wait);
  updateProgressText();
  m_widget_progress->setTextVisible(true);

  // Start the timer
  m_timer->start(1000); // In milliseconds

  emit started();
}

void TimeWidget::countdownReached()
{
  // Stop timer
  m_timer->stop();

  // Update UI
  m_widget_button->setText(tr("STOP"));

  // Focus button for easy stop
  m_widget_button->setFocus();

  // Emit end of timer
  emit timeout();
}

void TimeWidget::updateTime()
{
  // Update progress bar value
  m_widget_progress->setValue( m_widget_progress->value() - 1 );
  if(m_widget_progress->value() == 0)
	countdownReached();

  // Update progress bar text
  updateProgressText();

  emit countdownUpdated(m_widget_progress->value());
}

void TimeWidget::updateProgressText()
{
  const auto times  = timesFromSeconds( remainingSeconds() );
  const int hours   = std::get<0>(times);
  const int minutes = std::get<1>(times);
  const int seconds = std::get<2>(times);

  m_widget_progress->setFormat( progressText(hours, minutes, seconds) );
}

std::tuple<unsigned int,unsigned int,unsigned int> TimeWidget::timesFromSeconds(unsigned int total_seconds)
{
  const auto hours   = total_seconds / secs_in_one_hour;
  const auto minutes = (total_seconds - (hours * secs_in_one_hour)) / secs_in_one_minute;
  const auto seconds = total_seconds - (hours * secs_in_one_hour) - (minutes * secs_in_one_minute);

  return std::make_tuple(hours, minutes, seconds);
}

QString TimeWidget::progressText(unsigned int hours, unsigned int minutes, unsigned int seconds, bool short_version)
{
  QString format;

  if(hours > 0)
  {
	int days = 0;
	while( hours > hours_in_one_day )
	{
	  hours -= hours_in_one_day;
	  days++;
	}

	if( days > 0 )
	  format.append( QString::number(days) + " " + (short_version ? tr("d") : tr("days")) + " " );

	format.append( QString::number(hours) + " " + (short_version ? tr("h") : tr("hours")) + " " );
  }

  if(minutes > 0)
	format.append( QString::number(minutes) + " " + (short_version ? tr("m") : tr("mins")) + " " );

  format.append( QString::number(seconds) + " " + (short_version ? tr("s") : tr("secs")) + " " );

  return format;
}
