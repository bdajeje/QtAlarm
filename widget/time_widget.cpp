#include "time_widget.hpp"

#include "widget/jlabel.hpp"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QVBoxLayout>

#include <iostream> // \todo to remove

constexpr unsigned int one_hour   = 3600;
constexpr unsigned int one_minute = 60;

TimeWidget::TimeWidget(Type type, QWidget *parent)
  : QWidget(parent)
  , m_type(type)
{
  auto main_layout = new QVBoxLayout(this);

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
  time_label_layout->addWidget(widget_hours_label);
  time_label_layout->addWidget(widget_mins_label);
  time_label_layout->addWidget(widget_secs_label);
  auto time_input_layout = new QHBoxLayout;
  time_input_layout->addWidget(m_widget_hours_input);
  time_input_layout->addWidget(m_widget_mins_input);
  time_input_layout->addWidget(m_widget_secs_input);
  main_layout->addLayout(time_label_layout);
  main_layout->addLayout(time_input_layout);
  main_layout->addWidget(m_widget_progress);
  main_layout->addWidget(m_widget_button);

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
}

void TimeWidget::startState()
{
  // Get max value
  const auto time_to_wait = m_widget_hours_input->value() * one_hour +
                            m_widget_mins_input->value() * one_minute +
                            m_widget_secs_input->value();

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
  m_widget_progress->setValue((m_type == Type::Increase) ? 0 : time_to_wait);
  updateProgressText();
  m_widget_progress->setTextVisible(true);

  // Start the timer
  m_timer->start(1000); // In milliseconds
}

void TimeWidget::stopTimer()
{
  // Stop timer
  m_timer->stop();

  // Update UI
  toggleState();

  // Emit end of timer
  emit timeout();
}

void TimeWidget::updateTime()
{
  // Update progress bar value
  switch(m_type)
  {
    case Type::Decrease:
    {
      m_widget_progress->setValue( m_widget_progress->value() - 1 );
      if(m_widget_progress->value() == 0)
        stopTimer();
      break;
    }
    case Type::Increase:
    {
      m_widget_progress->setValue( m_widget_progress->value() + 1 );
      if(m_widget_progress->value() == m_widget_progress->maximum())
        stopTimer();
      break;
    }
  }

  // Update progress bar text
  updateProgressText();
}

void TimeWidget::updateProgressText()
{
  const auto remaining_time = m_widget_progress->value();
  const auto hours   = remaining_time / one_hour;
  const auto minutes = (remaining_time - (hours * one_hour)) / one_minute;
  const auto seconds = remaining_time - (hours * one_hour) - (minutes * one_minute);
  QString format;
  if(hours > 0)
    format.append( QString::number(hours) + tr(" hours ") );
  if(minutes > 0)
    format.append( QString::number(minutes) + tr(" mins ") );
  format.append( QString::number(seconds) + tr(" secs") );
  m_widget_progress->setFormat(format);
}
