#include "main_window.hpp"

#include <QApplication>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <cstdlib>
#include <iostream>

typedef void (QWidget::*QWidgetVoidSlot)();

MainWindow::MainWindow()
{
  // TO DO: more styling
  TSettings loadedSettings = LoadSettings();
  Logger::Instance() << "Loaded";
  for (auto it = loadedSettings.begin(); it != loadedSettings.end(); ++it)
    Logger::Instance() << it->first.c_str() << it->second.c_str();
  QPushButton * launchButton = new QPushButton("Launch", m_mainWidget);
  QPushButton * settingsButton = new QPushButton("Settings", m_mainWidget);
  QPushButton * exitButton = new QPushButton("Exit", m_mainWidget);
  launchButton->move(250, 100);
  settingsButton->move(250, 270);
  exitButton->move(250, 440);
  connect(launchButton, SIGNAL(clicked()), this, SLOT(OnMainLaunchClicked()));
  connect(settingsButton, SIGNAL(clicked()), this, SLOT(OnMainSettingsClicked()));
  connect(exitButton, SIGNAL(clicked()), this, SLOT(OnMainExitClicked()));

  QPushButton * backButton = new QPushButton("Back", m_settingsWidget);
  backButton->move(0, 0);
  connect(backButton, SIGNAL(clicked()), this, SLOT(OnSettingsBackClicked()));
  QGroupBox * graphics = new QGroupBox("Graphics settings", m_settingsWidget);
  QGroupBox * gameSettings = new QGroupBox("Game settings", m_settingsWidget);
  m_settingsWidget->setStyleSheet("QGroupBox { color: white; font-weight: bold; }");
//  QGroupBox * controlSettings = new QGroupBox("Controls", m_settingsWidget);
  // TO DO: add control settings
  QComboBox * resolutionChoice = new QComboBox(m_settingsWidget);
  resolutionChoice->setObjectName("resolution");
  resolutionChoice->addItems({ "640x480", "800x600", "1024x768", "1280x720", "1366x768", "Full screen"});
  resolutionChoice->setCurrentText(QString(loadedSettings.find("Resolution")->second.c_str()));
  QFormLayout * graphicsLayout = new QFormLayout(m_settingsWidget);
  QLabel * labelRes, * labelDif, * labelPS;
  labelRes = new QLabel();
  labelDif = new QLabel();
  labelPS = new QLabel();
  labelRes->setText("Resolution");
  labelRes->setStyleSheet("color: white; font-weight: bold;");
  graphicsLayout->addRow(labelRes, resolutionChoice);
  graphicsLayout->setFormAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  graphics->setLayout(graphicsLayout);
  graphics->resize(this->width() - 100, 100);
  graphics->move(10, 100);

  QFormLayout * gameSettingsLayout = new QFormLayout(m_settingsWidget);
  QComboBox * difficultyChoice = new QComboBox(m_settingsWidget);
  difficultyChoice->setObjectName("difficulty");
  difficultyChoice->addItems({ "Low", "Medium", "High" });
  difficultyChoice->setCurrentText(QString(loadedSettings.find("Difficulty")->second.c_str()));
  QSpinBox * playerSpeedChoice = new QSpinBox(m_settingsWidget);
  playerSpeedChoice->setObjectName("playerSpeed");
  playerSpeedChoice->setRange(1, 20);
  playerSpeedChoice->setValue(10);
  labelDif->setText("Difficulty");
  labelDif->setStyleSheet("color: white; font-weight: bold;");
  gameSettingsLayout->addRow(labelDif, difficultyChoice);
  labelPS->setText("Player speed");
  labelPS->setStyleSheet("color: white; font-weight: bold;");
  gameSettingsLayout->addRow(labelPS, playerSpeedChoice);
  gameSettingsLayout->setFormAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  gameSettings->setLayout(gameSettingsLayout);
  gameSettings->resize(this->width() - 100, 100);
  gameSettings->move(10, 250);

//  QFormLayout * controlLayout = new QFormLayout(m_settingsWidget);
//  QButtonGroup * buttonGroup = new QButtonGroup(m_settingsWidget);
//  QPushButton * moveLeftButton = new QPushButton(m_settingsWidget);
//  controlLayout->addRow("Move left", moveLeftButton);
//  QPushButton * moveRightButton = new QPushButton(m_settingsWidget);
//  controlLayout->addRow("Move right", moveRightButton);
//  buttonGroup->addButton(moveLeftButton);
//  buttonGroup->addButton(moveRightButton);
//  connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnSettingsControlButtonClicked(int)));
//  controlLayout->setFormAlignment(Qt::AlignHCenter | Qt::AlignCenter);
//  controlSettings->setLayout(controlLayout);
//  controlSettings->resize(this->width() - 100, 100);
//  controlSettings->move(10, 400);

  QPixmap background("data/background.jpg");
  background = background.scaled(this->size());
  QPalette palette;
  palette.setBrush(QPalette::Background, background);
  setPalette(palette);

  m_stack->addWidget(m_mainWidget);
  m_stack->addWidget(m_settingsWidget);
  m_stack->setCurrentIndex(0);
  setCentralWidget(m_stack);
  setMinimumSize(600, 600);
  setFocusPolicy(Qt::StrongFocus);
}
