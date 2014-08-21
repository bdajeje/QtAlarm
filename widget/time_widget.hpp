#ifndef TIME_WIDGET_HPP
#define TIME_WIDGET_HPP

#include <QCheckBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>

class TimeWidget : public QWidget
{
  Q_OBJECT

  public:

    explicit TimeWidget(QWidget *parent = 0);

    virtual ~TimeWidget() {}

    bool isRepeat() const { return m_widget_repeat_box->isChecked(); }

  protected:

    virtual int timeToWait() = 0;
    void cancelState();
    void startState();
    void stopTimer();
    void updateProgressText();

  protected:

    QPushButton*  m_widget_button;
    QProgressBar* m_widget_progress;
    QSpinBox*     m_widget_hours_input;
    QSpinBox*     m_widget_mins_input;
    QSpinBox*     m_widget_secs_input;
    QCheckBox*    m_widget_repeat_box;
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
};

#endif // TIME_WIDGET_HPP
