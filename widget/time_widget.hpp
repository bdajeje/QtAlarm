#ifndef TIME_WIDGET_HPP
#define TIME_WIDGET_HPP

#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>

class TimeWidget final : public QWidget
{
  Q_OBJECT

  public:

    /*! Possible behavior for the progress bar */
    enum class Type { Increase, Decrease };

    explicit TimeWidget(Type type, QWidget *parent = 0);  

  private:

    void cancelState();
    void startState();    
    void stopTimer();
    void updateProgressText();

  private:

    QPushButton*  m_widget_button;
    QProgressBar* m_widget_progress;
    QSpinBox*     m_widget_hours_input;
    QSpinBox*     m_widget_mins_input;
    QSpinBox*     m_widget_secs_input;
    QTimer*       m_timer;

    const Type m_type;

    /*! Current state (enabled/disabled) */
    bool m_active {false};

  public slots:

    /*! Toggle state for enable/disable */
    void toggleState();

    /*! Call every second when timer is active to update UI time related */
    void updateTime();

  signals:

    void timeout();

};

#endif // TIME_WIDGET_HPP
