#pragma once

#ifndef _ZOMBIE_H_
#define _ZOMBIE_H_

#include "Component.h"

class Zombie : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Zombie, Component)

public:
	void Initialize() override;
	virtual const std::string& GetComponentType();
};

#endif