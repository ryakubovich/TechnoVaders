#pragma once

#include <vector>
#include "ammo.hpp"
#include "obstacle.hpp"
// #include "ai.hpp"
// #include "player.hpp"

class Space
{
public:
  void Update()
  {
    m_ammoManager.Update();
    m_AI.Update();
    IntersectionCheck();
    m_AI.Shot();
  }

  void InputProcessing();

private:
  Player m_playerOne;
//  Player m_playerTwo;
  std::vector<Obstacle> m_obstacles;
  AmmoManager m_ammoManager;
  AI m_AI;

  void IntersectionCheck()
  {
    auto playersBullets = m_ammoManager.GetPlayersBullets();
    auto aliens = m_AI.GetAliens();
    for (auto pit = playersBullets.begin(); pit != playersBullets.end(); ++pit)
      for (auto ait = aliens.begin(); ait != aliens.end(); ++ait)
      {
        if (pit->GetBox().IsBoxIntersectingBox(ait->GetBox()))
        {
          // Process hit, delete bullet
          // TO DO: Accumulate missile score
          ait->Hit();
          m_ammoManager.DeleteBullet(true, pit);
          break;
        }
      }
    auto aliensBullets = m_ammoManager.GetAliensBullets();
    for (auto ait = aliensBullets.begin(); ait != aliensBullets.end(); ++ait)
    {
      for (auto oit = m_obstacles.begin(); oit != m_obstacles.end(); ++oit)
      {
        if (ait->GetBox().IsBoxIntersectingBox(oit->GetOverallBox()))
        {
          oit->Hit();
          m_ammoManager.DeleteBullet(false, ait);
          break;
        }
      }
      if (ait->GetBox().IsBoxIntersectingBox(m_playerOne.GetBox()))
      {
        m_playerOne.Hit();
        m_ammoManager.DeleteBullet(false, ait);
      }
    }
  }
};
