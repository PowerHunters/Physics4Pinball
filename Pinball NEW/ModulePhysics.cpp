#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"


#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	body_clicked = NULL;
	ground = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, bool dynamic)
{
	b2BodyDef body;

	if (dynamic)
		body.type = b2_dynamicBody;
	else
		body.type = b2_staticBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body_clicked = NULL;

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height , bool dynamic)
{
	b2BodyDef body;

	if (dynamic)
		body.type = b2_dynamicBody;
	else
		body.type = b2_staticBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, float degrees)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = DEGTORAD* degrees;

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, bool dynamic)
{
	b2BodyDef body;

	if (dynamic)
		body.type = b2_dynamicBody;
	else
		body.type = b2_staticBody;
	
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size, bool dynamic)
{
	b2BodyDef body;

	if (dynamic)
		body.type = b2_dynamicBody;
	else
		body.type = b2_staticBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.Set(p, size/2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateFlipper(b2Vec2 position, int width, int height, b2Vec2 rotation_point, float32 lower_angle, float32 upper_angle, SDL_Texture *tex)
{
	PhysBody* flipper = CreateRectangle(position.x, position.y, width, height);
	flipper->texture = tex;
	PhysBody* rotor = App->physics->CreateCircle(rotation_point.x, rotation_point.y, 5 , false);

	b2RevoluteJointDef joint_def;
	b2RevoluteJoint* joint;

	joint_def.Initialize(flipper->body, rotor->body, rotor->body->GetWorldCenter());

	joint_def.lowerAngle = lower_angle * DEGTORAD;
	joint_def.upperAngle = upper_angle * DEGTORAD;
	joint_def.enableMotor = true;
	joint_def.maxMotorTorque = 0.0f;
	joint_def.motorSpeed = 4.0f;
	joint_def.enableLimit = true;
	joint_def.collideConnected = false;

	joint = (b2RevoluteJoint*)world->CreateJoint(&joint_def);

	return flipper;
}



// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	b2Vec2 mouse(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
	body_clicked = nullptr;

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 0, 0);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 0, 0);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			if( App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			 {
				if (f->TestPoint(mouse))
					body_clicked = f->GetBody();
			 }
		}
	}

	if (mouse_joint == nullptr && body_clicked != nullptr)
	{
		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = body_clicked;
		def.target = mouse;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * body_clicked->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint)
	{
		mouse_joint->SetTarget(mouse);
		b2Vec2 posA(mouse_joint->GetAnchorA()), posB(mouse_joint->GetAnchorB());
		App->renderer->DrawLine(METERS_TO_PIXELS(posA.x), METERS_TO_PIXELS(posA.y), METERS_TO_PIXELS(posB.x), METERS_TO_PIXELS(posB.y), 255, 0, 0);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}

	return UPDATE_CONTINUE;
}

//PhysBody *ModulePhysics::CreateStarter(int x, int y, int width, int height, SDL_Texture* tex)
//{
//
//	PhysBody* start_box = App->physics->CreateRectangle(x, y, width, height, true);
//	start_box->texture = tex;
//
//	//b2BodyDef bd;
//	//bd.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//	//bd.type = b2_dynamicBody;
//
//	//b2Body *b = world->CreateBody(&bd);
//
//	//b2PolygonShape shape;
//
//	//int width = 16, height = 31;
//	//shape.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
//
//	//b2FixtureDef fixture;
//	//fixture.shape = &shape;
//	//fixture.density = 1.0f;
//
//	//b->CreateFixture(&fixture);
//
//	//App->scene->propulsor = new PhysBody();
//	//App->scene->propulsor->body = b;
//	//App->scene->propulsor->body->SetUserData(app->scene->propulsor);
//	//App->scene->propulsor->texture = texture;
//	//App->scene->propulsor->width = width * 0.5f;
//	//App->scene->propulsor->height = height * 0.5f;
//	
//	b2PrismaticJointDef joint_def;
//	b2PrismaticJoint* joint;
//
//	b2Vec2 worldAxis(1.0f, 0.0f);
//
//	joint_def.bodyA = ground;
//	joint_def.bodyB = start_box->body;
//	joint_def.localAnchorA.Set(0, 0);
//	joint_def.localAxisA.Set(0, -1);
//
//	joint_def.enableLimit = true;
//	joint_def.lowerTranslation = -0.5f;
//	joint_def.upperTranslation = 0.0f;
//		
//	joint_def.enableMotor = true;
//	joint_def.maxMotorForce = 20.0f;
//	joint_def.motorSpeed = 1.0f;
//		
//
//	joint = (b2PrismaticJoint*)world->CreateJoint(&joint_def);
//
//	return start_box;
//}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

//void PhysBody::Push(float x, float y)
//{
//	body->ApplyForceToCenter(b2Vec2(x, y), true);
//}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

