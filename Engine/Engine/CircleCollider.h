#pragma once
#ifndef _CIRCLECOLLIDER_H_
#define _CIRCLECOLLIDER_H_

#include"EngineCore.h"
#include"ICollider.h"
#include"Component.h"
#include"CollisionSystem.h"
/**
 * @class CircleCollider
 *
 * The CircleCollider class is a type of component added to an entity that represents a circle around the entity where collisions will occur
 * It is a circle centred on the entity with a radius of m_radius
 * To check for collisions, use the OnCollisionEnter, OnCollisionStay, or OnCollisionExit functions and consult their implementation
 */
class CircleCollider : public ICollider
{
	DECLARE_DYNAMIC_DERIVED_CLASS(CircleCollider, ICollider);

private:
	float m_radius = 0;
public:
	/**
 * @brief CircleCollider Constructor. Gets transform but not radius
 */
	CircleCollider() = default;
protected:
	~CircleCollider() override = default;

public:
	/**
 * @brief Sets the radius of the collider
 *
 * @param radius A float value that the collider's radius will be set to
 */
	void SetRadius(float radius);
	/**
 * @brief returns radius, really this is just a GetRadius function
 *
 * @return returns the float radius of the collider
 */
	float GetRadius() const;
	float GetBroadPhaseRadius() const override;

	/**
 * @brief Currently unimplemented, would use the isSolid variable to prevent two solid objects from overlapping
 *
 * @param other The other solid collider object that the CollisionSystem has found you to be colliding with
 * @return Always returns true
 */
	bool HandleCollision(ICollider* other) override;

	/**
* @brief Used to tell if this collider is a Circle or Box collider
*
* @return Returns the collider type, in this case CircleCollider
*/
	ColliderType GetType() const override;
};

#endif
