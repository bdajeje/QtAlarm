#ifndef CLOCK_WIDGET_HPP
#define CLOCK_WIDGET_HPP

#include <array>

#include <QLabel>

#include "utils/properties.hpp"
#include "widget/time_widget.hpp"

class ClockWidget final : public TimeWidget
{
  Q_OBJECT

  public:

		ClockWidget();
		virtual ~ClockWidget() = default;

  protected:

		virtual int timeToWait() const override;
		virtual void saveValues() const override;
		virtual void countdownReached() override;
		QDateTime nextDateTime() const;
		QDateTime selectedDateTime() const;
		void restart();

  private slots:

		virtual void startState() override;
		virtual void cancelState() override;
		void snooze();
		void updateSnooze();

  protected:

		QPushButton* m_snooze_button;
		QLabel* m_snooze_text;
		QTimer m_snooze_timer;
		int m_remaining_snooze_time {0}; // seconds
		std::array<QCheckBox*, 7> m_widget_days;

		static constexpr short number_days {7};
		static const std::array<utils::Property, 7> day_property;
};

#endif // CLOCK_WIDGET_HPP
