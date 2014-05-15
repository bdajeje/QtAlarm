#include "mainwindow.hpp"

#include <utils/properties.hpp>
#include <widget/time_widget.hpp>

#include <QMessageBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  auto central_widget = new QWidget(this);
  auto main_layout = new QVBoxLayout();

  // Create tab widget
  auto widget_tab = new QTabWidget;

  // First tab
  auto tab1 = new TimeWidget(TimeWidget::Type::Decrease);
  widget_tab->addTab(tab1, tr("Chrono"));

  // Second tab
  auto tab2 = new TimeWidget(TimeWidget::Type::Increase);
  widget_tab->addTab(tab2, tr("Alarm"));

  // Window icon
  const QIcon icon("../resources/icon.png");
  setWindowIcon(icon);

  // System tray
  m_tray_icon = new QSystemTrayIcon(icon, this);
  m_tray_icon->show();
  //m_tray_icon->setToolTip(tr("Alarm"));

  // Media player
  m_media_player = new QMediaPlayer(this);

  // Widget placement
  main_layout->addWidget(widget_tab);
  central_widget->setLayout(main_layout);
  setCentralWidget(central_widget);

  createMenu();

  // Connections
  connect(tab1, SIGNAL(timeout()), this, SLOT(timeout()));
  connect(tab2, SIGNAL(timeout()), this, SLOT(timeout()));
}

MainWindow::~MainWindow()
{
  delete m_sound_options;
}

void MainWindow::timeout()
{
  // Show notification
  m_tray_icon->showMessage(tr("Alarm"), tr("It's time"));

  // Play sound
  m_media_player->setMedia(QUrl::fromLocalFile( utils::Properties::get(utils::Property::AlarmFile) ));
  m_media_player->setVolume( utils::Properties::get(utils::Property::AlarmVolume).toInt() );
  m_media_player->play();
}

void MainWindow::createMenu()
{
  // File
  auto menu_file = menuBar()->addMenu(tr("&File"));
    // Quit
    auto action_quit = new QAction(tr("&Quit"), this);
    action_quit->setStatusTip(tr("Quit application"));
    action_quit->setIcon(QIcon("../resources/quit.png"));
    connect(action_quit, SIGNAL(triggered()), this, SLOT(close()));
    menu_file->addAction(action_quit);

  // Options
  auto menu_options = menuBar()->addMenu(tr("&Options"));
    // Sounds
    auto action_sounds = new QAction(tr("&Sounds"), this);
    action_sounds->setStatusTip(tr("Update sound options"));
    action_sounds->setIcon(QIcon("../resources/sounds.jpg"));
    connect(action_sounds, SIGNAL(triggered()), this, SLOT(showSoundOptions()));
    menu_options->addAction(action_sounds);

  // Help
  auto menu_help = menuBar()->addMenu(tr("&Help"));
    // About
    auto action_about = new QAction(tr("&About"), this);
    action_about->setStatusTip(tr("Information about this application"));
    action_about->setIcon(QIcon("../resources/about.png"));
    connect(action_about, SIGNAL(triggered()), this, SLOT(showAbout()));
    menu_help->addAction(action_about);
}

void MainWindow::showAbout()
{
  QMessageBox::information(this, tr("About"), tr("Todo"));
}

void MainWindow::showSoundOptions()
{
  if( !m_sound_options )
    m_sound_options = new SoundOptions();

  m_sound_options->show();
}
