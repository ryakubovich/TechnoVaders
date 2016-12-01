#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QGridLayout>
#include <QOpenGLWidget>
#include <QStackedWidget>

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

private slots:
  void OnMainSettingsClicked() { m_stack->setCurrentIndex(1); }
  void OnMainLaunchClicked() { if (m_glWidget != nullptr) setCentralWidget(m_glWidget); }
  void OnSettingsBackClicked() { m_stack->setCurrentIndex(0); }
};
