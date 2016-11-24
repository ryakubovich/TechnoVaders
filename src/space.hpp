#pragma once

#include <vector>
#include "ammo.hpp"
#include "obstacle.hpp"
#include "ai.hpp"
#include "player.hpp"

class Space
{
public:
  void Update()
  {
    m_bm.Update();
    m_ai.Update();
    IntersectionCheck();
    m_ai.Shot();
  }

  void InputProcessing();

private:
  Player m_playerOne;
//  Player m_playerTwo;
  std::vector<Obstacle> m_obstacles;
  BulletManager m_bm;
  AI m_ai;

  void IntersectionCheck()
  {
    auto playersBullets = m_bm.GetPlayersBullets();
    auto aliens = m_ai.GetAliens();
    for (auto pit = playersBullets.begin(); pit != playersBullets.end(); ++pit)
      for (auto ait = aliens.begin(); ait != aliens.end(); ++ait)
      {
        if (pit->GetBox().IsBoxIntersectingBox(ait->GetBox()))
        {
          // Process hit, delete bullet
          // TO DO: Accumulate missile score
          ait->Hit();
          m_bm.DeleteBullet(true, pit);
          break;
        }
      }
    auto aliensBullets = m_bm.GetAliensBullets();
    for (auto ait = aliensBullets.begin(); ait != aliensBullets.end(); ++ait)
    {
      for (auto oit = m_obstacles.begin(); oit != m_obstacles.end(); ++oit)
      {
        if (ait->GetBox().IsBoxIntersectingBox(oit->GetOverallBox()))
        {
          oit->Hit(ait->GetBox());
          m_bm.DeleteBullet(false, ait);
          break;
        }
      }
      if ((ait->GetBox()).IsBoxIntersectingBox(m_playerOne.GetBox()))
      {
        m_playerOne.Hit();
        m_bm.DeleteBullet(false, ait);
      }
    }
  }
};
