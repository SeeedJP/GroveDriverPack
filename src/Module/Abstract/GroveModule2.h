#pragma once

class GroveModule2
{
protected:
	bool _IsExist;

public:
	GroveModule2()
	{
		_IsExist = false;
	}

	bool IsExist() const
	{
		return _IsExist;
	}

	virtual bool Init() = 0;


};
