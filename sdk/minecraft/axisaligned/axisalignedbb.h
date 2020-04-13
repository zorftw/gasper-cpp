#pragma once

#include "../minecraft.h"

struct s_axisalignedbb {
	double minX, minY, minZ,
		maxX, maxY, maxZ;
};

class c_axisalignedbb {
private:
	jobject axis_aligned_obj;
public:
	c_axisalignedbb(jobject);
	~c_axisalignedbb();

	s_axisalignedbb get_native_boundingbox();
	void set_native_boundingbox(s_axisalignedbb target);

	void set(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);
};