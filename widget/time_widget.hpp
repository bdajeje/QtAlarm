#ifndef TIME_WIDGET_HPP
#define TIME_WIDGET_HPP

#include <QCheckBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

using TimeParts = std::tuple<unsigned int,unsigned int,unsigned int>;

class TimeWidget : public QWidget
{
  Q_OBJECT

  public:

		TimeWidget();
		virtual ~TimeWidget() = default;

		static QString progressText(const TimeParts& times, bool short_version = false);
		static TimeParts timesFromSeconds(unsigned int total_seconds);

		bool isActive() const { return m_active; }
		unsigned int remainingSeconds() const { return m_widget_progress->value(); }

	protected:

		virtual int timeToWait() const = 0;
		virtual void saveValues() const = 0;
		virtual void countdownReached();
		void updateProgressText(const TimeParts& times);
		void updateWindowTitle(const QString& title);

	protected slots:

		/*! Toggle state for enable/disable */
		void toggleState();

		/*! Call every second when timer is active to update UI time related */
		void updateTime();

		virtual void cancelState();
		virtual void startState();

	signals:

		void timeout();
		void stopped();
		void started();
		void countdownUpdated(int remaining_seconds);

  protected:

		QVBoxLayout*  m_main_layout;
		QHBoxLayout*  m_buttons_layout;
		QPushButton*  m_widget_button;
		QProgressBar* m_widget_progress;
		QSpinBox*     m_widget_hours_input;
		QSpinBox*     m_widget_mins_input;
		QSpinBox*     m_widget_secs_input;
		QTimer*       m_timer;

		/*! Current state (enabled/disabled) */
		bool m_active {false};

		static constexpr unsigned int secs_in_one_day    = 86400;
		static constexpr unsigned int secs_in_one_hour   = 3600;
		static constexpr unsigned int secs_in_one_minute = 60;
		static constexpr unsigned int hours_in_one_day   = 24;
};

#endif // TIME_WIDGET_HPP
