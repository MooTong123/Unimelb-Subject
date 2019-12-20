#include <ctime>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "physics.h"
#include "object.h"
#include "maths.h"
#include "glwin.h"
#include "stopwatch.h"

const int screenWidth = 1024;
const int screenHeight = 768;
const double dt = 0.01;
const double slowRatio = 100;

enum Objects {
	obj_player,
	obj_block,
	obj_trace
};
void eglRectangle (double x, double y, double width, double height);
Vector2D getMousePos ();
class Block: public Object
{
	public:
		Block () { size.w = 32; size.h = 32; }
		void render (long double dt)
		{
			glColor3ub(0, 0, 0);
			eglRectangle(size.x, size.y, size.w, size.h);
		}

		int getType () const { return obj_block; }
};
class Trace: public Object
{
	public:
		Trace (): velocity(4, 4) {}
		void setSpeed (Vector2D speed)
		{
			velocity = speed;
		}
		void update (long double dt)
		{
			size.w = velocity.x / 5;
			size.h = velocity.y / 5;
			size.x += velocity.x * dt;
			size.y += velocity.y * dt;

			if (getCollision(obj_block) || size.x < 0 || size.y < 0 || size.x > screenWidth || size.y > screenHeight) {
				destroy();
			}
		}
		void render (long double dt)
		{
			glBegin(GL_LINES);
			glColor3ub(0, 0, 0);
			glVertex2f(size.x, size.y);
			glVertex2f(size.x + velocity.x / 5, size.y + velocity.y / 5);
			glEnd();
		}
		int getType () const { return obj_trace; }
	private:
		Vector2D velocity;
};
class Weapon
{
	public:
		virtual void shoot () = 0;
		virtual void reload () = 0;
		virtual int getAmmoLoaded () = 0;
		virtual int getAmmoTotal () = 0;
		virtual void setpos (double x, double y) { this->x = x; this->y = y; }

	protected:
		double x;
		double y;
};
class Player: public Object
{
	class Scattergun: public Weapon
	{
		public:
			Scattergun (): canShoot(true), canReload(true), shootInterval(50000), reloadInterval(60000),
						   ammoLoaded(6), ammoTotal(32), maxAmmo(32), maxLoaded(6) {}
			void shoot ()
			{
				if (!canShoot && tShoot.peek() > shootInterval * dt) {
					canShoot = true;
					tShoot.stop();
					tShoot.clear();
				}
				if (ammoLoaded && canShoot) {
					traceMouse(3, 500);
					// todo: raytrace collision

					canShoot = false;
					tShoot.start();
					canReload = false;
					tReload.start();
					ammoLoaded--;
				}
			}
			void reload ()
			{
				if (!canReload && tReload.peek() > reloadInterval * dt) {
					canReload = true;
					tReload.stop();
					tReload.clear();
				}
				if (canReload && ammoTotal && ammoLoaded < maxLoaded) {
					ammoLoaded++;
					ammoTotal--;
					canReload = false;
					tReload.start();
				}
			}
			int getAmmoLoaded ()
			{
				return ammoLoaded;
			}
			int getAmmoTotal ()
			{
				return ammoTotal;
			}

		private:
			void traceMouse (double offset, double spd)
			{
				// get vectors
				Vector2D ldir1(lengthdir(distanceToPoint(Vector2D(x, y), getMousePos()), dirToPoint(Vector2D(x, y), getMousePos()) - offset));
				Vector2D ldir2(lengthdir(distanceToPoint(Vector2D(x, y), getMousePos()), dirToPoint(Vector2D(x, y), getMousePos())));
				Vector2D ldir3(lengthdir(distanceToPoint(Vector2D(x, y), getMousePos()), dirToPoint(Vector2D(x, y), getMousePos()) + offset));

				// make bullet traces
				addObject<Trace>(x, y).setSpeed(ldir1.unitise() * spd);
				addObject<Trace>(x, y).setSpeed(ldir2.unitise() * spd);
				addObject<Trace>(x, y).setSpeed(ldir3.unitise() * spd);
			}
			bool canShoot;
			bool canReload;


			Stopwatch tShoot;
			Stopwatch tReload;
			const int shootInterval;
			const int reloadInterval;

			int ammoLoaded;
			int ammoTotal;

			const int maxLoaded;
			const int maxAmmo;
	};
	public:
		Player (): body(1, 2, 0, speed / 2, speed / 2), speed(10), jSpeed(40), gravity(-4), falling(false), doubleJumped(false), reloading(false)
		{
			static Scattergun g;
			pGun = &g;
			size.w = 32;
			size.h = 32;
			body.set(32, 32);
			bigFont = win->makeFont(64, 0, FW_BOLD, "TF2 Secondary");
			smallFont = win->makeFont(48, 0, FW_BOLD, "TF2 Secondary");
		}

