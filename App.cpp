/**
Comp 394 S15 Assignment #2 Ping Pong 3D
**/

#include "App.h"
#include <iostream>
#include <cmath>
using namespace std;

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();


int main(int argc, const char* argv[]) {
	(void)argc; (void)argv;
	GApp::Settings settings(argc, argv);

	// Change the window and other startup parameters by modifying the
	// settings class.  For example:
	settings.window.width       = 1280; 
	settings.window.height      = 720;

	return App(settings).run();
}




App::App(const GApp::Settings& settings) : GApp(settings) {
	renderDevice->setColorClearValue(Color3(0.2, 0.2, 0.2));
	renderDevice->setSwapBuffersAutomatically(true);
}


void App::onInit() {
	GApp::onInit();
	// Turn on the developer HUD
	createDeveloperHUD();
	debugWindow->setVisible(false);
	developerWindow->setVisible(false);
	developerWindow->cameraControlWindow->setVisible(false);
	showRenderingStats = false;

	// Setup the camera with a good initial position and view direction to see the table
	activeCamera()->setPosition(Vector3(0,100,250));
	activeCamera()->lookAt(Vector3(0,0,0), Vector3(0,1,0));
	activeCamera()->setFarPlaneZ(-1000);
    
    ballPosition = Vector3(0,30,0);
    ballSpeed = Vector3(0,0,0);
    initPos = Vector3(0, 40.0, -100.0);
    initVel = Vector3(0, 0, 60.0);
    time = 0;
}


void App::onUserInput(UserInput *uinput) {

	// This block of code maps the 2D position of the mouse in screen space to a 3D position
	// 20 cm above the ping pong table.  It also rotates the paddle to make the handle move
	// in a cool way.  It also makes sure that the paddle does not cross the net and go onto
	// the opponent's side.
	Vector2 mouseXY = uinput->mouseXY();
	float xneg1to1 = mouseXY[0] / renderDevice->width() * 2.0 - 1.0;
	float y0to1 = mouseXY[1] / renderDevice->height();
	Matrix3 rotZ = Matrix3::fromAxisAngle(Vector3(0,0,1), aSin(-xneg1to1));    
	Vector3 lastPaddlePos = paddleFrame.translation;
	paddleFrame = CoordinateFrame(rotZ, Vector3(xneg1to1 * 100.0, 20.0, G3D::max(y0to1 * 137.0 + 20.0, 0.0)));
	Vector3 newPos = paddleFrame.translation;

	// This is a weighted average.  Update the velocity to be 10% the velocity calculated 
	// at the previous frame and 90% the velocity calculated at this frame.
	paddleVel = 0.1*paddleVel + 0.9*(newPos - lastPaddlePos);



	// This returns true if the SPACEBAR was pressed
	if (uinput->keyPressed(GKey(' '))) {
		// This is where you can "serve" a new ball from the opponent's side of the net 
		// toward you. I found that a good initial position for the ball is: (0, 30, -130).  
		// And, a good initial velocity is (0, 200, 400).  As usual for this program, all 
		// units are in cm.

	}
}



void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {

	// rdt is the change in time (dt) in seconds since the last call to onSimulation
	// So, you can slow down the simulation by half if you divide it by 2.
	rdt *= 2.0;
    time += rdt;
    //dt = sdt;

	// Here are a few other values that you may find useful..
	// Radius of the ball = 2cm
	// Radius of the paddle = 8cm
	// Acceleration due to gravity = 981cm/s^2 in the negative Y direction
	// See the diagram in the assignment handout for the dimensions of the ping pong table


    if (abs(ballPosition.z) <= 2.5 && ballPosition.y <= 15.25){ // check if the ball is colliding with the net
        // STILL SUPER BUGGY. DOESN'T DO WHAT WE NEED IT TO!
        initVel.z *= -1;
        initPos.z = ballPosition.z;
    }
    if(ballPosition.y <= 2.0) { // So far yDown is the only way I've been able to make the ball bounce on the table.
        initVel.y = -1 * (initVel.y - 9.8 * time);
        initPos.y = 2 * ballPosition.y;
        ballPosition.y *= 2;
        ballPosition.z = initPos.z+initVel.z*time;
        initPos.z = ballPosition.z;
        time = 0.0;
        }
    else {
        double y = initPos.y + initVel.y * time - 4.9*time*time;
        ballPosition = Vector3(0,y,initPos.z+initVel.z*time);
    }
//    return ballPosition;
}


void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) {
	rd->clear();


    Box tabletop(Vector3(-76.25, -5, -137), Vector3(76.25, 0, 137));
	Draw::box(tabletop, rd, Color3(0, 0.4, 0), Color3(1, 1, 1));
    
	Box tableMiddleLine(Vector3(-1, 0, -137), Vector3(1, 0, 137));
	Draw::box(tableMiddleLine, rd, Color3::white(), Color4::clear());
    
	Box legA(Vector3(-56.25, -71, 117), Vector3(-46.25, -5, 107));
	Draw::box(legA, rd, Color3(0.81, 0.77, 0.75), Color3(0.71, 0.67, 0.65));
    
	Box legB(Vector3(56.25, -71, 117), Vector3(46.25, -5, 107));
	Draw::box(legB, rd, Color3(0.81, 0.77, 0.75), Color3(0.71, 0.67, 0.65));
    
	Box netBounds(Vector3(-87.5, 0, -0.5), Vector3(87.5, 15.25, 0.5));
	Draw::box(netBounds, rd, Color4::clear(), Color3(0.55, 0.55, 0.55));
    
	LineSegment net;
	for (int y = 0; y < 15; y += 2)
	{
		net = LineSegment::fromTwoPoints(Point3(-87.5, y, 0), Point3(87.5, y, 0));
		Draw::lineSegment(net, rd, Color3::black());
	}
	for (int x = -87; x < 87; x += 2)
	{
		net = LineSegment::fromTwoPoints(Point3(x, 0, 0), Point3(x, 15.25, 0));
		Draw::lineSegment(net, rd, Color3::black());
    }
    
    // Sphere ball(Vector3(0,30,-137), 2.0);
    Sphere ball(ballPosition, 2.0);
    Draw::sphere(ball, rd, Color3::red());




	// Draw the paddle using 2 cylinders
	rd->pushState();
	rd->setObjectToWorldMatrix(paddleFrame);
	Cylinder paddle(Vector3(0,0,-0.5), Vector3(0,0,0.5), 8.0);
	Cylinder handle(Vector3(0,-7.5,0), Vector3(0,-16,0), 1.5);
	Draw::cylinder(paddle, rd, Color3(0.5,0,0), Color4::clear());
	Draw::cylinder(handle, rd, Color3(0.3,0.4,0), Color4::clear());
	rd->popState();  

	// Call to make the GApp show the output of debugDraw
	drawDebugShapes();
}


