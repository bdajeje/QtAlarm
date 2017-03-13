#ifndef SNOOZE_OPTIONS_H
#define SNOOZE_OPTIONS_H

#include <QDialog>
#include <QSpinBox>

class SnoozeOptions final : public QDialog
{
	Q_OBJECT

	public:

		SnoozeOptions(QWidget *parent = 0, Qt::WindowFlags flags = 0);

	public slots:

		void saveSnoozeTime(int value);

	private:

		QSpinBox* m_snooze_time;
};

#endif // SNOOZE_OPTIONS_H
