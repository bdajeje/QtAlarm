#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "widget/chrono_widget.hpp"
#include "widget/clock_widget.hpp"
#include "widget/sound_options.hpp"
#include "widget/snooze_options.hpp"

#include <QMainWindow>
#include <QtMultimedia/qmediaplayer.h>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:

		explicit MainWindow(QWidget *parent = 0);

  private:

		void createMenu();
		QMenu* createTrayMenu();
		QAction* createQuitAction();
		QString fileToPlay();
		QString addApplicationPath(QString path);
		void raiseWindow();
		void defaultToolTip();
		QString remainingTimeStr( const TimeWidget* time_widget );

  private:

		QSystemTrayIcon* m_tray_icon;
		QMediaPlayer*    m_media_player;
		SoundOptions*    m_sound_options;
		SnoozeOptions*   m_snooze_options;
		QAction*         m_action_stop_alarm;
		ClockWidget*     m_alarm_tab;
		ChronoWidget*    m_chrono_tab;
		bool             m_stopped {true};

  public slots:

		void timeout();
		void showAbout();
		void showSoundOptions();
		void showSnoozeOptions();
		void toggleWindowVisibility(QSystemTrayIcon::ActivationReason = QSystemTrayIcon::Trigger);
		void increaseVolume();
		void stopSound();
		void alarmStatusChanged(QMediaPlayer::State status);
		void alarmStarted();
		void alarmStopped();
		void countdownUpdated();
		void tabChanged(int index);
		void closeWindow();
		void updateTitle(QString);

  signals:

		void menuStopSound();
};

#endif // MAINWINDOW_HPP
