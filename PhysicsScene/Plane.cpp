#include "Plane.h"
#include <Gizmos.h>

Plane::Plane()
	: PhysicsObject(PLANE)
{
	m_normal = glm::vec2(0.0f, 1.0f);
	m_distanceToOrigin = 0.0f;
}

Plane::Plane(glm::vec2 normal, float distanceToOrigin)
	: PhysicsObject(PLANE)
{
	m_normal = glm::normalize(normal);
	m_distanceToOrigin = distanceToOrigin;
}

void Plane::makeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	glm::vec2 perpendicular(m_normal.y, -m_normal.x);
	glm::vec4 color(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (perpendicular * lineSegmentLength);
	glm::vec2 end = centerPoint - (perpendicular * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, color);
}

void Plane::resolveCollision(Rigidbody* other, glm::vec2 contact)
{
	glm::vec2 collisionNormal = m_normal;
	glm::vec2 relativeVelocity = other->getVelocity();

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal) /
		(1 / other->getMass());

	glm::vec2 force = collisionNormal * j;

	other->applyForce(force, contact - other->getPosition());
}