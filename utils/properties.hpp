#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <map>
#include <string>

#include <QString>

namespace utils {

/*! Existing properties */
enum Property { AlarmFile, AlarmVolume, DefaultSound,
                ClockHour, ClockMin, ClockSec,
                ChronoHour, ChronoMin, ChronoSec,
                ClockRepeatMonday,
                ClockRepeatTuesday,
                ClockRepeatWednesday,
                ClockRepeatThursday,
                ClockRepeatFriday,
                ClockRepeatSaturday,
                ClockRepeatSunday,
                Undefined };

Property toProperty(const QString& from);
const QString& toString(Property property);

class Properties final
{
  public:

    static Properties& instance();
    static const QString& get( Property property );
    static bool save( Property property, const QString& value );
    static bool isValid() { return instance().m_valid; }

    const QString& filepath() const { return m_filepath; }

  private:

    Properties(QString filepath);
    bool save();
    void verify(Property property, const QString& value);

  private:

    std::map<Property, QString> m_properties;
    const QString m_filepath;
    bool m_valid {false};
};

} // namespace utils

#endif // PROPERTIES_HPP
