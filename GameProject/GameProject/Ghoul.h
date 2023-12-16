#pragma once

#ifndef _GHOUL_H_
#define _GHOUL_H_

#include "Component.h"

class Ghoul : public Component
{
public:
	static std::string _type;

	void Initialize() override;
	virtual const std::string& GetComponentType();
};

#endif