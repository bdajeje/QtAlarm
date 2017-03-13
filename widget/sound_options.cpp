#include "sound_options.hpp"

#include "utils/properties.hpp"
#include "utils/strings.hpp"
#include "utils/icons_manager.hpp"

#include <QFileDialog>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>

SoundOptions::SoundOptions(QWidget *parent, Qt::WindowFlags flags)
  : QDialog(parent, flags)
{
  setWindowTitle(tr("Sound Options"));
  setWindowIcon(utils::IconsManager::get("sounds.jpg"));

  auto main_layout = new QVBoxLayout;

  m_widget_sound_label = new JLabel;
  m_widget_slider = new QSlider(Qt::Orientation::Horizontal);
  m_widget_slider->setTickInterval(1);
  m_widget_slider->setMinimum(0);
  m_widget_slider->setMaximum(100);
  m_widget_slider->setValue( utils::Properties::get(utils::Property::Volume).toInt() );

  auto widget_select_file      = new QPushButton(tr("Select specific sound"));
  auto widget_select_directory = new QPushButton(tr("Select directory of sounds"));
  m_widget_file_label          = new QLabel();

  // Volume fading
  m_widget_fading_label = new JLabel;
  m_widget_volume_fade = new QSlider(Qt::Orientation::Horizontal);
  m_widget_volume_fade->setMinimum(0);
  m_widget_volume_fade->setMaximum(120);
  m_widget_volume_fade->setTickInterval(1);
  m_widget_volume_fade->setValue( utils::Properties::get(utils::Property::FadeVolume).toInt() );

  // Adding widget to layout
  main_layout->addWidget(m_widget_sound_label);
  main_layout->addWidget(m_widget_slider);
  main_layout->addWidget(m_widget_fading_label);
  main_layout->addWidget(m_widget_volume_fade);
  main_layout->addWidget( new QLabel(tr("Selected sound(s):")) );
  main_layout->addWidget(widget_select_file);
  main_layout->addWidget(widget_select_directory);
  main_layout->addWidget(m_widget_file_label);
  setLayout(main_layout);

  updateVolume(m_widget_slider->value());
  updateVolumeFading(m_widget_volume_fade->value());
  updateSelectFileUI(utils::Properties::get(utils::Property::AlarmFile));

  connect(m_widget_sound_label, SIGNAL(clicked()), m_widget_slider, SLOT(setFocus()));
  connect(m_widget_slider, SIGNAL(valueChanged(int)), this, SLOT(updateVolume(int)));
  connect(m_widget_volume_fade, SIGNAL(valueChanged(int)), this, SLOT(updateVolumeFading(int)));
  connect(m_widget_slider, SIGNAL(sliderReleased()), this, SLOT(saveVolume()));
  connect(widget_select_file, SIGNAL(pressed()), this, SLOT(selectFile()));
  connect(widget_select_directory, SIGNAL(pressed()), this, SLOT(selectDirectory()));
  connect(m_widget_volume_fade, SIGNAL(sliderReleased()), this, SLOT(saveVolumeFading()));
}

void SoundOptions::setSelectedPath(const QString& path)
{
  if(path.isEmpty())
	return;

  updateSelectFileUI(path);
  utils::Properties::save( utils::Property::AlarmFile, path );
}

void SoundOptions::selectFile()
{
  setSelectedPath( QFileDialog::getOpenFileName(this, tr("Choose File"), "", tr("Files (*.ogg *.mp3)")) );
}

void SoundOptions::selectDirectory()
{
  setSelectedPath( QFileDialog::getExistingDirectory(this, tr("Choose Directory")) );
}

void SoundOptions::updateSelectFileUI(const QString& filepath)
{
  m_widget_file_label->setText( utils::strings::fitIn(filepath.toStdString(), 40, utils::strings::Fit::Middle).c_str() );
}

void SoundOptions::updateVolume(int value)
{
  m_widget_sound_label->setText(tr("Sound volume: ") + QString::number(value) + "%");
}

void SoundOptions::saveVolume()
{
  utils::Properties::save( utils::Property::Volume, QString::number(m_widget_slider->value()) );
}

void SoundOptions::updateVolumeFading(int value)
{
  m_widget_fading_label->setText(tr("Reach max volume at: ") + QString::number(value) + " secs");
}

void SoundOptions::saveVolumeFading()
{
  utils::Properties::save( utils::Property::FadeVolume, QString::number(m_widget_volume_fade->value()) );
}

void SoundOptions::keyReleaseEvent(QKeyEvent* event)
{
  if( event->key() == Qt::Key_Escape )
	close();
  else
	QWidget::keyReleaseEvent(event);
}
