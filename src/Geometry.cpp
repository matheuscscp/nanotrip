#include "common.hpp"

#include "Geometry.hpp"

#include "InputManager.hpp"
#include "Camera.hpp"

using namespace common;
using namespace lalge;

Ellipse::Ellipse (
	const R2Vector& r,
	const Scalar& depthconst,
	const Scalar& a,
	const Scalar& b,
	const Scalar& angle
) : GameObject ( r, depthconst ), a_ ( a ), b_ ( b ), angle_ ( angle )
{
	if ( a <= 0 )
		throw ( mexception ( "Invalid ellipse \"a\" axis" ) );
	
	if ( b <= 0 )
		throw ( mexception ( "Invalid ellipse \"b\" axis" ) );
	
	if ( ( angle < 0 ) || ( angle >= 180 ) )
		throw ( mexception ( "Invalid ellipse angle" ) );
}

Ellipse::~Ellipse ()
{
}

void Ellipse::update()
{
}

void Ellipse::render()
{
}

GameObject* Ellipse::clone()
{
	return new Ellipse( *this );
}

const Scalar& Ellipse::a () const
{
	return a_;
}

const Scalar& Ellipse::b () const
{
	return b_;
}

const Scalar& Ellipse::angle () const
{
	return angle_;
}

void Ellipse::set_a (const Scalar& a)
{
	if ( a <= 0 )
		throw ( mexception ( "Invalid ellipse \"a\" axis" ) );
	
	a_ = a;
}

void Ellipse::set_b (const Scalar& b)
{
	if ( b <= 0 )
		throw ( mexception ( "Invalid ellipse \"b\" axis" ) );
	
	b_ = b;
}

void Ellipse::setAngle (const Scalar& angle)
{
	if ( ( angle < 0 ) || ( angle >= 180 ) )
		throw ( mexception ( "Invalid ellipse angle" ) );
	
	angle_ = angle;
}

Scalar Ellipse::area () const
{
	return ( pi * a_ * b_ );
}

bool Ellipse::mouseInside () const
{
	// FIXME
	return false;
}

bool Ellipse::mouseDownInside () const
{
	// FIXME
	return false;
}

bool Ellipse::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

Circle::Circle (
	const R2Vector& r,
	const Scalar& depthconst,
	const Scalar& radius
) : GameObject ( r, depthconst ), radius_ ( radius )
{
	if ( radius <= 0 )
		throw ( mexception ( "Invalid circle radius" ) );
}

Circle::~Circle ()
{
}

void Circle::update()
{
}

void Circle::render()
{
}

GameObject* Circle::clone()
{
	return new Circle( *this );
}

const Scalar& Circle::radius () const
{
	return radius_;
}

void Circle::setRadius (const Scalar& radius)
{
	if ( radius <= 0 )
		throw ( mexception ( "Invalid circle radius" ) );
	
	radius_ = radius;
}

Scalar Circle::length () const
{
	return ( 2 * pi * radius_ );
}

Scalar Circle::area () const
{
	return ( pi * radius_ * radius_ );
}

bool Circle::mouseInside () const
{
	return( range(
		( Camera::pos * depthconst ) +
		r2vec(
			InputManager::instance()->mouseX(),
			InputManager::instance()->mouseY()
		)
	).length() <= radius_ );
}

bool Circle::mouseDownInside () const
{
	return( range(
		( Camera::pos * depthconst ) +
		r2vec(
			InputManager::instance()->mouseDownX(),
			InputManager::instance()->mouseDownY()
		)
	).length() <= radius_ );
}

bool Circle::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Circle::colliding (const Circle& param) const
{
	Scalar dif = sqrt (
	( param.r.x ( 0 ) - r.x ( 0 ) ) * ( param.r.x ( 0 ) - r.x ( 0 ) ) +
	( param.r.x ( 1 ) - r.x ( 1 ) ) * ( param.r.x ( 1 ) - r.x ( 1 ) )
	);
	
	return ( radius_ + param.radius_ >= dif );
}

Line::Line (
	const R2Vector& r,
	const Scalar& depthconst,
	const R2Vector& dir
) : GameObject ( r, depthconst ), dir_ ( dir )
{
	if ( !dir.length () )
		throw ( mexception ( "Invalid line direction" ) );
}

Line::~Line ()
{
}

void Line::update()
{
}

void Line::render()
{
}

GameObject* Line::clone()
{
	return new Line( *this );
}

const R2Vector& Line::dir () const
{
	return dir_;
}

void Line::setDir (const R2Vector& dir)
{
	if ( !dir.length () )
		throw ( mexception ( "Invalid line direction" ) );
	
	dir_ = dir;
}

bool Line::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Line::colliding (const Circle& param) const
{
	R2Vector tmp;
	
	tmp.set ( 0, param.r.x ( 0 ) - r.x ( 0 ) );
	tmp.set ( 1, param.r.x ( 1 ) - r.x ( 1 ) );
	
	Scalar projlen = dir_.proj ( tmp ).length ();
	
	Scalar normal_length = sqrt (
		tmp.length () * tmp.length () -
		projlen * projlen
	);
	
	return ( param.radius () >= normal_length );
}

bool Line::colliding (const Line& param) const
{
	return (	( dir_.x ( 0 ) * param.dir_.x ( 1 ) ) !=
			( dir_.x ( 1 ) * param.dir_.x ( 0 ) )	);
}

LineSegment::LineSegment (
	const R2Vector& r,
	const Scalar& depthconst,
	const R2Vector& dir,
	const Scalar& interval_size,
	const Scalar& t0
) : GameObject ( r, depthconst ), dir_ ( dir ),
interval_size_ ( interval_size ), t0 ( t0 )
{
	if ( !dir.length () )
		throw ( mexception ( "Invalid line segment direction" ) );
	
	if ( interval_size <= 0 )
		throw ( mexception ( "Invalid line segment interval size" ) );
}

