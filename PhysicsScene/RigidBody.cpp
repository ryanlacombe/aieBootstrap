#include "RigidBody.h"

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) : PhysicsObject(shapeID)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	// Apply gravity
		applyForce(gravity * m_mass * timeStep, m_position);
	//Clamp drag
	if (glm::length(m_velocity) < 0.01f)
		m_velocity = glm::vec2(0.0f, 0.0f);
	if (glm::abs(m_angularVelocity) < 0.01f)
		m_angularVelocity = 0;
	//Calculate linear movement
	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_position += m_velocity * timeStep;
	//Calculate angular movement
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
}

void Rigidbody::debug()
{

}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 position)
{
	//Calculate the change in velocity
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * position.x - force.x * position.y) / m_moment;
}

void Rigidbody::resolveCollision(Rigidbody* other, glm::vec2 contact, glm::vec2* collisionNormal)
{
	//Use or assign the collision normal
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : other->getPosition() - m_position);
	//Find the perpendicular to the collision normal
	glm::vec2 perpendicular(normal.y, -normal.x);

	//Find the radii from axes to application of force
	float radius = glm::dot(contact - m_position, -perpendicular);
	float otherRadius = glm::dot(contact - other->m_position, perpendicular);

	//Find the velocities of the contact point for each object
	float velocity = glm::dot(m_velocity, normal) - radius * m_angularVelocity;
	float otherVelocity = glm::dot(other->m_velocity, normal) + otherRadius * other->m_angularVelocity;

	//If the objects are moving apart ignore the collision
	if (velocity <= otherVelocity) return;

	//Find the effective mass at contact point for each object
	float mass = 1.0f / (1.0f / m_mass + (radius * radius) / m_moment);
	float otherMass = 1.0f / (1.0f / other->m_mass + (otherRadius * otherRadius) / other->m_moment);

	//Find the average elasticity
	float elasticity = (m_elasticity + other->m_elasticity) / 2;

	//Find the impulse
	float j = (1.0f + elasticity) * mass * otherMass /
		(mass + otherMass) * (velocity - otherVelocity);

	//Find the force
	glm::vec2 force = normal * j;

	//Apply equal and opposite forces
	other->applyForce(force, contact - other->m_position);
	applyForce(-force, contact - m_position);
}
