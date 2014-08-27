#ifndef TIME_WIDGET_HPP
#define TIME_WIDGET_HPP

#include <QCheckBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class TimeWidget : public QWidget
{
  Q_OBJECT

  public:

    explicit TimeWidget(QWidget *parent = 0);

    virtual ~TimeWidget() {}

  protected:

    virtual int timeToWait() const = 0;
    virtual void saveValues() const = 0;
    void cancelState();
    void startState();
    void countdownReached();
    void updateProgressText();

  protected:

    QVBoxLayout*  m_main_layout;
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

  public slots:

    /*! Toggle state for enable/disable */
    void toggleState();

    /*! Call every second when timer is active to update UI time related */
    void updateTime();

  signals:

    void timeout();
    void stopped();
};

#endif // TIME_WIDGET_HPP
