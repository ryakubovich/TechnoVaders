#pragma once

#include <QListWidget>
#include "logger.hpp"
#include "json/json.h"
#include "json/value.h"
#include "gl_widget.hpp"
#include <fstream>
#include <map>
#include <string>
#include <QComboBox>
#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include <QOpenGLWidget>
#include <QStackedWidget>
#include <iostream>

using TSettings = std::map<std::string, std::string>;
typedef void (QWidget::*QWidgetVoidSlot)();

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  void AfterWonRound(int levelNumber, float score);

private:
  QTimer * m_timer = nullptr;
  QGridLayout * m_layout = nullptr;
  QOpenGLWidget * m_glWidget = nullptr;
  QWidget * m_mainWidget = new QWidget();
  QWidget * m_settingsWidget = new QWidget();
  QStackedWidget * m_stack = new QStackedWidget();
  QWidget * m_winnerPage = new QWidget();

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
  void OnMainLaunchClicked()
  {
    if (m_glWidget == nullptr)
    {
      std::string resolution;
      if ((resolution = m_settingsWidget->findChild<QComboBox *>("resolution")->currentText().toStdString()) == "Full screen")
        setWindowState(windowState() | Qt::WindowFullScreen);
//      else
//      {
//        std::size_t xPos = resolution.find('x');
//        int width = std::stoi(resolution.substr(0, xPos));
//        int height = std::stoi(resolution.substr(xPos + 1));
//      }
      m_glWidget = new GLWidget(this, qRgb(20, 20, 50));
      m_timer = new QTimer(m_glWidget);
      m_timer->setInterval(10);
    }
//    m_glWidget->window()->setWindowTitle("Space Invaders");
//    setCentralWidget(m_glWidget);
    m_stack->addWidget(m_glWidget);
    m_stack->setCurrentIndex(2);
    connect(m_timer, &QTimer::timeout, m_glWidget, static_cast<QWidgetVoidSlot>(&QWidget::update));
    m_timer->start();
  }
  void OnSettingsBackClicked() { m_stack->setCurrentIndex(0); }
  void OnMainExitClicked()
  {
    if (!WriteSettings()) Logger::Instance() << "Unsuccessful write of the settings file";
    close();
  }
  void OnWinnerContinueClicked()
  {
    m_stack->setCurrentIndex(2);
    m_stack->removeWidget(m_winnerPage);
    delete m_winnerPage;
    m_winnerPage = nullptr;
  }
  void OnWinnerExitClicked()
  {
    m_stack->setCurrentIndex(0);
    delete m_glWidget;
    m_glWidget = nullptr;
  }
  void OnGameFinished()
  {
    setWindowState(windowState() ^ Qt::WindowFullScreen);
    m_stack->setCurrentIndex(0);
  }
};
