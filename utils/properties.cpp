#include "properties.hpp"

#include <utils/files.hpp>

#include <iostream>
#include <vector>

#include <QStringList>

namespace utils {

namespace properties {
  const QString sound_file       = "sound_file";
  const QString sound_volume     = "sound_volume";
  const QString default_sound    = "sound_default";
  const QString alarm_hour       = "alarm_hour";
  const QString alarm_min        = "alarm_min";
  const QString alarm_sec        = "alarm_sec";
  const QString chrono_hour      = "chrono_hour";
  const QString chrono_min       = "chrono_min";
  const QString chrono_sec       = "chrono_sec";
  const QString repeat_monday    = "repeat_monday";
  const QString repeat_tuesday   = "repeat_tuesday";
  const QString repeat_wednesday = "repeat_wednesday";
  const QString repeat_thursday  = "repeat_thursday";
  const QString repeat_friday    = "repeat_friday";
  const QString repeat_saturday  = "repeat_saturday";
  const QString repeat_sunday    = "repeat_sunday";
  const QString volume_fading    = "volume_fading";
  const QString undefined        = "Undefined property";
}

Properties::Properties(QString filepath)
  : m_filepath(std::move(filepath))
{
  std::string content;
  if(!files::read(m_filepath.toStdString(), content) || content.empty())
    return;

  // Split line by line
  QString properties_content(content.c_str());
  QStringList lines = properties_content.split("\n");

  // Iterate over lines to find property pairs
  for( const QString& line : lines )
  {
    // Ignore comments
    if( line.startsWith("#") )
      continue;

    QStringList parts = line.split("=");
    if(parts.size() < 2)
      continue;

    m_properties[toProperty(parts.at(0))] = parts.at(1);
  }

  // We are now considered as valid property
  m_valid = true;

  // Put default values for empty fields
  verify( Property::DefaultSound, "resources/sounds/default.ogg" );
  verify( Property::Volume, "50" );
}

void Properties::verify(Property property, const QString& value)
{
  if( m_properties[property].isEmpty() )
    m_properties[property] = value;
}

Properties& Properties::instance()
{
  static Properties instance("resources/properties");
  return instance;
}

const QString& Properties::get( Property property )
{
  if( property == Property::Undefined )
    return properties::undefined;

  return instance().m_properties[property];
}

bool Properties::save( Property property, const QString& value )
{
  instance().m_properties[property] = value;

  return instance().save();
}

bool Properties::save()
{
  QString content;
  for( const auto it : m_properties )
    content += toString(it.first) + '=' + it.second + '\n';

  return files::create(m_filepath.toStdString(), content.toStdString());
}

Property toProperty(const QString& from)
{
  auto clean_from = from.toLower();

  if(clean_from == properties::sound_file)
    return Property::AlarmFile;
  else if(clean_from == properties::sound_volume)
    return Property::Volume;
  else if(clean_from == properties::default_sound)
    return Property::DefaultSound;
  else if(clean_from == properties::alarm_hour)
    return Property::ClockHour;
  else if(clean_from == properties::alarm_min)
    return Property::ClockMin;
  else if(clean_from == properties::alarm_sec)
    return Property::ClockSec;
  else if(clean_from == properties::chrono_hour)
    return Property::ChronoHour;
  else if(clean_from == properties::chrono_min)
    return Property::ChronoMin;
  else if(clean_from == properties::chrono_sec)
    return Property::ChronoSec;
  else if(clean_from == properties::repeat_monday )
    return Property::ClockRepeatMonday;
  else if(clean_from == properties::repeat_tuesday )
    return Property::ClockRepeatTuesday;
  else if(clean_from == properties::repeat_wednesday )
    return Property::ClockRepeatWednesday;
  else if(clean_from == properties::repeat_thursday )
    return Property::ClockRepeatThursday;
  else if(clean_from == properties::repeat_friday )
    return Property::ClockRepeatFriday;
  else if(clean_from == properties::repeat_saturday )
    return Property::ClockRepeatSaturday;
  else if(clean_from == properties::repeat_sunday )
    return Property::ClockRepeatSunday;
  else if(clean_from == properties::volume_fading )
    return Property::FadeVolume;

  std::cerr << "Unknown property: " << from.toStdString() << std::endl;
  return Property::Undefined;
}

const QString& toString(Property property)
{
  switch(property)
  {
    case Property::AlarmFile:            return properties::sound_file;
    case Property::Volume:          return properties::sound_volume;
    case Property::DefaultSound:         return properties::default_sound;
    case Property::ClockHour:            return properties::alarm_hour;
    case Property::ClockMin:             return properties::alarm_min;
    case Property::ClockSec:             return properties::alarm_sec;
    case Property::ChronoHour:           return properties::chrono_hour;
    case Property::ChronoMin:            return properties::chrono_min;
    case Property::ChronoSec:            return properties::chrono_sec;
    case Property::ClockRepeatMonday:    return properties::repeat_monday;
    case Property::ClockRepeatTuesday:   return properties::repeat_tuesday;
    case Property::ClockRepeatWednesday: return properties::repeat_wednesday;
    case Property::ClockRepeatThursday:  return properties::repeat_thursday;
    case Property::ClockRepeatFriday:    return properties::repeat_friday;
    case Property::ClockRepeatSaturday:  return properties::repeat_saturday;
    case Property::ClockRepeatSunday:    return properties::repeat_sunday;
    case Property::FadeVolume:           return properties::volume_fading;
    default:
    {
      std::cerr << "Unknown property: " << property << std::endl;
      return properties::undefined;
    }
  }
}

} // namespace utils
