#ifndef _MEDIA_MT9V032_H
#define _MEDIA_MT9V032_H

<<<<<<< HEAD
<<<<<<< HEAD
struct v4l2_subdev;

struct mt9v032_platform_data {
	unsigned int clk_pol:1;

	void (*set_clock)(struct v4l2_subdev *subdev, unsigned int rate);

=======
struct mt9v032_platform_data {
	unsigned int clk_pol:1;

>>>>>>> v3.18
=======
struct mt9v032_platform_data {
	unsigned int clk_pol:1;

>>>>>>> v3.18
	const s64 *link_freqs;
	s64 link_def_freq;
};

#endif
