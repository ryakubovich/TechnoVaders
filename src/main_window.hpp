#pragma once

#include <QListWidget>
#include "logger.hpp"
#include "json/json.h"
#include "json/value.h"
#include <fstream>
#include <map>
#include <string>
#include <QComboBox>
#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include <QOpenGLWidget>
#include <QStackedWidget>

using TSettings = std::map<std::string, std::string>;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();

private:
  QTimer * m_timer = nullptr;
  QGridLayout * m_layout = nullptr;
  QOpenGLWidget * m_glWidget = nullptr;
  QWidget * m_mainWidget = new QWidget();
  QWidget * m_settingsWidget = new QWidget();
  QStackedWidget * m_stack = new QStackedWidget();

  TSettings LoadSettings() const
  {
    Json::Value settings;
    TSettings result;
    std::ifstream file("data/settings.json");
    if (!file.is_open()) return result;
    file >> settings;
    file.close();
    Json::Value & common = settings["Common"];
    result["Resolution"] = common["Resolution"].asCString();
    result["Difficulty"] = common["Difficulty"].asCString();
    // TO DO: add control settings load
    return result;
  }

  bool WriteSettings()
  {
    Json::Value settings;
    std::ofstream file("data/settings.json");
    if (file.is_open())
    {
      auto & common = settings["Common"];
      common["Resolution"] = Json::Value(m_settingsWidget->findChild<QComboBox *>("resolution")->currentText().toStdString());
      common["Difficulty"] = Json::Value(m_settingsWidget->findChild<QComboBox *>("difficulty")->currentText().toStdString());
      Json::StyledWriter styledWriter;
      file << styledWriter.write(settings);
      file.close();
      return true;
    }
    return false;
  }

private slots:
  void OnMainSettingsClicked() { m_stack->setCurrentIndex(1); }
  void OnMainLaunchClicked() { if (m_glWidget != nullptr) setCentralWidget(m_glWidget); }
  void OnSettingsBackClicked() { m_stack->setCurrentIndex(0); }
  void OnMainExitClicked()
  {
    if (!WriteSettings()) Logger::Instance() << "Unsuccessful write of the settings file";
    close();
  }
};
