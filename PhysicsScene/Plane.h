#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distanceToOrigin);
	~Plane() {}

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) {}
	virtual void debug() {}
	virtual void makeGizmo();

	void resolveCollision(Rigidbody* other, glm::vec2 contact);

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
};