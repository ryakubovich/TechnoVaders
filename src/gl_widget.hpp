#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGraphicsView>
#include <QPushButton>
#include <QTime>
#include "textured_rect.hpp"
#include "space.hpp"
#include <string>

#include <array>

class MainWindow;

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  GLWidget(MainWindow * mw, QColor const & background);
  ~GLWidget();

protected:
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void initializeGL() override;

  void Update(float elapsedSeconds);
  void Render();

  void showEvent(QShowEvent * event) override;
  void mousePressEvent(QMouseEvent * e) override;
  void mouseDoubleClickEvent(QMouseEvent * e) override;
  void mouseMoveEvent(QMouseEvent * e) override;
  void mouseReleaseEvent(QMouseEvent * e) override;
  void wheelEvent(QWheelEvent * e) override;
  void keyPressEvent(QKeyEvent * e) override;
  void keyReleaseEvent(QKeyEvent * e) override;

private:
  int L2D(int px) const { return px * devicePixelRatio(); }

  MainWindow * m_mainWindow;
  QPushButton * m_continueButton = nullptr;
  QPushButton * m_exitButton = nullptr;
  bool m_finished = false;

  unsigned int m_frames = 0;
  QTime m_time;
  QColor m_background;
  QSize m_screenSize;

  QOpenGLTexture * m_textureAlien = nullptr;
  QOpenGLTexture * m_texturePlayer = nullptr;
  QOpenGLTexture * m_textureBullet = nullptr;
  QOpenGLTexture * m_textureMissile = nullptr;
  QOpenGLTexture * m_textureSubObstacle = nullptr;
  TexturedRect * m_texturedRect = nullptr;

  Space * m_space1;
  int m_levelNumber = 1;
  QVector2D m_position = QVector2D(200, 200);
  std::array<bool, 2> m_directions = {{ false, false }};
  std::array<bool, 2> m_missileDirections = {{ false, false }};

  using TLevelData = std::map<std::string, std::string>;
  TLevelData GetLevelData(int levelNumber)
  {
    TLevelData result;
    std::ifstream levelFile("data/levels/" + std::to_string(m_levelNumber));
    std::string parameter, value;
    if (levelFile.is_open())
    {
      while (levelFile.good())
      {
        levelFile >> parameter >> value;
        result[parameter] = value;
      }
    }
    else
    {
      levelFile.close();
      std::ofstream nextLevelFile("data/levels/" + std::to_string(levelNumber));
      std::ifstream prevLevelFile("data/levels/" + std::to_string(levelNumber - 1));
      while (prevLevelFile.good())
      {
        prevLevelFile >> parameter >> value;
        if (parameter == "aGunName" || parameter == "pGunName")
          result[parameter] = value;
        else if (parameter == "pLives" || parameter == "pGunHolderAmmo" || parameter == "aNumber")
          result[parameter] = std::to_string(stoi(value) * 11 / 10);
        else
          result[parameter] = std::to_string(stof(value) * 1.1f);
      }
      prevLevelFile.close();
      for (auto it = result.begin(); it != result.end(); ++it)
        nextLevelFile << it->first << it->second << std::endl;
      nextLevelFile.close();
    }
    Logger::Instance() << "Readed levelfile";
    return result;
  }

private slots:
  void OnWinnerContinueClicked()
  {
    m_levelNumber++;
    delete m_space1;
    TLevelData LD = GetLevelData(m_levelNumber);
    m_space1 = new Space(stof(LD["pHealth"]), stoi(LD["pLives"]), LD["pGunName"], stoi(LD["pGunHolderAmmo"]),
        stof(LD["pGunBulletCaliber"]), stof(LD["pGunBulletVelocity"]), stof(LD["pGunMissileCaliber"]), stof(LD["pGunMissileVelocity"]),
        stof(LD["pGunLimit"]), stoi(LD["aNumber"]), stof(LD["aHealth"]), LD["aGunName"], stoi(LD["aGunHolderAmmo"]),
        stof(LD["aGunBulletCaliber"]), stof(LD["aGunBulletVelocity"]), stof(LD["oWidth"]), stof(LD["oHeight"]), stof(LD["shotChance"]),
        this->width(), this->height());
    m_finished = false;
    m_continueButton->setVisible(false);
    m_exitButton->setVisible(false);
  }
};
