
#ifndef ELECTROMAGNETISM_HPP
#define ELECTROMAGNETISM_HPP

#include "Geometry.hpp"
#include "TileSet.hpp"

namespace phys
{

const lalge::Scalar elec_mass = 9.10938188e-31;
const lalge::Scalar prot_mass = 1.67262158e-27;

const lalge::Scalar e_charge = 1.60217646e-19;

const lalge::Scalar vac_permit = 8.85418782e-12;
const lalge::Scalar vac_permea = 1.25663706e-6;

const lalge::Scalar grav_const = 6.673e-11;
const lalge::Scalar elec_const = ( (lalge::Scalar) 1 / ( 4 * lalge::pi * vac_permit ) );
const lalge::Scalar mag_const = ( (lalge::Scalar) vac_permea / ( 4 * lalge::pi ) );

}

class InvElasticConst {};

class GravField
{
public:
	virtual lalge::R2Vector evaluate (const lalge::R2Vector& r) const = 0;
};

class ElecField
{
public:
	virtual lalge::R2Vector evaluate (const lalge::R2Vector& r) const = 0;
};

class MagField
{
public:
	virtual lalge::R3Vector evaluate (const lalge::R2Vector& r) const = 0;
};

class GravFieldConst : public GravField
{
public:
	lalge::R2Vector val;
	
	GravFieldConst (const lalge::R2Vector& val = lalge::R2Vector ());
	
	lalge::R2Vector evaluate (const lalge::R2Vector& r) const;
};

class ElecFieldConst : public ElecField
{
public:
	lalge::R2Vector val;
	
	ElecFieldConst (const lalge::R2Vector& val = lalge::R2Vector ());
	
	lalge::R2Vector evaluate (const lalge::R2Vector& r) const;
};

class MagFieldConst : public MagField
{
public:
	lalge::R3Vector val;
	
	MagFieldConst (const lalge::R3Vector& val = lalge::R3Vector ());
	
	lalge::R3Vector evaluate (const lalge::R2Vector& r) const;
};

class Barrier : public Line
{
private:
	lalge::Scalar k_;
public:
	Barrier (const lalge::Scalar& k = 0.25);
	virtual ~Barrier ();
	
	const lalge::Scalar& k () const;
	
	void set_k (const lalge::Scalar& k);
};

class Particle : public Circle
{
public:
	class NoMass {};
	class AlreadyPinned {};
	class AlreadyUnpinned {};
	class NegativeDT {};
private:
	lalge::Scalar k_;
	lalge::Scalar m_;
public:
	lalge::Scalar q;
	
	lalge::R2Vector v;
	lalge::R2Vector a;
	lalge::R2Vector F;
private:
	bool pinned_;
	
	TileSet* tileset;
public:
	Particle (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		const lalge::Scalar& radius = 1,
		const lalge::Scalar& k = 0.25,
		const lalge::Scalar& m = 1,
		const lalge::Scalar& q = 0,
		const lalge::R2Vector& v = lalge::R2Vector (),
		const lalge::R2Vector& a = lalge::R2Vector (),
		const lalge::R2Vector& F = lalge::R2Vector (),
		const bool& pinned = true
	);
	virtual ~Particle ();
	
	virtual void update ();
	
	virtual void render ();
	
	virtual GameObject* clone () const;
	
	const lalge::Scalar& k () const;
	const lalge::Scalar& m () const;
	const bool& pinned () const;
	
	void setTileSet (TileSet* tileset);
	void set_k (const lalge::Scalar& k);
	void set_m (const lalge::Scalar& m);
	void pin ();
	void unpin ();
	
	lalge::R2Vector velocity (const Particle& param) const;
	
	// best updating order: setFieldAccel, respondColl, update_v_r
	
	lalge::R2Vector fieldForce (const Particle& param) const;
	lalge::R2Vector gravForce (const Particle& param) const;
	lalge::R2Vector elecForce (const Particle& param) const;
	lalge::R2Vector magForce (const Particle& param) const; // FIXME
	
	lalge::R2Vector gravForce (const GravField& g) const;
	lalge::R2Vector elecForce (const ElecField& E) const;
	lalge::R2Vector magForce (const MagField& B) const;
	
	void setFieldAccel (const lalge::R2Vector& field_net_force);
	
	bool colliding (const Particle& param, const lalge::Scalar& dt) const;
	bool colliding (const Barrier& param, const lalge::Scalar& dt) const;
	
	void respondColl (Particle& param);
	void respondColl (const Barrier& param);
	
	void update_v_r (const lalge::Scalar& dt);
	
	void handleMouseUpLeft ();
};

class Avatar : public Particle
{
public:
	lalge::Scalar max_charge;
	lalge::Scalar min_charge;
	
	Avatar (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		const lalge::Scalar& radius = 1,
		const lalge::Scalar& k = 0.25,
		const lalge::Scalar& m = 1,
		const lalge::Scalar& q = 0,
		const lalge::R2Vector& v = lalge::R2Vector (),
		const lalge::R2Vector& a = lalge::R2Vector (),
		const lalge::R2Vector& F = lalge::R2Vector (),
		const bool& pinned = true
	);
	
	void update ();
};

#endif
