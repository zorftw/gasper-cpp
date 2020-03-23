#pragma once

namespace sdk {

	struct vec3d {
		double x, y, z;
	};

	using qword = unsigned long long;
	using t_createdvms = jint(__stdcall*)(JavaVM**, jsize, jsize*);

	namespace util {
		inline float wrap_to_180(float value)
		{
			if (value >= 180.f)
				value -= 360.f;
			if (value < -180.f)
				value += 360.f;
			return value;
		}

		inline float radiants_to_deg(float x)
		{
			return x * 180.f / M_PI;
		}

		inline std::pair<float, float> get_angles(sdk::vec3d pos, sdk::vec3d pos1)
		{
			double d_x = pos1.x - pos.x;
			double d_y = pos1.y - pos.y;
			double d_z = pos1.z - pos.z;

			double hypothenuse = sqrt(d_x * d_x + d_z * d_z);
			float yaw = radiants_to_deg(atan2(d_z, d_x)) - 90.f;
			float pitch = radiants_to_deg(-atan2(d_y, hypothenuse));

			return std::make_pair(yaw, pitch);
		}

		inline static double distance(double x, double y) {
			return sqrt(pow(x, 2) + pow(y, 2));
		}

		inline static double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
			return distance(y1 - y2, distance(x1 - x2, z1 - z2));
		}
	}
};