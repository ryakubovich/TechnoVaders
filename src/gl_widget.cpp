#include "gl_widget.hpp"

#include <QPainter>
#include <QPaintEngine>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QCoreApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QGuiApplication>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <cmath>

#include <chrono>
#include <iostream>

#include "main_window.hpp"

namespace
{

int constexpr kLeftDirection = 0;
int constexpr kRightDirection = 1;

bool IsLeftButton(Qt::MouseButtons buttons)
{
  return buttons & Qt::LeftButton;
}
bool IsLeftButton(QMouseEvent const * const e)
{
  return IsLeftButton(e->button()) || IsLeftButton(e->buttons());
}

bool IsRightButton(Qt::MouseButtons buttons)
{
  return buttons & Qt::RightButton;
}
bool IsRightButton(QMouseEvent const * const e)
{
  return IsRightButton(e->button()) || IsRightButton(e->buttons());
}

} // namespace

GLWidget::GLWidget(MainWindow * mw, QColor const & background)
  : m_mainWindow(mw), m_background(background)
{
  setMinimumSize(1024, 768);
  setFocusPolicy(Qt::StrongFocus);
  m_continueButton = new QPushButton("Continue", this);
  m_exitButton = new QPushButton("Exit", this);
  m_continueButton->move(600, 310);
  m_exitButton->move(800, 310);
  m_continueButton->setVisible(false);
  m_exitButton->setVisible(false);
  connect(m_continueButton, SIGNAL(clicked()), this, SLOT(OnWinnerContinueClicked()));
  connect(m_exitButton, SIGNAL(clicked()), m_mainWindow, SLOT(OnWinnerExitClicked()));
}

GLWidget::~GLWidget()
{
  makeCurrent();
  delete m_textureAlien;
  delete m_texturePlayer;
  delete m_textureBullet;
  delete m_texturedRect;
  doneCurrent();
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  m_texturedRect = new TexturedRect();
  m_texturedRect->Initialize(this);
  TLevelData LD = GetLevelData(m_levelNumber);
  m_space1 = new Space(stof(LD["pHealth"]), stoi(LD["pLives"]), LD["pGunName"], stoi(LD["pGunHolderAmmo"]),
      stof(LD["pGunBulletCaliber"]), stof(LD["pGunBulletVelocity"]), stof(LD["pGunMissileCaliber"]), stof(LD["pGunMissileVelocity"]),
      stof(LD["pGunLimit"]), stoi(LD["aNumber"]), stof(LD["aHealth"]), LD["aGunName"], stoi(LD["aGunHolderAmmo"]),
      stof(LD["aGunBulletCaliber"]), stof(LD["aGunBulletVelocity"]));

  m_textureAlien = new QOpenGLTexture(QImage("data/alien.png"));
  m_texturePlayer = new QOpenGLTexture(QImage("data/starship_good.png"));
  m_textureBullet = new QOpenGLTexture(QImage("data/bullet_good.png"));

  m_time.start();
}

void GLWidget::paintGL()
{
  int const elapsed = m_time.elapsed();

  static std::chrono::time_point<std::chrono::steady_clock> lastCallTimestamp = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastCallTimestamp);
  Update(duration.count() / 1000.0f);
  lastCallTimestamp = std::chrono::steady_clock::now();

  QPainter painter;
  painter.begin(this);
  painter.beginNativePainting();

  glClearColor(m_background.redF(), m_background.greenF(), m_background.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glFrontFace(GL_CW);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Render();

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);

  painter.endNativePainting();

  if (elapsed != 0)
  {
    QString framesPerSecond;
    framesPerSecond.setNum(m_frames / (elapsed / 1000.0), 'f', 2);
    painter.setPen(Qt::white);
    painter.drawText(20, 40, framesPerSecond + " fps");
  }

  if (m_finished)
  {
    QString screenText, levelText, scoreText;
    painter.setFont(QFont("Arial", 30));
    screenText = "Congratulations! You won the round!";
    QTextStream(&levelText) << "Level: " << m_levelNumber;
    QTextStream(&scoreText) << "Score: " << m_space1->GetPlayer().GetScore();
    painter.drawText(600, 200, screenText);
    painter.drawText(600, 235, levelText);
    painter.drawText(600, 270, scoreText);
  }

  painter.end();

  if (!(m_frames % 100))
  {
    m_time.start();
    m_frames = 0;
  }
  ++m_frames;
  update();
}

void GLWidget::resizeGL(int w, int h)
{
  m_screenSize.setWidth(w);
  m_screenSize.setHeight(h);
}

void GLWidget::Update(float elapsedSeconds)
{
  if (!m_finished)
  {
    if (m_directions[kLeftDirection]) m_space1->InputProcessing(InputType::MoveLeft, elapsedSeconds);
    else if (m_directions[kRightDirection]) m_space1->InputProcessing(InputType::MoveRight, elapsedSeconds);
    try
    {
      m_space1->Update();
    }
    catch(EndOfTheGameException)
    {
      m_finished = true;
      m_continueButton->setVisible(true);
      m_exitButton->setVisible(true);
    }
  }
}

void GLWidget::Render()
{
  m_texturedRect->Render(m_texturePlayer,
                         QVector2D(m_space1->GetPlayer().GetBox().GetCenter().x(),
                                   m_space1->GetPlayer().GetBox().GetCenter().y()),
                         QSize(90, 144), m_screenSize);
  for (auto const & playerBullet : m_space1->GetBM().GetPlayersBullets())
    m_texturedRect->Render(m_textureBullet,
                           QVector2D(playerBullet.GetBox().GetCenter().x(),
                                     playerBullet.GetBox().GetCenter().y()),
                           QSize(9, 32), m_screenSize);
  for (auto const & alien : m_space1->GetAI().GetAliens())
    m_texturedRect->Render(m_textureAlien,
                           QVector2D(alien.GetBox().GetCenter().x(),
                                     alien.GetBox().GetCenter().y()),
                           QSize(128, 128), m_screenSize);
}

void GLWidget::mousePressEvent(QMouseEvent * e)
{
  QOpenGLWidget::mousePressEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseDoubleClickEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsRightButton(e))
  {
    // ...
  }
}

void GLWidget::mouseMoveEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseMoveEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * e)
{
  QOpenGLWidget::mouseReleaseEvent(e);

  int const px = L2D(e->x());
  int const py = L2D(e->y());
  if (IsLeftButton(e))
  {
    // ...
  }
}

void GLWidget::wheelEvent(QWheelEvent * e)
{
  QOpenGLWidget::wheelEvent(e);

  int const delta = e->delta();
  int const px = L2D(e->x());
  int const py = L2D(e->y());
  // ...
}

void GLWidget::keyPressEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Left)
    m_directions[kLeftDirection] = true;
  else if (e->key() == Qt::Key_Right)
    m_directions[kRightDirection] = true;
  else if (e->key() == Qt::Key_Up)
    m_space1->InputProcessing(InputType::Shot, 0.0f);
}

void GLWidget::keyReleaseEvent(QKeyEvent * e)
{
  if (e->key() == Qt::Key_Left)
    m_directions[kLeftDirection] = false;
  else if (e->key() == Qt::Key_Right)
    m_directions[kRightDirection] = false;
}
