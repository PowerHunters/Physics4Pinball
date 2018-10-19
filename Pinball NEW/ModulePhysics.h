#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum FLIPPER_SIDE
{
	LEFT_FLIPPER,
	RIGHT_FLIPPER
};

//class FLIPPER : public PhysBody
//{
//public:
//	FLIPPER(){}
//	FLIPPER_SIDE	 side;
//};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	SDL_Texture* texture;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, bool dynamic = true);
	PhysBody* CreateRectangle(int x, int y, int width, int height , bool dynamic = true);
	PhysBody* CreateChain(int x, int y, int* points, int size, bool dynamic = true);

	b2RevoluteJoint *createFlipperJoint(b2Body *rotor, b2Body *stick, float32 &lower_angle, float32 &upper_angle, FLIPPER_SIDE side);
	//FLIPPER *createLeftFlipper(b2Vec2 rotation_point, float32 lower_angle, float32 upper_angle, SDL_Texture *tex);
	//FLIPPER *createRightFlipper(b2Vec2 rotation_point, float32 lower_angle, float32 upper_angle, SDL_Texture *tex);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, float degrees = 0);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	//flipers void methods
	//void engageLeftFlipper();
	//void engageRightFlipper();
	//void disengageLeftFlipper();
	//void disengageRightFlipper();

	b2RevoluteJoint* right_flipper_joint;
	b2RevoluteJoint* left_flipper_joint;

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	b2Body* body_clicked;
};