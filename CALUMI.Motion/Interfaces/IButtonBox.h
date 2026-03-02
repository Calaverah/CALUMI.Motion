#pragma once

#pragma warning(push,0)
#include <QDialogButtonBox>
#pragma warning(pop)

class IButtonBox
{
public:
	virtual QDialogButtonBox* GetButtonBox() const = 0;
};