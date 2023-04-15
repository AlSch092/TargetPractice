#pragma once

namespace Phys
{
	struct Vector3 //vector represents line going in some direction forever
	{
		float X, Y, Z;
	};

	struct Point3
	{
		float X, Y, Z;
	};

	struct Line3
	{
		Point3 Position;
		Vector3 Direction;
	};
}