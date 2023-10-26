/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

template<class T>
class SerialCounter
{
	T m_Value = 0;
public:
	inline T Allocate()
	{
		return m_Value++;
	}
};

