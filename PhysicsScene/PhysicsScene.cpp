#include "PhysicsScene.h"

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0,0);
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	for (auto i = m_actors.begin(); i < m_actors.end(); i++)
	{
		if (*i == actor)
		{
			m_actors.erase(i);
		}
	}
}

void PhysicsScene::update(float dt)
{
	// update physics at a fixed time step

	//store how much time has accumulated since last update
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	//while enough time accumulated
	while (accumulatedTime >= m_timeStep)
	{
		//for each PhysicsObject in m_actors
		for (auto pActor : m_actors)
		{
			//update the physics of that object
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}

		//spend the time needed for update
		accumulatedTime -= m_timeStep;
	}

}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors) {
		pActor->makeGizmo();
	}
}
