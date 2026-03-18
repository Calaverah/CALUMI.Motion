//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#pragma warning(push,0)
#include <QDialogButtonBox>
#pragma warning(pop)

class IButtonBox
{
public:
	virtual QDialogButtonBox* GetButtonBox() const = 0;
};