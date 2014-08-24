#include "chrono_widget.hpp"

#include <utils/properties.hpp>

ChronoWidget::ChronoWidget(QWidget *parent)
  : TimeWidget(parent)
{
  m_widget_repeat_box = new QCheckBox(tr("Repeat"));
  m_main_layout->insertWidget(2, m_widget_repeat_box);

  const QString& hour_str = utils::Properties::get( utils::Property::ChronoHour );
  const QString& min_str  = utils::Properties::get( utils::Property::ChronoMin );
  const QString& sec_str  = utils::Properties::get( utils::Property::ChronoSec );

  if( !hour_str.isEmpty() )
    m_widget_hours_input->setValue( hour_str.toInt() );
  if( !min_str.isEmpty() )
    m_widget_mins_input->setValue( min_str.toInt() );
  if( !sec_str.isEmpty() )
    m_widget_secs_input->setValue( sec_str.toInt() );
}

int ChronoWidget::timeToWait()
{
  return m_widget_hours_input->value() * secs_in_one_hour +
         m_widget_mins_input->value() * secs_in_one_minute +
         m_widget_secs_input->value();
}

void ChronoWidget::saveValues()
{
  utils::Properties::save( utils::Property::ChronoHour, QString::number(m_widget_hours_input->value()) );
  utils::Properties::save( utils::Property::ChronoMin, QString::number(m_widget_mins_input->value()) );
  utils::Properties::save( utils::Property::ChronoSec, QString::number(m_widget_secs_input->value()) );
}

bool ChronoWidget::isRepeat() const
{
  return m_widget_repeat_box->isChecked();
}
