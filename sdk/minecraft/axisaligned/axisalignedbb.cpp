#include "axisalignedbb.h"

#include "../../../main/gasper.h"

c_axisalignedbb::c_axisalignedbb(jobject obj)
{
	axis_aligned_obj = obj;
}

c_axisalignedbb::~c_axisalignedbb()
{
	gasper::instance->get_env()->DeleteLocalRef(axis_aligned_obj);
}

s_axisalignedbb c_axisalignedbb::get_native_boundingbox()
{
	auto this_clazz = gasper::instance->get_env()->GetObjectClass(axis_aligned_obj);

	auto minX_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("a") : xorstr_("field_72340_a"), xorstr_("D")),
		minY_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("b") : xorstr_("field_72338_b"), xorstr_("D")),
		minZ_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("c") : xorstr_("field_72339_c"), xorstr_("D"));

	auto maxX_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("d") : xorstr_("field_72336_d"), xorstr_("D")),
		maxY_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("e") : xorstr_("field_72337_e"), xorstr_("D")),
		maxZ_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("f") : xorstr_("field_72334_f"), xorstr_("D"));

	gasper::instance->get_env()->DeleteLocalRef(this_clazz);

	s_axisalignedbb bb{};
	bb.minX = gasper::instance->get_env()->GetDoubleField(axis_aligned_obj, minX_fid);
	bb.minY = gasper::instance->get_env()->GetDoubleField(axis_aligned_obj, minY_fid);
	bb.minZ = gasper::instance->get_env()->GetDoubleField(axis_aligned_obj, minZ_fid);

	bb.maxX = gasper::instance->get_env()->GetDoubleField(axis_aligned_obj, maxX_fid);
	bb.maxY = gasper::instance->get_env()->GetDoubleField(axis_aligned_obj, maxY_fid);
	bb.maxZ = gasper::instance->get_env()->GetDoubleField(axis_aligned_obj, maxZ_fid);

	return bb;
}

void c_axisalignedbb::set_native_boundingbox(s_axisalignedbb target)
{
	set(target.minX, target.minY, target.minZ, target.maxX, target.maxY, target.maxZ);
}

void c_axisalignedbb::set(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
{
	auto this_clazz = gasper::instance->get_env()->GetObjectClass(axis_aligned_obj);

	auto minX_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("a") : xorstr_("field_72340_a"), xorstr_("D")), 
		minY_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("b") : xorstr_("field_72338_b"), xorstr_("D")), 
		minZ_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("c") : xorstr_("field_72339_c"), xorstr_("D"));

	auto maxX_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("d") : xorstr_("field_72336_d"), xorstr_("D")),
		maxY_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("e") : xorstr_("field_72337_e"), xorstr_("D")),
		maxZ_fid = gasper::instance->get_env()->GetFieldID(this_clazz, BADLION_CLIENT ? xorstr_("f") : xorstr_("field_72334_f"), xorstr_("D"));

	gasper::instance->get_env()->DeleteLocalRef(this_clazz);

	gasper::instance->get_env()->SetDoubleField(axis_aligned_obj, minX_fid, minX);
	gasper::instance->get_env()->SetDoubleField(axis_aligned_obj, minY_fid, minY);
	gasper::instance->get_env()->SetDoubleField(axis_aligned_obj, minZ_fid, minZ);

	gasper::instance->get_env()->SetDoubleField(axis_aligned_obj, maxX_fid, maxX);
	gasper::instance->get_env()->SetDoubleField(axis_aligned_obj, maxY_fid, maxY);
	gasper::instance->get_env()->SetDoubleField(axis_aligned_obj, maxZ_fid, maxZ);

}
