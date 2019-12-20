#ifndef __PHYSICS_H
#define __PHYSICS_H

#include <cmath>
#include <list>
#pragma warning(disable:4244)

template <class T>
inline
T sign (T x)
{
	if (x > 0) {
		return 1;
	}
	if (x < 0) {
		return -1;
	}
	return 0;
}

struct Vector2D {
	friend void wrap (Vector2D&, double, double);
	double x;
	double y;
	Vector2D (double x = 0, double y = 0)
	{
		this->x = x;
		this->y = y;
	}
	Vector2D (const Vector2D& v): x(v.x), y(v.y) {}
	Vector2D& operator = (const Vector2D& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	Vector2D& operator = (double s)
	{
		x = s;
		y = s;
	}
	Vector2D operator + (const Vector2D& v) const
	{
		return Vector2D(x + v.x, y + v.y);
	}
	Vector2D operator - (const Vector2D& v) const
	{
		return Vector2D(x - v.x, y - v.y);
	}
	Vector2D operator * (const Vector2D& v) const
	{
		return Vector2D(x * v.x, y * v.y);
	}
	Vector2D operator / (const Vector2D& v) const
	{
		return Vector2D(x / v.x, y / v.y);
	}
	Vector2D operator + (double s) const
	{
		return Vector2D(x + s, y + s);
	}
	Vector2D operator - (double s) const
	{
		return Vector2D(x - s, y - s);
	}
	Vector2D operator * (double s) const
	{
		return Vector2D(x * s, y * s);
	}
	Vector2D operator / (double s) const
	{
		return Vector2D(x / s, y / s);
	}
	Vector2D& operator += (const Vector2D& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2D& operator -= (const Vector2D& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vector2D& operator *= (const Vector2D& v)	
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	Vector2D& operator /= (const Vector2D& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	Vector2D& operator += (double s)			// operator+= is used to add another Vector2D to this Vector2D.
	{
		x += s;
		y += s;
		return *this;
	}
	Vector2D& operator -= (double s)			// operator-= is used to subtract another Vector2D from this Vector2D.
	{
		x -= s;
		y -= s;
		return *this;
	}
	Vector2D& operator *= (double s)			// operator*= is used to scale this Vector2D by a s.
	{
		x *= s;
		y *= s;
		return *this;
	}
	Vector2D& operator /= (double s)			// operator/= is used to scale this Vector2D by a s.
	{
		x /= s;
		y /= s;
		return *this;
	}

	Vector2D operator - () const
	{
		return Vector2D(-x, -y);
	}

	bool operator == (Vector2D v) const
	{
		return x == v.x && y == v.y;
	}
	bool operator == (double s) const
	{
		return x == s && y == s;
	}
	bool operator != (Vector2D v) const
	{
		return !(*this == v);
	}
	bool operator != (double s) const
	{
		return !(*this == s);
	}
	bool operator > (Vector2D v) const
	{
		return x > v.x && y > v.y;
	}
	bool operator < (Vector2D v) const
	{
		const Vector2D& u = *this;
		return (!(u > v) && !(u == v));
	}

	double length () const
	{
		return sqrt(x*x + y*y);
	}

	Vector2D abs () const
	{
		return Vector2D(::abs(x), ::abs(y));
	}

	void clear ()
	{
		x = 0;
		y = 0;
	}

	void normalise ()
	{
		double length = this->length();

		if (length == 0) {
			length = 1;
		}
		x /= length;
		y /= length;
	}
	Vector2D unitise () const
	{
		double length = this->length();
		if (length == 0) {
			return *this;
		}
		return Vector2D(x / length, y / length);
	}
};
Vector2D operator + (double s, Vector2D v)
{
	v += s;
	return v;
}
Vector2D operator - (double s, Vector2D v)
{
	v -= s;
	return v;
}
Vector2D operator / (double s, Vector2D v)
{
	v /= s;
	return v;
}
Vector2D operator * (double s, Vector2D v)
{
	v *= s;
	return v;
}

void wrap (Vector2D& v, double width, double height)
{
	if (v.x < 0) {
		v.x = width;
	}
	if (v.y < 0) {
		v.y = height;
	}
	if (v.x > width) {
		v.x = 0;
	}
	if (v.y > height) {
		v.y = 0;
	}
}

struct State {
	Vector2D pos;
	Vector2D vel;
	Vector2D force;
	Vector2D maxVel;
};
struct Derivative {
	Vector2D dpos;
	Vector2D dvel;
};
class Mass
{
	public:
		Mass (double mass = 1, double frictionx = 0, double frictiony = 0, double maxVelX = 100, double maxVelY = 100):
			m(mass), f(frictionx, frictiony)
		{
			state.maxVel = Vector2D(maxVelX, maxVelY);
		}

		Vector2D getPos () const { return state.pos; }

		void rollback () { state = prevState; }

		double getx () const { return state.pos.x; }
		double gety () const { return state.pos.y; }
		double getx (long double dt) const {
			State st(interpolate(dt));
			return st.pos.x;
		}
		double gety (long double dt) const {
			State st(interpolate(dt));
			return st.pos.y;
		}

		Vector2D getVel () const { return state.vel; }

		void add (const Vector2D& force)
		{
			state.vel += force;
		}

		void apply (const Vector2D& force)
		{
			state.force += force;
		}
		void apply (Mass& target, const Vector2D& force)
		{
			target.apply(force);
			apply(-force);
		}

		void slow (const Vector2D& force)
		{
			apply(-force.x * sign(force.x), -force.y * sign(force.y));
		}

		void add (double x, double y)
		{
			Vector2D force(x, y);
			state.vel += force;
		}

		void apply (double x, double y)
		{
			Vector2D force(x, y);
			state.force += force;
		}
		void apply (Mass& target, double x, double y)
		{
			Vector2D force(x, y);
			target.apply(force);
			apply(-force);
		}

		void slow (double x, double y)
		{
			Vector2D force(-x * sign(state.vel.x), -y * sign(state.vel.y));
			apply(force);
		}

		void update (long double dt)
		{
			solve();
			prevState = state;
			simulate(dt);
			reset();
		}

		void reset ()
		{
			state.force.clear();
		}
		void stop ()
		{
			reset();
			state.vel.clear();
		}
		void stopx ()
		{
			state.force.x = 0;
			state.vel.x = 0;
		}
		void stopy ()
		{
			state.force.y = 0;
			state.vel.y = 0;
		}
		void set (const Vector2D& pos)
		{
			state.pos = pos;
		}
		void set (double x, double y)
		{
			state.pos = Vector2D(x, y);
		}

		virtual void solve () {}
		void simulate (long double dt)
		{
			Vector2D friction = -state.vel.unitise() * f;
			apply(friction);

			Derivative a = evaluate(state, 0.0, m, Derivative());
			Derivative b = evaluate(state, dt * 0.5, m, a);
			Derivative c = evaluate(state, dt * 0.5, m, b);
			Derivative d = evaluate(state, dt, m, c);

			const Vector2D dpdt = 1.0 / 6.0 * (a.dpos + 2.0 * (b.dpos + c.dpos) + d.dpos);
			const Vector2D dvdt = 1.0 / 6.0 * (a.dvel + 2.0 * (b.dvel + c.dvel) + d.dvel);

			state.pos += dpdt * dt;
			state.vel += dvdt * dt;
		}
		State interpolate (double alpha) const
		{
			State ret;
			ret.pos = state.pos * alpha + prevState.pos * (1 - alpha);
			ret.vel = state.vel * alpha + prevState.vel * (1 - alpha);
			return ret;
		}

	protected:
		State state, prevState;
		virtual Vector2D acceleration (const State& initial, double m)
		{
			return (initial.force / m);
		}

		double m;
		Vector2D f;

	private:
		Derivative evaluate (const State& initial, double dt, double m, const Derivative& d)
		{
			State state;
			state.pos = initial.pos + d.dpos * dt;
			state.vel = initial.vel + d.dvel * dt;
			state.force = initial.force;

			Derivative output;
			output.dpos = state.vel;
			output.dvel = acceleration(state, m);
			return output;
		}
};

#endif