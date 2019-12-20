#ifndef __PHYSICS_H
#include <cmath>
#include <list>
#pragma warning(disable:4244)

class Vector2D {
	friend void wrap (Vector2D&, double, double);
public:
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
	double getx () const { return x; }
	double gety () const { return y; }

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

private:
	double x;
	double y;
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
};
struct Derivative {
	Vector2D dpos;
	Vector2D dvel;
};
class Mass
{
	public:
		Mass (double mass = 1, double friction = 0): m(mass), f(friction) {}

		Vector2D getPos () const { return state.pos; }
		double getx () const { return state.pos.getx(); }
		double gety () const { return state.pos.gety(); }

		void apply (const Vector2D& force)
		{
			this->state.force += force;
		}
		void apply (Mass& target, const Vector2D& force)
		{
			target.apply(force);
			this->apply(-force);
		}

		void update (long double dt)
		{
			reset();
			solve();
			prevState = state;
			simulate(dt);
		}
		void reset ()
		{
			state.force.clear();
		}
		virtual void solve () {}
		void draw (double alpha)
		{
			State temp(state);
			state = interpolate(prevState, state, alpha);
			render();
			state = temp;
		}
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

	protected:
		State state, prevState;

		virtual void render () {}
		virtual Vector2D acceleration (const State& state, double m)
		{
			return (state.force / m);
		}

		double m;
		double f;

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

		State interpolate (const State& previous, const State& current, double alpha)
		{
			State ret;
			ret.pos = current.pos * alpha + previous.pos * (1 - alpha);
			ret.vel = current.vel * alpha + previous.vel * (1 - alpha);
			return ret;
		}
};