LineSegment::~LineSegment ()
{
}

void LineSegment::update()
{
}

void LineSegment::render()
{
}

GameObject* LineSegment::clone()
{
	return new LineSegment( *this );
}

const R2Vector& LineSegment::dir () const
{
	return dir_;
}

const Scalar& LineSegment::interval_size () const
{
	return interval_size_;
}

void LineSegment::setDir (const R2Vector& dir)
{
	if ( !dir.length () )
		throw ( mexception ( "Invalid line segment direction" ) );
	
	dir_ = dir;
}

void LineSegment::setIntervalSize (const Scalar& interval_size)
{
	if ( interval_size <= 0 )
		throw ( mexception ( "Invalid line segment interval size" ) );
	
	interval_size_ = interval_size;
}

Scalar LineSegment::length () const
{
	return ( dir_.length () * interval_size_ );
}

bool LineSegment::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool LineSegment::colliding (const Circle& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool LineSegment::colliding (const Line& param) const
{
	Scalar t = (
		dir_.x ( 0 ) * ( -param.dir ().x ( 1 ) ) -
		dir_.x ( 1 ) * ( -param.dir ().x ( 0 ) )
	);
	
	if ( !t )
		return false;
	
	t = (
		( param.r.x ( 0 ) - r.x ( 0 ) ) * ( -param.dir ().x ( 1 ) ) -
		( param.r.x ( 1 ) - r.x ( 1 ) ) * ( -param.dir ().x ( 0 ) )
	) / t;
	
	return ( ( t >= t0 ) && ( t <= t0 + interval_size_ ) );
}

bool LineSegment::colliding (const LineSegment& param) const
{
	Scalar t, t1, t2;
	
	t = (
		dir_.x ( 0 ) * ( -param.dir_.x ( 1 ) ) -
		dir_.x ( 1 ) * ( -param.dir_.x ( 0 ) )
	);
	
	if ( !t )
		return false;
	
	t1 = (
		( param.r.x ( 0 ) - r.x ( 0 ) ) * ( -param.dir_.x ( 1 ) ) -
		( param.r.x ( 1 ) - r.x ( 1 ) ) * ( -param.dir_.x ( 0 ) )
	) / t;
	
	if ( !( ( t1 >= t0 ) && ( t1 <= t0 + interval_size_ ) ) )
		return false;
	
	t2 = (
		dir_.x ( 0 ) * ( param.r.x ( 1 ) - r.x ( 1 ) ) -
		dir_.x ( 1 ) * ( param.r.x ( 0 ) - r.x ( 0 ) )
	) / t;
	
	return (
		( t2 >= param.t0 ) &&
		( t2 <= param.t0 + param.interval_size_ )
	);
}

Rectangle::Rectangle (
	const R2Vector& r,
	const Scalar& depthconst,
	const Scalar& w,
	const Scalar& h,
	const Scalar& angle
) : GameObject ( r, depthconst ), w_ ( w ), h_ ( h ), angle_ ( angle )
{
	if ( w <= 0 )
		throw ( mexception ( "Invalid rectangle width" ) );
	
	if ( h <= 0 )
		throw ( mexception ( "Invalid rectangle height" ) );
	
	if ( ( angle < 0 ) || ( angle >= 180 ) )
		throw ( mexception ( "Invalid rectangle angle" ) );
}

Rectangle::~Rectangle ()
{
}

void Rectangle::update()
{
}

void Rectangle::render()
{
}

GameObject* Rectangle::clone()
{
	return new Rectangle( *this );
}

const Scalar& Rectangle::w () const
{
	return w_;
}

const Scalar& Rectangle::h () const
{
	return h_;
}

const Scalar& Rectangle::angle () const
{
	return angle_;
}

void Rectangle::setW (const Scalar& w)
{
	if ( w <= 0 )
		throw ( mexception ( "Invalid rectangle width" ) );
	
	w_ = w;
}

void Rectangle::setH (const Scalar& h)
{
	if ( h <= 0 )
		throw ( mexception ( "Invalid rectangle height" ) );
	
	h_ = h;
}

void Rectangle::setAngle (const Scalar& angle)
{
	if ( ( angle < 0 ) || ( angle >= 180 ) )
		throw ( mexception ( "Invalid rectangle angle" ) );
	
	angle_ = angle;
}

Scalar Rectangle::length () const
{
	return ( 2 * ( w_ + h_ ) );
}

Scalar Rectangle::area () const
{
	return ( w_ * h_ );
}

bool Rectangle::mouseInside () const
{
	R2Vector tmp = rotate( -angle_, range(
		( Camera::pos * depthconst ) +
		r2vec(
			InputManager::instance()->mouseX(),
			InputManager::instance()->mouseY()
		)
	) );
	
	return(
		( ABSVAL( tmp.x( 0 ) ) <= w_ / 2 ) &&
		( ABSVAL( tmp.x( 1 ) ) <= h_ / 2 )
	);
}

bool Rectangle::mouseDownInside () const
{
	R2Vector tmp = rotate( -angle_, range(
		( Camera::pos * depthconst ) +
		r2vec(
			InputManager::instance()->mouseDownX(),
			InputManager::instance()->mouseDownY()
		)
	) );
	
	return(
		( ABSVAL( tmp.x( 0 ) ) <= w_ / 2 ) &&
		( ABSVAL( tmp.x( 1 ) ) <= h_ / 2 )
	);
}

bool Rectangle::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Rectangle::colliding (const Circle& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Rectangle::colliding (const Line& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Rectangle::colliding (const LineSegment& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Rectangle::colliding (const Rectangle& param) const
{
	// FIXME
	return ( !( &param ) );
}
