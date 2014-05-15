#include "properties.hpp"

#include <utils/files.hpp>

#include <vector>

#include <QStringList>

namespace utils {

namespace properties {
  const QString sound_file   = "sound_file";
  const QString sound_volume = "sound_volume";
  const QString undefined    = "Undefined property";
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
  verify( Property::AlarmFile, "../resources/alarm.ogg" );
  verify( Property::AlarmVolume, "50" );
}

void Properties::verify(Property property, const QString& value)
{
  if( m_properties[property].isEmpty() )
    m_properties[property] = value;
}

Properties& Properties::instance()
{
  static Properties instance("../resources/properties");
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
    return Property::AlarmVolume;

  return Property::Undefined;
}

const QString& toString(Property property)
{
  switch(property)
  {
    case Property::AlarmFile:   return properties::sound_file;
    case Property::AlarmVolume: return properties::sound_volume;
    default:                    return properties::undefined;

  }
}

} // namespace utils
