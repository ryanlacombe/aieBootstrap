#include "PhysicsSceneApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "PhysicsScene.h"
#include "Plane.h"
#include <Gizmos.h>
#include <vector>

PhysicsSceneApp::PhysicsSceneApp() {

}

PhysicsSceneApp::~PhysicsSceneApp() 
{

}

bool PhysicsSceneApp::startup() {
	
	// increase the 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	// initialize gravity
	glm::vec2 gravity = glm::vec2(0.0f, -80.0f);

	// initialize the PhysicsScene
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(gravity);
	m_physicsScene->setTimeStep(0.01f);

	// initialize position and velocity
	glm::vec2 initialPosition = glm::vec2(-60.0f, 0.0f);
	glm::vec2 finalPosition = glm::vec2(60.0f, 0.0f);
	glm::vec2 initialVelocity = calculateVelocity(initialPosition, finalPosition, gravity.y, 5.0f);

	// simulate using kinematic formulae
	setupContinuousDemo(initialPosition, initialVelocity, gravity.y);

	// create a ball to print on the screen
	Sphere* ball = new Sphere(initialPosition, initialVelocity,
		1.0f, 4.0f, glm::vec4(1.0f, 0.0f, 0.5f, 1.0f));
	m_physicsScene->addActor(ball);

	Sphere* ball1 = new Sphere(glm::vec2(-40.0f, 0.0f), glm::vec2(100.0f, 0.0f),
		6.0f, 8.0f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(ball1);

	Sphere* ball2 = new Sphere(glm::vec2(40.0f, 0.0f), glm::vec2(30.0f, 0.0f),
		4.0f, 6.0f, glm::vec4(0, 1, 0, 1));
	m_physicsScene->addActor(ball2);

	Sphere* ball3 = new Sphere(glm::vec2(60.0f, 0.0f), glm::vec2(60.0f, 0.0f),
		1.0f, 4.0f, glm::vec4(0, 0, 1, 1));
	m_physicsScene->addActor(ball3);

	Plane* floor = new Plane(glm::vec2(1.0f, -2.0f), 40.0f);
	m_physicsScene->addActor(floor);

	Plane* floor2 = new Plane(glm::vec2(-1.0f, -2.0f), 40.0f);
	m_physicsScene->addActor(floor2);

	Plane* floor3 = new Plane(glm::vec2(1.0f, 2.0f), 40.0f);
	m_physicsScene->addActor(floor3);

	Plane* floor4 = new Plane(glm::vec2(-1.0f, 2.0f), 40.0f);
	m_physicsScene->addActor(floor4);

	return true;
}

void PhysicsSceneApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsSceneApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	// clear the buffer
	aie::Gizmos::clear();

	aie::Gizmos::add2DAABB(glm::vec2(-60.0f, 0.0f), glm::vec2(4.0f, 4.0f), glm::vec4(1, 1, 1, 1));
	aie::Gizmos::add2DAABB(glm::vec2(60.0f, 0.0f), glm::vec2(4.0f, 4.0f), glm::vec4(1, 1, 1, 1));

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsSceneApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsSceneApp::setupContinuousDemo(glm::vec2 initialPosition, glm::vec2 initialVelocity, float gravity)
{
	float time = 0.0f;
	float timeStep = 0.5f;
	float radius = 3.0f;
	int segments = 12;
	glm::vec4 color = glm::vec4(1, 1, 0, 1);
	glm::vec2 finalPosition = initialPosition;

	while (time <= 5) {
		// calculate the position of the projectile at the time
		finalPosition.x = initialPosition.x + initialVelocity.x * time;
		finalPosition.y = (initialPosition.y + initialVelocity.y * time) + (0.5f * gravity * (time * time));

		aie::Gizmos::add2DCircle(finalPosition, radius, segments, color);
		time += timeStep;
	}
}

glm::vec2 PhysicsSceneApp::calculateVelocity(glm::vec2 initialPosition, glm::vec2 finalPosition, float gravity, float time)
{
	glm::vec2 initialVelocity = glm::vec2(0, 0);

	initialVelocity.x = (finalPosition.x - initialPosition.x) / time;
	initialVelocity.y = (finalPosition.y - initialPosition.y - (0.5f * gravity * (time * time)) / time);

	return initialVelocity;
}
