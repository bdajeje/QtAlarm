#include "mainwindow.hpp"

#include <iostream>

#include "utils/files.hpp"
#include "utils/math.hpp"
#include "utils/properties.hpp"
#include "utils/icons_manager.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMenuBar>
#include <QMessageBox>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  auto central_widget = new QWidget(this);
  auto main_layout    = new QVBoxLayout();

  // Create tab widget
  auto widget_tab = new QTabWidget;

  // First tab
  m_alarm_tab = new ClockWidget();
  widget_tab->addTab(m_alarm_tab, tr("Alarm"));

  // Second tab
  m_chrono_tab = new ChronoWidget();
  widget_tab->addTab(m_chrono_tab, tr("Chrono"));

  // Window icon
  const QIcon& icon = utils::IconsManager::get("icon.png");
  setWindowIcon( icon );

  // System tray
  m_tray_icon = new QSystemTrayIcon(icon, this);
  m_tray_icon->show();
  m_tray_icon->setContextMenu( createTrayMenu() );
  defaultToolTip();

  // Media player
  m_media_player = new QMediaPlayer(this);

  // Sound options
  m_sound_options = new SoundOptions(this, Qt::Dialog);
  m_sound_options->setWindowModality(Qt::ApplicationModal);

  // Widget placement
  main_layout->addWidget(widget_tab);
  central_widget->setLayout(main_layout);
  setCentralWidget(central_widget);

  // Open last used tab by default
  widget_tab->setCurrentIndex( utils::Properties::get(utils::Property::LastUsedTab).toInt() );

  // Menu
  createMenu();

  // Connections
  connect(m_chrono_tab, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(m_alarm_tab, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(m_chrono_tab, SIGNAL(stopped()), this, SLOT(alarmStopped()));
  connect(m_alarm_tab, SIGNAL(stopped()), this, SLOT(alarmStopped()));
  connect(m_chrono_tab, SIGNAL(started()), this, SLOT(alarmStarted()));
  connect(m_alarm_tab, SIGNAL(started()), this, SLOT(alarmStarted()));
  connect(m_chrono_tab, SIGNAL(countdownUpdated(int)), this, SLOT(countdownUpdated()));
  connect(m_alarm_tab, SIGNAL(countdownUpdated(int)), this, SLOT(countdownUpdated()));
  connect(this, SIGNAL(menuStopSound()), m_chrono_tab, SLOT(cancelState()));
  connect(this, SIGNAL(menuStopSound()), m_alarm_tab, SLOT(cancelState()));
  connect(m_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(toggleWindowVisibility(QSystemTrayIcon::ActivationReason)));
  connect(m_media_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(alarmStatusChanged(QMediaPlayer::State)));
  connect(widget_tab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

QString MainWindow::fileToPlay()
{
  QString filepath = utils::Properties::get(utils::Property::AlarmFile);

  // Randomly select a sound if path is a directory
  const QDir sound_directory = QDir(filepath);
  if( sound_directory.exists() )
  {
    // List all sounds in this directory and get a random one
    QStringList file_list = sound_directory.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);

    // Get only sound files
    static const QRegExp sounds_regex( "mp3|ogg" );
    file_list = file_list.filter(sounds_regex);

    // No usable file? Return default one
    if( file_list.isEmpty() )
    {
      std::cerr << "No playable sound in directory '" << filepath.toStdString() << "', falling back on default sound";
      return utils::Properties::get( utils::Property::DefaultSound );
    }

    // Randomly select a sound among the list of sounds
    return utils::files::ensureDirEnd(filepath) + file_list.at( utils::random(0, file_list.size() - 1) );
  }
  else if( !QFile::exists(filepath) )
  {
    std::cerr << "Invalid file '" << filepath.toStdString() << "', falling back on default sound";
    return utils::Properties::get( utils::Property::DefaultSound );
  }
  else return filepath;
}

void MainWindow::raiseWindow()
{
  showNormal();
  raise();
  activateWindow();
}

void MainWindow::timeout()
{
  // Starting new alarm
  m_stopped = false;

  // Bring window in front of the screen
  raiseWindow();

  // Restore default tooltip message
  defaultToolTip();

  // Show notification
  m_tray_icon->showMessage(tr("Alarm"), tr("It's time"));

  // Find media to play
  QString file_to_play = addApplicationPath( fileToPlay() );
  m_media_player->setMedia(QUrl::fromLocalFile( file_to_play ));

  // Set volume
  if( utils::Properties::get(utils::Property::FadeVolume).toInt() == 0 )
    m_media_player->setVolume( utils::Properties::get(utils::Property::Volume).toInt() );
  else
  {
    m_media_player->setVolume( 0 );
    increaseVolume();
  }

  // Start alarm sound
  m_media_player->play();
}

QString MainWindow::addApplicationPath(QString path)
{
  static const QString application_path = utils::files::ensureDirEnd(QCoreApplication::applicationDirPath());

  if( path.startsWith('/') )
    return path;

  return application_path + path;
}

void MainWindow::createMenu()
{
  // File
  auto menu_file = menuBar()->addMenu(tr("&File"));
    // Minimize
    auto action_minimized = new QAction(tr("&Minimize"), this);
    action_minimized->setStatusTip(tr("Minimize application in system tray"));
    action_minimized->setIcon(utils::IconsManager::get("minimize.png"));
    connect(action_minimized, SIGNAL(triggered()), this, SLOT(toggleWindowVisibility()));
    menu_file->addAction(action_minimized);
    // Quit
    menu_file->addAction( createQuitAction() );

  // Options
  auto menu_options = menuBar()->addMenu(tr("&Options"));
    // Sounds
    auto action_sounds = new QAction(tr("&Sounds"), this);
    action_sounds->setStatusTip(tr("Update sound options"));
    action_sounds->setIcon(utils::IconsManager::get("sounds.jpg"));
    connect(action_sounds, SIGNAL(triggered()), this, SLOT(showSoundOptions()));
    menu_options->addAction(action_sounds);

  // Help
  auto menu_help = menuBar()->addMenu(tr("&Help"));
    // About
    auto action_about = new QAction(tr("&About"), this);
    action_about->setStatusTip(tr("Information about this application"));
    action_about->setIcon(utils::IconsManager::get("about.png"));
    connect(action_about, SIGNAL(triggered()), this, SLOT(showAbout()));
    menu_help->addAction(action_about);
}

QMenu* MainWindow::createTrayMenu()
{
  QMenu* tray_menu = new QMenu();

  // Stop alarm
  m_action_stop_alarm = new QAction(tr("&Stop"), this);
  m_action_stop_alarm->setStatusTip(tr("Stop alarm"));
  m_action_stop_alarm->setIcon(utils::IconsManager::get("stop.ico"));
  m_action_stop_alarm->setEnabled(false);
  connect(m_action_stop_alarm, SIGNAL(triggered()), this, SIGNAL(menuStopSound()));
  tray_menu->addAction( m_action_stop_alarm );

  // Quit
  tray_menu->addAction( createQuitAction() );

  return tray_menu;
}

QAction* MainWindow::createQuitAction()
{
  auto action_quit = new QAction(tr("&Quit"), this);
  action_quit->setStatusTip(tr("Quit application"));
  action_quit->setIcon(utils::IconsManager::get("quit.png"));
  connect(action_quit, SIGNAL(triggered()), this, SLOT(close()));
  return action_quit;
}

void MainWindow::showAbout()
{
  static const QString about_text = R"MTEXT(
Alarm is a simple application which starts a sound whenever you want.
There are two different possible alarms:

1. Clock
Set a time when to trigger the alarm.
If there is no selected day, the alarm is triggered as soon as the specified time is reached.
If at least one day is specified then the alarm is be triggered at the next selected day.

2. Countdown
The alarm is triggered when the time reach 0 seconds.

To stop the alarm you simply need to push the stop button.
You can cancel the alarm at any time by clicking on the same button you used to start it.
You can also cancel the alarm from the system tray icon menu.

You can specified several options in the 'Options' menu:
- Volume: Set the volume of the alarm in percentage.
- Smooth alarm: Time to reach, in seconds, the maximum volume. You can disable this feature by setting a value of 0.
- Sound to play: Select a file to use as ringtone.
- Sound to play in directory: Select a directory where to randomly pick a sound to play as ringtone.
                                    )MTEXT";
  QMessageBox::information(this, tr("About"), about_text);
}

void MainWindow::showSoundOptions()
{
  m_sound_options->move( pos().x(), pos().y() );
  m_sound_options->show();
}

void MainWindow::toggleWindowVisibility(QSystemTrayIcon::ActivationReason reason)
{
  if( reason == QSystemTrayIcon::Trigger )
    isVisible() ? hide() : show();
}

void MainWindow::stopSound()
{
  m_stopped = true;
  m_media_player->stop();
}

void MainWindow::increaseVolume()
{
  if(m_stopped)
    return;

  const auto fade_time = utils::Properties::get( utils::Property::FadeVolume ).toFloat();
  if( fade_time <= 0 )
    return;

  // Raise volume
  const auto raise = utils::Properties::get( utils::Property::Volume ).toFloat() / fade_time;
  m_media_player->setVolume( m_media_player->volume() + raise );

  // Are we done with slowly inscreasing?
  if( m_media_player->volume() >= 100 )
    return;

  // Update every second
  QTimer::singleShot(1000, this, SLOT(increaseVolume()));
}

void MainWindow::alarmStatusChanged(QMediaPlayer::State state)
{
  if( !m_stopped && state == QMediaPlayer::State::StoppedState )
    timeout();
}

void MainWindow::alarmStarted()
{
  m_action_stop_alarm->setEnabled(true);
}

void MainWindow::alarmStopped()
{
  m_action_stop_alarm->setEnabled(false);
  stopSound();
}

void MainWindow::countdownUpdated()
{
  // Update progression in tooltip window
  QString text;

  if( m_alarm_tab->isActive() )
   text.append( tr("Alarm in ") + remainingTimeStr( m_alarm_tab ) + "\n" );

  if( m_chrono_tab->isActive() )
   text.append( tr("Chrono in ") + remainingTimeStr( m_chrono_tab ) + "\n" );

  m_tray_icon->setToolTip(text);
}

QString MainWindow::remainingTimeStr( const TimeWidget* time_widget )
{
  const auto times = TimeWidget::timesFromSeconds( time_widget->remainingSeconds() );
  return TimeWidget::progressText( std::get<0>(times), std::get<1>(times), std::get<2>(times) );
}

void MainWindow::defaultToolTip()
{
  m_tray_icon->setToolTip(tr("Alarm"));
}

void MainWindow::tabChanged(int index)
{
  utils::Properties::save(utils::Property::LastUsedTab, QString::number(index));
}
