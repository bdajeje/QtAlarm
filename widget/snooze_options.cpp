#include "snooze_options.hpp"

#include "utils/properties.hpp"
#include "utils/icons_manager.hpp"

#include <QVBoxLayout>

SnoozeOptions::SnoozeOptions(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	setWindowTitle(tr("Sound Options"));
	setWindowIcon(utils::IconsManager::get("snooze.png"));

	auto main_layout = new QVBoxLayout;

	m_snooze_time = new QSpinBox;
	m_snooze_time->setPrefix("Snooze time: ");
	m_snooze_time->setSuffix(" mins");
	m_snooze_time->setMinimum(1);
	m_snooze_time->setMaximum(30);
	m_snooze_time->setValue( utils::Properties::get(utils::Property::SnoozeTime).toInt() );

	// Adding widget to layout
	main_layout->addWidget(m_snooze_time);
	setLayout(main_layout);

	connect(m_snooze_time, SIGNAL(valueChanged(int)), this, SLOT(saveSnoozeTime(int)));
}

void SnoozeOptions::saveSnoozeTime(int value)
{
  utils::Properties::save( utils::Property::SnoozeTime, QString::number(value) );
}
