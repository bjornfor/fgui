#include <assert.h>

#include "../src/fgui_3d.h"

void test_fgui_scale1()
{
	int ret;
	struct fgui_vec4 in = {1, 1, 1, 1};
	struct fgui_vec4 out = {0};
	struct fgui_vec4 out_expected = {1, 1, 1, 1};

	fgui_scale(1, 1, 1, &in, &out);
	assert((0 == memcmp(&out, &out_expected, sizeof(in))) && "fgui_scale failed");
}

void test_fgui_scale2()
{
	int ret;
	struct fgui_vec4 in = {1, 1, 1, 1};
	struct fgui_vec4 out = {0};
	struct fgui_vec4 out_expected = {2, 1, 1, 1};

	fgui_scale(2, 1, 1, &in, &out);
	assert((0 == memcmp(&out, &out_expected, sizeof(in))) && "fgui_scale failed");
}

void test_fgui_scale3()
{
	int ret;
	struct fgui_vec4 in = {1, 1, 1, 1};
	struct fgui_vec4 out = {0};
	struct fgui_vec4 out_expected = {1, 2, 2, 1};

	fgui_scale(1, 2, 2, &in, &out);
	assert((0 == memcmp(&out, &out_expected, sizeof(in))) && "fgui_scale failed");
}

void test_fgui_translate1()
{
	int ret;
	struct fgui_vec4 in = {1, 1, 1, 1};
	struct fgui_vec4 out = {0};
	struct fgui_vec4 out_expected = {1, 1, 1, 1};

	fgui_translate(0, 0, 0, &in, &out);
	assert((0 == memcmp(&out, &out_expected, sizeof(in))) && "fgui_translate failed");
}

void test_fgui_translate2()
{
	int ret;
	struct fgui_vec4 in = {1, 1, 1, 1};
	struct fgui_vec4 out = {0};
	struct fgui_vec4 out_expected = {2, 2, 2, 1};

	fgui_translate(1, 1, 1, &in, &out);
	assert((0 == memcmp(&out, &out_expected, sizeof(in))) && "fgui_translate failed");
}

void test_fgui_translate3()
{
	int ret;
	struct fgui_vec4 in = {1, 1, 1, 1};
	struct fgui_vec4 out = {0};
	struct fgui_vec4 out_expected = {3, 2, 2, 1};

	fgui_translate(2, 1, 1, &in, &out);
	assert((0 == memcmp(&out, &out_expected, sizeof(in))) && "fgui_translate failed");
}

void test_fgui_transform_vector1()
{
	int ret;
	struct fgui_vec4 in = {1, 1, 1, 1};
	struct fgui_vec4 out = {0};
	struct fgui_vec4 out_expected = {1, 1, 1, 1};

	fgui_transform_vector(&in, 1,1,1, 0,0,0, 0,0,0, &out);
	assert((0 == memcmp(&out, &out_expected, sizeof(in))) && "fgui_transform_vector failed");
}

void test_fgui_transform_vector2()
{
	int ret;
	struct fgui_vec4 in = {1, 1, 1, 1};
	struct fgui_vec4 out = {0};
	struct fgui_vec4 out_expected = {3, 3, 3, 1};

	fgui_transform_vector(&in, 1,1,1, 0,0,0, 2,2,2, &out);
	assert((0 == memcmp(&out, &out_expected, sizeof(in))) && "fgui_transform_vector failed");
}

int main(int argc, char *argv[])
{
	test_fgui_scale1();
	test_fgui_scale2();
	test_fgui_scale3();

	test_fgui_translate1();
	test_fgui_translate2();
	test_fgui_translate3();

	test_fgui_transform_vector1();
	test_fgui_transform_vector2();

	return 0;
}
