#ifndef RESOURCES_MANAGER_HPP
#define RESOURCES_MANAGER_HPP

#include <QApplication>
#include <QString>

#include <map>

namespace utils {

template<class T>
class ResourcesManager
{
  public:

    virtual ~ResourcesManager() {}

    static ResourcesManager& instance()
    {
      static ResourcesManager instance(QApplication::applicationDirPath() + "/resources/images/");
      return instance;
    }

    static T& get(const QString& name)
    {
      auto& resources = instance().m_resources;

      auto found = resources.find(name);
      if(found != resources.end())
        return found->second;

      resources[name] = T(instance().m_filepath + name);
      return resources[name];
    }

  protected:

    ResourcesManager(QString filepath)
      : m_filepath{ filepath }
    {}

  protected:

    /* Map a resource name to the resource */
    std::map<QString, T> m_resources;

    QString m_filepath;
};

} // utils

#endif // RESOURCES_MANAGER_HPP