		void update (long double dt)
		{
			// don't float in midair
			if (placeFree(size.x, size.y - size.h / 2)) {
				falling = true;
			}
			
			// shoot
			if (win->checkMouse(MB_LEFT)) {
				pGun->shoot();
				reloading = false;
			}
			if (pGun->getAmmoLoaded() == 0) {
				reloading = true;
			}

			// toggle reload
			if (win->getKeyPressed('R')) {
				reloading = true;
			}
			// do reload
			if (reloading) {
				pGun->reload();
			}

			// horizontal movement
			if (win->getKey('A')) {
				body.apply(-speed, 0);
			}
			if (win->getKey('D')) {
				body.apply(speed, 0);
			}

			// jump
			if (win->getKeyPressed('W') && !doubleJumped) {
				if (!falling) {
					body.add(0, jSpeed);
					falling = true;
				}
				else if (!doubleJumped) {
					body.add(0, jSpeed);
					doubleJumped = true;
				}
			}
			else if (falling) {
				body.apply(0, -jSpeed * 0.05);
			}

			// stop falling if hit floor
			if (getCollision(obj_block)) {
				if (falling) {
					body.stopy();
					if (body.gety() < other->gety()) {
						body.set(body.getx(), other->gety() - other->geth());
					}
					else {
						body.set(body.getx(), other->gety() + other->geth());
					}
					falling = false;
					doubleJumped = false;
				}
			}

			// do gravity
			if (falling) {
				body.apply(0, gravity);
			}

			// update physics and game position

			body.update(dt);
			// if you hit a wall, slow down
			if (placeMeeting(body.getx(), body.gety(), obj_block)) {
				body.slow(speed * 8, 0);
				body.set(size.x, body.gety());
			}
			size.x = body.getx();
			size.y = body.gety();
			pGun->setpos(size.x, size.y);
		}
		void render (long double dt)
		{
			glColor3ub(255, 0, 0);
			eglRectangle(body.getx(dt), body.gety(dt), 32, 32);
			glColor3ub(0, 0, 0);
			win->wDrawFont(bigFont, itoa(pGun->getAmmoLoaded()), screenWidth - 128, 64);
			win->wDrawFont(smallFont, itoa(pGun->getAmmoTotal()), screenWidth - 96, 48);
		}

		int getType () const { return obj_player; }

	private:
		void traceMouse (double offset, double spd)
		{
			// get vectors
			Vector2D ldir1(lengthdir(distanceToPoint(body.getPos(), getMousePos()), dirToPoint(body.getPos(), getMousePos()) - offset));
			Vector2D ldir2(lengthdir(distanceToPoint(body.getPos(), getMousePos()), dirToPoint(body.getPos(), getMousePos())));
			Vector2D ldir3(lengthdir(distanceToPoint(body.getPos(), getMousePos()), dirToPoint(body.getPos(), getMousePos()) + offset));

			// make bullet traces
			addObject<Trace>(size.x, size.y).setSpeed(ldir1.unitise() * spd);
			addObject<Trace>(size.x, size.y).setSpeed(ldir2.unitise() * spd);
			addObject<Trace>(size.x, size.y).setSpeed(ldir3.unitise() * spd);
		}

		Font bigFont;
		Font smallFont;

		Mass body;
		bool falling;
		bool doubleJumped;

		bool reloading;

		const double speed;
		const double jSpeed;
		const double gravity;

		Weapon* pGun;
};

int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	bool done = false;

	GLWindow& myWin = *GLWindow::getInstance(screenWidth, screenHeight, false);


	addObject<Player>();

	// border around room
	for (int i = 0; i < 32; ++i) {
		addObject<Block>(i * 32, 0);
		addObject<Block>(i * 32, 698);
	}
	for (int i = 1; i < 32; ++i) {
		addObject<Block>(0, i * 32);
		addObject<Block>(978, i * 32);
	}

	addObject<Block>(128, 128);
	addObject<Block>(160, 128);
	addObject<Block>(192, 128);

	addObject<Block>(256, 32);
	
	double oldtime = clock();
	double newtime = oldtime;
	double accumulator = 0;

	while (!done) {
		// Window message handling
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = true;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (myWin.getKey(VK_ESCAPE)) {
			done = true;
		}
		else {
			// do physics
			
			newtime = clock();
			double frametime = newtime - oldtime;
			frametime /= slowRatio;
			oldtime = newtime;

			// Update physics the correct number of times
			// to create a smooth simulation
			for (accumulator += frametime; accumulator >= dt; accumulator -= dt) {
				Object::updateAll(dt);
			}
	
			// Clear screen, draw object and swap buffers
			wClear();
			Object::renderAll(accumulator / dt);
			myWin.wSwapBuffers();
		}
		// Rendering loop
	}
	Object::destroyAll();
	return 0;
}
void eglRectangle (double x, double y, double width, double height)
{
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}
Vector2D getMousePos ()
{
	POINT pos;
	GetCursorPos(&pos);
	return Vector2D(pos.x, 786 - pos.y);
}