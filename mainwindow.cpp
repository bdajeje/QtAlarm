#include "mainwindow.hpp"

#include <iostream>

#include <utils/files.hpp>
#include <utils/math.hpp>
#include <utils/properties.hpp>
#include <widget/clock_widget.hpp>
#include <widget/chrono_widget.hpp>

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
  auto main_layout = new QVBoxLayout();

  // Create tab widget
  auto widget_tab = new QTabWidget;

  // First tab
  auto alarm_tab = new ClockWidget();
  widget_tab->addTab(alarm_tab, tr("Alarm"));

  // Second tab
  auto chrono_tab = new ChronoWidget();
  widget_tab->addTab(chrono_tab, tr("Chrono"));

  // Window icon
  const QIcon icon("resources/images/icon.png");
  setWindowIcon(icon);

  // System tray
  m_tray_icon = new QSystemTrayIcon(icon, this);
  m_tray_icon->show();
  m_tray_icon->setToolTip(tr("Alarm"));
  m_tray_icon->setContextMenu( createTrayMenu() );

  // Media player
  m_media_player = new QMediaPlayer(this);

  // Sound options
  m_sound_options = new SoundOptions(this, Qt::Dialog);
  m_sound_options->setWindowModality(Qt::ApplicationModal);

  // Widget placement
  main_layout->addWidget(widget_tab);
  central_widget->setLayout(main_layout);
  setCentralWidget(central_widget);

  createMenu();

  // Connections
  connect(chrono_tab, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(alarm_tab, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(chrono_tab, SIGNAL(stopped()), this, SLOT(stopSound()));
  connect(alarm_tab, SIGNAL(stopped()), this, SLOT(stopSound()));
  connect(m_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(toggleWindowVisibility(QSystemTrayIcon::ActivationReason)));
  connect(m_media_player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(alarmStatusChanged(QMediaPlayer::State)));
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
  // Bring window in front of the screen
  raiseWindow();

  // Starting new alarm
  m_stopped = false;

  // Show notification
  m_tray_icon->showMessage(tr("Alarm"), tr("It's time"));

  // Find media to play
  QString file_to_play = addApplicationPath( fileToPlay() );
  m_media_player->setMedia(QUrl::fromLocalFile( file_to_play ));

  // Set volume
  auto increase_time = utils::Properties::get(utils::Property::FadeVolume).toInt();
  if( increase_time == 0 )
  {
    m_media_player->setVolume( utils::Properties::get(utils::Property::Volume).toInt() );
  }
  else
  {
    m_media_player->setVolume( 0 );
    increaseVolume();
  }

  // Start alarm sound
  m_media_player->play();  std::cout << "playing: " << file_to_play.toStdString() << " at " << m_media_player->volume() << std::endl;
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
    // Quit
    menu_file->addAction( createQuitAction() );

  // Options
  auto menu_options = menuBar()->addMenu(tr("&Options"));
    // Sounds
    auto action_sounds = new QAction(tr("&Sounds"), this);
    action_sounds->setStatusTip(tr("Update sound options"));
    action_sounds->setIcon(QIcon("resources/images/sounds.jpg"));
    connect(action_sounds, SIGNAL(triggered()), this, SLOT(showSoundOptions()));
    menu_options->addAction(action_sounds);

  // Help
  auto menu_help = menuBar()->addMenu(tr("&Help"));
    // About
    auto action_about = new QAction(tr("&About"), this);
    action_about->setStatusTip(tr("Information about this application"));
    action_about->setIcon(QIcon("resources/images/about.png"));
    connect(action_about, SIGNAL(triggered()), this, SLOT(showAbout()));
    menu_help->addAction(action_about);
}

QMenu* MainWindow::createTrayMenu()
{
  QMenu* tray_menu = new QMenu();
  tray_menu->addAction( createQuitAction() );
  return tray_menu;
}

QAction* MainWindow::createQuitAction()
{
  auto action_quit = new QAction(tr("&Quit"), this);
  action_quit->setStatusTip(tr("Quit application"));
  action_quit->setIcon(QIcon("resources/images/quit.png"));
  connect(action_quit, SIGNAL(triggered()), this, SLOT(close()));
  return action_quit;
}

void MainWindow::showAbout()
{
  QMessageBox::information(this, tr("About"), tr("About_text"));
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
{ std::cout << "increase volume" << std::endl;
  const auto fade_time = utils::Properties::get( utils::Property::FadeVolume ).toFloat(); std::cout << "fade time: " << fade_time << std::endl;
  if( fade_time <= 0 )
    return;

  // Raise volume
  const auto raise = utils::Properties::get( utils::Property::Volume ).toFloat() / fade_time;
  m_media_player->setVolume( m_media_player->volume() + raise );

  std::cout << "volume is now set to : " << m_media_player->volume() << std::endl;

  // Are we done with slowly inscreasing?
  if( m_media_player->volume() >= 100 )
    return;
std::cout << "starting again" << std::endl;
  // Update every second
  QTimer::singleShot(1000, this, SLOT(increaseVolume()));
}

void MainWindow::alarmStatusChanged(QMediaPlayer::State state)
{ std::cout << "status changed: " << state << std::endl;
  if( !m_stopped && state == QMediaPlayer::State::StoppedState )
  {
    std::cout << "relaunching" << std::endl;
    timeout();
  }
}
