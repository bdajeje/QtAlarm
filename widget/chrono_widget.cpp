#include "chrono_widget.hpp"

#include <utils/properties.hpp>

ChronoWidget::ChronoWidget()
{
  const QString& hour_str = utils::Properties::get( utils::Property::ChronoHour );
  const QString& min_str  = utils::Properties::get( utils::Property::ChronoMin );
  const QString& sec_str  = utils::Properties::get( utils::Property::ChronoSec );

  if( !hour_str.isEmpty() )
		m_widget_hours_input->setValue( hour_str.toInt() );
  if( !min_str.isEmpty() )
		m_widget_mins_input->setValue( min_str.toInt() );
  if( !sec_str.isEmpty() )
		m_widget_secs_input->setValue( sec_str.toInt() );

	m_note_area = new QTextEdit;
	m_note_area->setPlaceholderText(tr("You can write a note here"));
	m_main_layout->insertWidget(m_main_layout->indexOf(m_widget_progress), m_note_area, -1);
}

int ChronoWidget::timeToWait() const
{
  return m_widget_hours_input->value() * secs_in_one_hour +
				 m_widget_mins_input->value() * secs_in_one_minute +
				 m_widget_secs_input->value();
}

void ChronoWidget::saveValues() const
{
  utils::Properties::save( utils::Property::ChronoHour, QString::number(m_widget_hours_input->value()) );
  utils::Properties::save( utils::Property::ChronoMin, QString::number(m_widget_mins_input->value()) );
  utils::Properties::save( utils::Property::ChronoSec, QString::number(m_widget_secs_input->value()) );
}