//FLIPPER *ModulePhysics::createLeftFlipper(b2Vec2 rotation_point, float32 lower_angle, float32 upper_angle, SDL_Texture *tex)
//{
//	// Rotor of the flipper
//	float32 radius = 5;
//	b2BodyDef rotor_def;
//	rotor_def.position.Set(PIXEL_TO_METERS(rotation_point.x), PIXEL_TO_METERS(rotation_point.y));
//	rotor_def.type = b2_staticBody;
//	b2Body *rotor = world->CreateBody(&rotor_def);
//	b2CircleShape shape;
//	shape.m_radius = PIXEL_TO_METERS(radius);
//	b2FixtureDef rotor_fixture;
//	rotor_fixture.shape = &shape;
//	rotor->CreateFixture(&rotor_fixture);
//
//	// Flipper
//	b2BodyDef stick_def;
//	stick_def.position.Set(PIXEL_TO_METERS(rotation_point.x), PIXEL_TO_METERS(rotation_point.y));
//	stick_def.type = b2_dynamicBody;
//	b2Body *stick = world->CreateBody(&stick_def);
//
//	b2FixtureDef left_flip_fixture;
//	int left_flipper[14] = {
//		1, 1,
//		46, 3,
//		49, 6,
//		46, 8,
//		3, 9,
//		0, 5,
//		1, 1
//	};
//
//	b2PolygonShape poly_shape;
//	left_flip_fixture.shape = polyFromPoints(&poly_shape, left_flipper, sizeof(left_flipper) / sizeof(int));
//	left_flip_fixture.density = 1.0f;
//	stick->CreateFixture(&left_flip_fixture);
//
//	// PhysBody declaration
//	FLIPPER* flip = new FLIPPER();
//	flip->body = stick;
//	stick->SetUserData(flip);
//	flip->texture = tex;
//	flip->side = LEFT_FLIPPER;
//	flip->width = 0;
//	flip->height = 0;
//
//	left_flipper_joint = createFlipperJoint(rotor, stick, lower_angle, upper_angle, LEFT_FLIPPER);
//
//	return flip;
//}

//FLIPPER *ModulePhysics::createRightFlipper(b2Vec2 rotation_point, float32 lower_angle, float32 upper_angle, SDL_Texture *tex)
//{
//	// Rotor of the flipper
//	float32 radius = 5;
//	b2BodyDef rotor_def;
//	rotor_def.position.Set(PIXEL_TO_METERS(rotation_point.x), PIXEL_TO_METERS(rotation_point.y));
//	rotor_def.type = b2_staticBody;
//	b2Body *rotor = world->CreateBody(&rotor_def);
//	b2CircleShape shape;
//	shape.m_radius = PIXEL_TO_METERS(radius);
//	b2FixtureDef rotor_fixture;
//	rotor_fixture.shape = &shape;
//	rotor->CreateFixture(&rotor_fixture);
//
//	// Flipper
//	b2BodyDef stick_def;
//	stick_def.position.Set(PIXEL_TO_METERS(rotation_point.x), PIXEL_TO_METERS(rotation_point.y));
//	stick_def.type = b2_dynamicBody;
//	b2Body *stick = world->CreateBody(&stick_def);
//
//	b2FixtureDef right_flip_fixture;
//	int right_flipper[14] = {
//		47, 1,
//		4, 3,
//		2, 5,
//		4, 7,
//		47, 9,
//		49, 5,
//		47, 1
//	};
//
//	b2PolygonShape poly_shape;
//	right_flip_fixture.shape = polyFromPoints(&poly_shape, right_flipper, sizeof(right_flipper) / sizeof(int));
//	right_flip_fixture.density = 1.0f;
//	stick->CreateFixture(&right_flip_fixture);
//
//	// PhysBody declaration
//	FLIPPER* flip = new FLIPPER();
//	flip->body = stick;
//	stick->SetUserData(flip);
//	flip->texture = tex;
//	flip->side = RIGHT_FLIPPER;
//	flip->width = 0;
//	flip->height = 0;
//
//	right_flipper_joint = createFlipperJoint(rotor, stick, lower_angle, upper_angle, RIGHT_FLIPPER);
//
//	return flip;
//}

//b2RevoluteJoint *ModulePhysics::createFlipperJoint(b2Body *rotor, b2Body *stick, float32 &lower_angle, float32 &upper_angle, FLIPPER_SIDE side)
//{
//	b2RevoluteJointDef revoluteJointDef;
//	revoluteJointDef.bodyA = rotor;
//	revoluteJointDef.bodyB = stick;
//	revoluteJointDef.collideConnected = false;
//
//	revoluteJointDef.enableLimit = true;
//	revoluteJointDef.lowerAngle = lower_angle * DEGTORAD;
//	revoluteJointDef.upperAngle = upper_angle * DEGTORAD;
//
//	revoluteJointDef.enableMotor = false;
//	revoluteJointDef.maxMotorTorque = 720.0f * DEGTORAD;
//	revoluteJointDef.motorSpeed = 0.0f;
//
//	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
//
//	if (side == LEFT_FLIPPER)
//		revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(5), PIXEL_TO_METERS(4));
//	else
//		revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(45), PIXEL_TO_METERS(4));
//
//	return (b2RevoluteJoint*)world->CreateJoint(&revoluteJointDef);
//}

//void ModulePhysics::engageRightFlipper()
//{
//	b2RevoluteJoint left_joints_flipper;
//
//	EnableMotor(true);
//	SetMotorSpeed(1080 * DEGTORAD);
//
//}
//
//void ModulePhysics::engageLeftFlipper()
//{
//	b2RevoluteJoint* left_joints_flipper;
//	
//	EnableMotor(true);
//	SetMotorSpeed(-1080 * DEGTORAD);
//
//}
//void ModulePhysics::disengageLeftFlipper()
//{
//	b2RevoluteJoint* *item = app->physics->left_joints_flippers.getFirst();
//		item->data->EnableMotor(false);
//		item->data->SetMotorSpeed(0.0f);
//}
//
//void ModulePhysics::disengageRightFlipper()
//{
//	b2RevoluteJoint*> *item = app->physics->right_joints_flippers.getFirst();
//		item->data->EnableMotor(false);
//		item->data->SetMotorSpeed(0.0f);
//}