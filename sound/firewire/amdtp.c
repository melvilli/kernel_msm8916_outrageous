/*
 * Audio and Music Data Transmission Protocol (IEC 61883-6) streams
 * with Common Isochronous Packet (IEC 61883-1) headers
 *
 * Copyright (c) Clemens Ladisch <clemens@ladisch.de>
 * Licensed under the terms of the GNU General Public License, version 2.
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/firewire.h>
#include <linux/module.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <sound/pcm.h>
=======
#include <linux/sched.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/rawmidi.h>
>>>>>>> v3.18
#include "amdtp.h"

#define TICKS_PER_CYCLE		3072
#define CYCLES_PER_SECOND	8000
#define TICKS_PER_SECOND	(TICKS_PER_CYCLE * CYCLES_PER_SECOND)

#define TRANSFER_DELAY_TICKS	0x2e00 /* 479.17 µs */

<<<<<<< HEAD
#define TAG_CIP			1

#define CIP_EOH			(1u << 31)
#define CIP_FMT_AM		(0x10 << 24)
#define AMDTP_FDF_AM824		(0 << 19)
#define AMDTP_FDF_SFC_SHIFT	16
=======
/* isochronous header parameters */
#define ISO_DATA_LENGTH_SHIFT	16
#define TAG_CIP			1

/* common isochronous packet header parameters */
#define CIP_EOH			(1u << 31)
#define CIP_EOH_MASK		0x80000000
#define CIP_FMT_AM		(0x10 << 24)
#define CIP_FMT_MASK		0x3f000000
#define CIP_SYT_MASK		0x0000ffff
#define CIP_SYT_NO_INFO		0xffff
#define CIP_FDF_MASK		0x00ff0000
#define CIP_FDF_SFC_SHIFT	16

/*
 * Audio and Music transfer protocol specific parameters
 * only "Clock-based rate control mode" is supported
 */
#define AMDTP_FDF_AM824		(0 << (CIP_FDF_SFC_SHIFT + 3))
#define AMDTP_FDF_NO_DATA	0xff
#define AMDTP_DBS_MASK		0x00ff0000
#define AMDTP_DBS_SHIFT		16
#define AMDTP_DBC_MASK		0x000000ff
>>>>>>> v3.18

/* TODO: make these configurable */
#define INTERRUPT_INTERVAL	16
#define QUEUE_LENGTH		48

<<<<<<< HEAD
static void pcm_period_tasklet(unsigned long data);

/**
 * amdtp_out_stream_init - initialize an AMDTP output stream structure
 * @s: the AMDTP output stream to initialize
 * @unit: the target of the stream
 * @flags: the packet transmission method to use
 */
int amdtp_out_stream_init(struct amdtp_out_stream *s, struct fw_unit *unit,
			  enum cip_out_flags flags)
{
	if (flags != CIP_NONBLOCKING)
		return -EINVAL;

	s->unit = fw_unit_get(unit);
=======
#define IN_PACKET_HEADER_SIZE	4
#define OUT_PACKET_HEADER_SIZE	0

static void pcm_period_tasklet(unsigned long data);

/**
 * amdtp_stream_init - initialize an AMDTP stream structure
 * @s: the AMDTP stream to initialize
 * @unit: the target of the stream
 * @dir: the direction of stream
 * @flags: the packet transmission method to use
 */
int amdtp_stream_init(struct amdtp_stream *s, struct fw_unit *unit,
		      enum amdtp_stream_direction dir, enum cip_flags flags)
{
	s->unit = fw_unit_get(unit);
	s->direction = dir;
>>>>>>> v3.18
	s->flags = flags;
	s->context = ERR_PTR(-1);
	mutex_init(&s->mutex);
	tasklet_init(&s->period_tasklet, pcm_period_tasklet, (unsigned long)s);
	s->packet_index = 0;

<<<<<<< HEAD
	return 0;
}
EXPORT_SYMBOL(amdtp_out_stream_init);

/**
 * amdtp_out_stream_destroy - free stream resources
 * @s: the AMDTP output stream to destroy
 */
void amdtp_out_stream_destroy(struct amdtp_out_stream *s)
{
	WARN_ON(!IS_ERR(s->context));
	mutex_destroy(&s->mutex);
	fw_unit_put(s->unit);
}
EXPORT_SYMBOL(amdtp_out_stream_destroy);

/**
 * amdtp_out_stream_set_rate - set the sample rate
 * @s: the AMDTP output stream to configure
 * @rate: the sample rate
 *
 * The sample rate must be set before the stream is started, and must not be
 * changed while the stream is running.
 */
void amdtp_out_stream_set_rate(struct amdtp_out_stream *s, unsigned int rate)
{
	static const struct {
		unsigned int rate;
		unsigned int syt_interval;
	} rate_info[] = {
		[CIP_SFC_32000]  = {  32000,  8, },
		[CIP_SFC_44100]  = {  44100,  8, },
		[CIP_SFC_48000]  = {  48000,  8, },
		[CIP_SFC_88200]  = {  88200, 16, },
		[CIP_SFC_96000]  = {  96000, 16, },
		[CIP_SFC_176400] = { 176400, 32, },
		[CIP_SFC_192000] = { 192000, 32, },
	};
	unsigned int sfc;

	if (WARN_ON(!IS_ERR(s->context)))
		return;

	for (sfc = 0; sfc < ARRAY_SIZE(rate_info); ++sfc)
		if (rate_info[sfc].rate == rate) {
			s->sfc = sfc;
			s->syt_interval = rate_info[sfc].syt_interval;
			return;
		}
	WARN_ON(1);
}
EXPORT_SYMBOL(amdtp_out_stream_set_rate);

/**
 * amdtp_out_stream_get_max_payload - get the stream's packet size
 * @s: the AMDTP output stream
 *
 * This function must not be called before the stream has been configured
 * with amdtp_out_stream_set_hw_params(), amdtp_out_stream_set_pcm(), and
 * amdtp_out_stream_set_midi().
 */
unsigned int amdtp_out_stream_get_max_payload(struct amdtp_out_stream *s)
{
	static const unsigned int max_data_blocks[] = {
		[CIP_SFC_32000]  =  4,
		[CIP_SFC_44100]  =  6,
		[CIP_SFC_48000]  =  6,
		[CIP_SFC_88200]  = 12,
		[CIP_SFC_96000]  = 12,
		[CIP_SFC_176400] = 23,
		[CIP_SFC_192000] = 24,
	};

	s->data_block_quadlets = s->pcm_channels;
	s->data_block_quadlets += DIV_ROUND_UP(s->midi_ports, 8);

	return 8 + max_data_blocks[s->sfc] * 4 * s->data_block_quadlets;
}
EXPORT_SYMBOL(amdtp_out_stream_get_max_payload);

static void amdtp_write_s16(struct amdtp_out_stream *s,
			    struct snd_pcm_substream *pcm,
			    __be32 *buffer, unsigned int frames);
static void amdtp_write_s32(struct amdtp_out_stream *s,
			    struct snd_pcm_substream *pcm,
			    __be32 *buffer, unsigned int frames);

/**
 * amdtp_out_stream_set_pcm_format - set the PCM format
 * @s: the AMDTP output stream to configure
 * @format: the format of the ALSA PCM device
 *
 * The sample format must be set before the stream is started, and must not be
 * changed while the stream is running.
 */
void amdtp_out_stream_set_pcm_format(struct amdtp_out_stream *s,
				     snd_pcm_format_t format)
{
	if (WARN_ON(!IS_ERR(s->context)))
=======
	init_waitqueue_head(&s->callback_wait);
	s->callbacked = false;
	s->sync_slave = NULL;

	s->rx_blocks_for_midi = UINT_MAX;

	return 0;
}
EXPORT_SYMBOL(amdtp_stream_init);

/**
 * amdtp_stream_destroy - free stream resources
 * @s: the AMDTP stream to destroy
 */
void amdtp_stream_destroy(struct amdtp_stream *s)
{
	WARN_ON(amdtp_stream_running(s));
	mutex_destroy(&s->mutex);
	fw_unit_put(s->unit);
}
EXPORT_SYMBOL(amdtp_stream_destroy);

const unsigned int amdtp_syt_intervals[CIP_SFC_COUNT] = {
	[CIP_SFC_32000]  =  8,
	[CIP_SFC_44100]  =  8,
	[CIP_SFC_48000]  =  8,
	[CIP_SFC_88200]  = 16,
	[CIP_SFC_96000]  = 16,
	[CIP_SFC_176400] = 32,
	[CIP_SFC_192000] = 32,
};
EXPORT_SYMBOL(amdtp_syt_intervals);

const unsigned int amdtp_rate_table[CIP_SFC_COUNT] = {
	[CIP_SFC_32000]  =  32000,
	[CIP_SFC_44100]  =  44100,
	[CIP_SFC_48000]  =  48000,
	[CIP_SFC_88200]  =  88200,
	[CIP_SFC_96000]  =  96000,
	[CIP_SFC_176400] = 176400,
	[CIP_SFC_192000] = 192000,
};
EXPORT_SYMBOL(amdtp_rate_table);

/**
 * amdtp_stream_add_pcm_hw_constraints - add hw constraints for PCM substream
 * @s:		the AMDTP stream, which must be initialized.
 * @runtime:	the PCM substream runtime
 */
int amdtp_stream_add_pcm_hw_constraints(struct amdtp_stream *s,
					struct snd_pcm_runtime *runtime)
{
	int err;

	/* AM824 in IEC 61883-6 can deliver 24bit data */
	err = snd_pcm_hw_constraint_msbits(runtime, 0, 32, 24);
	if (err < 0)
		goto end;

	/*
	 * Currently firewire-lib processes 16 packets in one software
	 * interrupt callback. This equals to 2msec but actually the
	 * interval of the interrupts has a jitter.
	 * Additionally, even if adding a constraint to fit period size to
	 * 2msec, actual calculated frames per period doesn't equal to 2msec,
	 * depending on sampling rate.
	 * Anyway, the interval to call snd_pcm_period_elapsed() cannot 2msec.
	 * Here let us use 5msec for safe period interrupt.
	 */
	err = snd_pcm_hw_constraint_minmax(runtime,
					   SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					   5000, UINT_MAX);
	if (err < 0)
		goto end;

	/* Non-Blocking stream has no more constraints */
	if (!(s->flags & CIP_BLOCKING))
		goto end;

	/*
	 * One AMDTP packet can include some frames. In blocking mode, the
	 * number equals to SYT_INTERVAL. So the number is 8, 16 or 32,
	 * depending on its sampling rate. For accurate period interrupt, it's
	 * preferrable to aligh period/buffer sizes to current SYT_INTERVAL.
	 *
	 * TODO: These constraints can be improved with propper rules.
	 * Currently apply LCM of SYT_INTEVALs.
	 */
	err = snd_pcm_hw_constraint_step(runtime, 0,
					 SNDRV_PCM_HW_PARAM_PERIOD_SIZE, 32);
	if (err < 0)
		goto end;
	err = snd_pcm_hw_constraint_step(runtime, 0,
					 SNDRV_PCM_HW_PARAM_BUFFER_SIZE, 32);
end:
	return err;
}
EXPORT_SYMBOL(amdtp_stream_add_pcm_hw_constraints);

/**
 * amdtp_stream_set_parameters - set stream parameters
 * @s: the AMDTP stream to configure
 * @rate: the sample rate
 * @pcm_channels: the number of PCM samples in each data block, to be encoded
 *                as AM824 multi-bit linear audio
 * @midi_ports: the number of MIDI ports (i.e., MPX-MIDI Data Channels)
 *
 * The parameters must be set before the stream is started, and must not be
 * changed while the stream is running.
 */
void amdtp_stream_set_parameters(struct amdtp_stream *s,
				 unsigned int rate,
				 unsigned int pcm_channels,
				 unsigned int midi_ports)
{
	unsigned int i, sfc, midi_channels;

	midi_channels = DIV_ROUND_UP(midi_ports, 8);

	if (WARN_ON(amdtp_stream_running(s)) |
	    WARN_ON(pcm_channels > AMDTP_MAX_CHANNELS_FOR_PCM) |
	    WARN_ON(midi_channels > AMDTP_MAX_CHANNELS_FOR_MIDI))
		return;

	for (sfc = 0; sfc < ARRAY_SIZE(amdtp_rate_table); ++sfc)
		if (amdtp_rate_table[sfc] == rate)
			goto sfc_found;
	WARN_ON(1);
	return;

sfc_found:
	s->pcm_channels = pcm_channels;
	s->sfc = sfc;
	s->data_block_quadlets = s->pcm_channels + midi_channels;
	s->midi_ports = midi_ports;

	s->syt_interval = amdtp_syt_intervals[sfc];

	/* default buffering in the device */
	s->transfer_delay = TRANSFER_DELAY_TICKS - TICKS_PER_CYCLE;
	if (s->flags & CIP_BLOCKING)
		/* additional buffering needed to adjust for no-data packets */
		s->transfer_delay += TICKS_PER_SECOND * s->syt_interval / rate;

	/* init the position map for PCM and MIDI channels */
	for (i = 0; i < pcm_channels; i++)
		s->pcm_positions[i] = i;
	s->midi_position = s->pcm_channels;
}
EXPORT_SYMBOL(amdtp_stream_set_parameters);

/**
 * amdtp_stream_get_max_payload - get the stream's packet size
 * @s: the AMDTP stream
 *
 * This function must not be called before the stream has been configured
 * with amdtp_stream_set_parameters().
 */
unsigned int amdtp_stream_get_max_payload(struct amdtp_stream *s)
{
	return 8 + s->syt_interval * s->data_block_quadlets * 4;
}
EXPORT_SYMBOL(amdtp_stream_get_max_payload);

static void amdtp_write_s16(struct amdtp_stream *s,
			    struct snd_pcm_substream *pcm,
			    __be32 *buffer, unsigned int frames);
static void amdtp_write_s32(struct amdtp_stream *s,
			    struct snd_pcm_substream *pcm,
			    __be32 *buffer, unsigned int frames);
static void amdtp_read_s32(struct amdtp_stream *s,
			   struct snd_pcm_substream *pcm,
			   __be32 *buffer, unsigned int frames);

/**
 * amdtp_stream_set_pcm_format - set the PCM format
 * @s: the AMDTP stream to configure
 * @format: the format of the ALSA PCM device
 *
 * The sample format must be set after the other paramters (rate/PCM channels/
 * MIDI) and before the stream is started, and must not be changed while the
 * stream is running.
 */
void amdtp_stream_set_pcm_format(struct amdtp_stream *s,
				 snd_pcm_format_t format)
{
	if (WARN_ON(amdtp_stream_pcm_running(s)))
>>>>>>> v3.18
		return;

	switch (format) {
	default:
		WARN_ON(1);
		/* fall through */
	case SNDRV_PCM_FORMAT_S16:
<<<<<<< HEAD
		s->transfer_samples = amdtp_write_s16;
		break;
	case SNDRV_PCM_FORMAT_S32:
		s->transfer_samples = amdtp_write_s32;
		break;
	}
}
EXPORT_SYMBOL(amdtp_out_stream_set_pcm_format);

/**
 * amdtp_out_stream_pcm_prepare - prepare PCM device for running
 * @s: the AMDTP output stream
 *
 * This function should be called from the PCM device's .prepare callback.
 */
void amdtp_out_stream_pcm_prepare(struct amdtp_out_stream *s)
=======
		if (s->direction == AMDTP_OUT_STREAM) {
			s->transfer_samples = amdtp_write_s16;
			break;
		}
		WARN_ON(1);
		/* fall through */
	case SNDRV_PCM_FORMAT_S32:
		if (s->direction == AMDTP_OUT_STREAM)
			s->transfer_samples = amdtp_write_s32;
		else
			s->transfer_samples = amdtp_read_s32;
		break;
	}
}
EXPORT_SYMBOL(amdtp_stream_set_pcm_format);

/**
 * amdtp_stream_pcm_prepare - prepare PCM device for running
 * @s: the AMDTP stream
 *
 * This function should be called from the PCM device's .prepare callback.
 */
void amdtp_stream_pcm_prepare(struct amdtp_stream *s)
>>>>>>> v3.18
{
	tasklet_kill(&s->period_tasklet);
	s->pcm_buffer_pointer = 0;
	s->pcm_period_pointer = 0;
	s->pointer_flush = true;
}
<<<<<<< HEAD
EXPORT_SYMBOL(amdtp_out_stream_pcm_prepare);

static unsigned int calculate_data_blocks(struct amdtp_out_stream *s)
{
	unsigned int phase, data_blocks;

	if (!cip_sfc_is_base_44100(s->sfc)) {
=======
EXPORT_SYMBOL(amdtp_stream_pcm_prepare);

static unsigned int calculate_data_blocks(struct amdtp_stream *s)
{
	unsigned int phase, data_blocks;

	if (s->flags & CIP_BLOCKING)
		data_blocks = s->syt_interval;
	else if (!cip_sfc_is_base_44100(s->sfc)) {
>>>>>>> v3.18
		/* Sample_rate / 8000 is an integer, and precomputed. */
		data_blocks = s->data_block_state;
	} else {
		phase = s->data_block_state;

		/*
		 * This calculates the number of data blocks per packet so that
		 * 1) the overall rate is correct and exactly synchronized to
		 *    the bus clock, and
		 * 2) packets with a rounded-up number of blocks occur as early
		 *    as possible in the sequence (to prevent underruns of the
		 *    device's buffer).
		 */
		if (s->sfc == CIP_SFC_44100)
			/* 6 6 5 6 5 6 5 ... */
			data_blocks = 5 + ((phase & 1) ^
					   (phase == 0 || phase >= 40));
		else
			/* 12 11 11 11 11 ... or 23 22 22 22 22 ... */
			data_blocks = 11 * (s->sfc >> 1) + (phase == 0);
		if (++phase >= (80 >> (s->sfc >> 1)))
			phase = 0;
		s->data_block_state = phase;
	}

	return data_blocks;
}

<<<<<<< HEAD
static unsigned int calculate_syt(struct amdtp_out_stream *s,
=======
static unsigned int calculate_syt(struct amdtp_stream *s,
>>>>>>> v3.18
				  unsigned int cycle)
{
	unsigned int syt_offset, phase, index, syt;

	if (s->last_syt_offset < TICKS_PER_CYCLE) {
		if (!cip_sfc_is_base_44100(s->sfc))
			syt_offset = s->last_syt_offset + s->syt_offset_state;
		else {
		/*
		 * The time, in ticks, of the n'th SYT_INTERVAL sample is:
		 *   n * SYT_INTERVAL * 24576000 / sample_rate
		 * Modulo TICKS_PER_CYCLE, the difference between successive
		 * elements is about 1386.23.  Rounding the results of this
		 * formula to the SYT precision results in a sequence of
		 * differences that begins with:
		 *   1386 1386 1387 1386 1386 1386 1387 1386 1386 1386 1387 ...
		 * This code generates _exactly_ the same sequence.
		 */
			phase = s->syt_offset_state;
			index = phase % 13;
			syt_offset = s->last_syt_offset;
			syt_offset += 1386 + ((index && !(index & 3)) ||
					      phase == 146);
			if (++phase >= 147)
				phase = 0;
			s->syt_offset_state = phase;
		}
	} else
		syt_offset = s->last_syt_offset - TICKS_PER_CYCLE;
	s->last_syt_offset = syt_offset;

	if (syt_offset < TICKS_PER_CYCLE) {
<<<<<<< HEAD
		syt_offset += TRANSFER_DELAY_TICKS - TICKS_PER_CYCLE;
		syt = (cycle + syt_offset / TICKS_PER_CYCLE) << 12;
		syt += syt_offset % TICKS_PER_CYCLE;

		return syt & 0xffff;
	} else {
		return 0xffff; /* no info */
	}
}

static void amdtp_write_s32(struct amdtp_out_stream *s,
=======
		syt_offset += s->transfer_delay;
		syt = (cycle + syt_offset / TICKS_PER_CYCLE) << 12;
		syt += syt_offset % TICKS_PER_CYCLE;

		return syt & CIP_SYT_MASK;
	} else {
		return CIP_SYT_NO_INFO;
	}
}

static void amdtp_write_s32(struct amdtp_stream *s,
>>>>>>> v3.18
			    struct snd_pcm_substream *pcm,
			    __be32 *buffer, unsigned int frames)
{
	struct snd_pcm_runtime *runtime = pcm->runtime;
<<<<<<< HEAD
	unsigned int channels, remaining_frames, frame_step, i, c;
=======
	unsigned int channels, remaining_frames, i, c;
>>>>>>> v3.18
	const u32 *src;

	channels = s->pcm_channels;
	src = (void *)runtime->dma_area +
<<<<<<< HEAD
			s->pcm_buffer_pointer * (runtime->frame_bits / 8);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;
	frame_step = s->data_block_quadlets - channels;

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c) {
			*buffer = cpu_to_be32((*src >> 8) | 0x40000000);
			src++;
			buffer++;
		}
		buffer += frame_step;
=======
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c) {
			buffer[s->pcm_positions[c]] =
					cpu_to_be32((*src >> 8) | 0x40000000);
			src++;
		}
		buffer += s->data_block_quadlets;
>>>>>>> v3.18
		if (--remaining_frames == 0)
			src = (void *)runtime->dma_area;
	}
}

<<<<<<< HEAD
static void amdtp_write_s16(struct amdtp_out_stream *s,
=======
static void amdtp_write_s16(struct amdtp_stream *s,
>>>>>>> v3.18
			    struct snd_pcm_substream *pcm,
			    __be32 *buffer, unsigned int frames)
{
	struct snd_pcm_runtime *runtime = pcm->runtime;
<<<<<<< HEAD
	unsigned int channels, remaining_frames, frame_step, i, c;
=======
	unsigned int channels, remaining_frames, i, c;
>>>>>>> v3.18
	const u16 *src;

	channels = s->pcm_channels;
	src = (void *)runtime->dma_area +
<<<<<<< HEAD
			s->pcm_buffer_pointer * (runtime->frame_bits / 8);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;
	frame_step = s->data_block_quadlets - channels;

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c) {
			*buffer = cpu_to_be32((*src << 8) | 0x40000000);
			src++;
			buffer++;
		}
		buffer += frame_step;
=======
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c) {
			buffer[s->pcm_positions[c]] =
					cpu_to_be32((*src << 8) | 0x42000000);
			src++;
		}
		buffer += s->data_block_quadlets;
>>>>>>> v3.18
		if (--remaining_frames == 0)
			src = (void *)runtime->dma_area;
	}
}

<<<<<<< HEAD
static void amdtp_fill_pcm_silence(struct amdtp_out_stream *s,
=======
static void amdtp_read_s32(struct amdtp_stream *s,
			   struct snd_pcm_substream *pcm,
			   __be32 *buffer, unsigned int frames)
{
	struct snd_pcm_runtime *runtime = pcm->runtime;
	unsigned int channels, remaining_frames, i, c;
	u32 *dst;

	channels = s->pcm_channels;
	dst  = (void *)runtime->dma_area +
			frames_to_bytes(runtime, s->pcm_buffer_pointer);
	remaining_frames = runtime->buffer_size - s->pcm_buffer_pointer;

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < channels; ++c) {
			*dst = be32_to_cpu(buffer[s->pcm_positions[c]]) << 8;
			dst++;
		}
		buffer += s->data_block_quadlets;
		if (--remaining_frames == 0)
			dst = (void *)runtime->dma_area;
	}
}

static void amdtp_fill_pcm_silence(struct amdtp_stream *s,
>>>>>>> v3.18
				   __be32 *buffer, unsigned int frames)
{
	unsigned int i, c;

	for (i = 0; i < frames; ++i) {
		for (c = 0; c < s->pcm_channels; ++c)
<<<<<<< HEAD
			buffer[c] = cpu_to_be32(0x40000000);
=======
			buffer[s->pcm_positions[c]] = cpu_to_be32(0x40000000);
>>>>>>> v3.18
		buffer += s->data_block_quadlets;
	}
}

<<<<<<< HEAD
static void amdtp_fill_midi(struct amdtp_out_stream *s,
			    __be32 *buffer, unsigned int frames)
{
	unsigned int i;

	for (i = 0; i < frames; ++i)
		buffer[s->pcm_channels + i * s->data_block_quadlets] =
						cpu_to_be32(0x80000000);
}

static void queue_out_packet(struct amdtp_out_stream *s, unsigned int cycle)
{
	__be32 *buffer;
	unsigned int index, data_blocks, syt, ptr;
	struct snd_pcm_substream *pcm;
	struct fw_iso_packet packet;
	int err;

	if (s->packet_index < 0)
		return;
	index = s->packet_index;

	data_blocks = calculate_data_blocks(s);
	syt = calculate_syt(s, cycle);

	buffer = s->buffer.packets[index].buffer;
	buffer[0] = cpu_to_be32(ACCESS_ONCE(s->source_node_id_field) |
				(s->data_block_quadlets << 16) |
				s->data_block_counter);
	buffer[1] = cpu_to_be32(CIP_EOH | CIP_FMT_AM | AMDTP_FDF_AM824 |
				(s->sfc << AMDTP_FDF_SFC_SHIFT) | syt);
=======
static void amdtp_fill_midi(struct amdtp_stream *s,
			    __be32 *buffer, unsigned int frames)
{
	unsigned int f, port;
	u8 *b;

	for (f = 0; f < frames; f++) {
		buffer[s->midi_position] = 0;
		b = (u8 *)&buffer[s->midi_position];

		port = (s->data_block_counter + f) % 8;
		if ((f >= s->rx_blocks_for_midi) ||
		    (s->midi[port] == NULL) ||
		    (snd_rawmidi_transmit(s->midi[port], b + 1, 1) <= 0))
			b[0] = 0x80;
		else
			b[0] = 0x81;

		buffer += s->data_block_quadlets;
	}
}

static void amdtp_pull_midi(struct amdtp_stream *s,
			    __be32 *buffer, unsigned int frames)
{
	unsigned int f, port;
	int len;
	u8 *b;

	for (f = 0; f < frames; f++) {
		port = (s->data_block_counter + f) % 8;
		b = (u8 *)&buffer[s->midi_position];

		len = b[0] - 0x80;
		if ((1 <= len) &&  (len <= 3) && (s->midi[port]))
			snd_rawmidi_receive(s->midi[port], b + 1, len);

		buffer += s->data_block_quadlets;
	}
}

static void update_pcm_pointers(struct amdtp_stream *s,
				struct snd_pcm_substream *pcm,
				unsigned int frames)
{
	unsigned int ptr;

	/*
	 * In IEC 61883-6, one data block represents one event. In ALSA, one
	 * event equals to one PCM frame. But Dice has a quirk to transfer
	 * two PCM frames in one data block.
	 */
	if (s->double_pcm_frames)
		frames *= 2;

	ptr = s->pcm_buffer_pointer + frames;
	if (ptr >= pcm->runtime->buffer_size)
		ptr -= pcm->runtime->buffer_size;
	ACCESS_ONCE(s->pcm_buffer_pointer) = ptr;

	s->pcm_period_pointer += frames;
	if (s->pcm_period_pointer >= pcm->runtime->period_size) {
		s->pcm_period_pointer -= pcm->runtime->period_size;
		s->pointer_flush = false;
		tasklet_hi_schedule(&s->period_tasklet);
	}
}

static void pcm_period_tasklet(unsigned long data)
{
	struct amdtp_stream *s = (void *)data;
	struct snd_pcm_substream *pcm = ACCESS_ONCE(s->pcm);

	if (pcm)
		snd_pcm_period_elapsed(pcm);
}

static int queue_packet(struct amdtp_stream *s,
			unsigned int header_length,
			unsigned int payload_length, bool skip)
{
	struct fw_iso_packet p = {0};
	int err = 0;

	if (IS_ERR(s->context))
		goto end;

	p.interrupt = IS_ALIGNED(s->packet_index + 1, INTERRUPT_INTERVAL);
	p.tag = TAG_CIP;
	p.header_length = header_length;
	p.payload_length = (!skip) ? payload_length : 0;
	p.skip = skip;
	err = fw_iso_context_queue(s->context, &p, &s->buffer.iso_buffer,
				   s->buffer.packets[s->packet_index].offset);
	if (err < 0) {
		dev_err(&s->unit->device, "queueing error: %d\n", err);
		goto end;
	}

	if (++s->packet_index >= QUEUE_LENGTH)
		s->packet_index = 0;
end:
	return err;
}

static inline int queue_out_packet(struct amdtp_stream *s,
				   unsigned int payload_length, bool skip)
{
	return queue_packet(s, OUT_PACKET_HEADER_SIZE,
			    payload_length, skip);
}

static inline int queue_in_packet(struct amdtp_stream *s)
{
	return queue_packet(s, IN_PACKET_HEADER_SIZE,
			    amdtp_stream_get_max_payload(s), false);
}

static void handle_out_packet(struct amdtp_stream *s, unsigned int syt)
{
	__be32 *buffer;
	unsigned int data_blocks, payload_length;
	struct snd_pcm_substream *pcm;

	if (s->packet_index < 0)
		return;

	/* this module generate empty packet for 'no data' */
	if (!(s->flags & CIP_BLOCKING) || (syt != CIP_SYT_NO_INFO))
		data_blocks = calculate_data_blocks(s);
	else
		data_blocks = 0;

	buffer = s->buffer.packets[s->packet_index].buffer;
	buffer[0] = cpu_to_be32(ACCESS_ONCE(s->source_node_id_field) |
				(s->data_block_quadlets << AMDTP_DBS_SHIFT) |
				s->data_block_counter);
	buffer[1] = cpu_to_be32(CIP_EOH | CIP_FMT_AM | AMDTP_FDF_AM824 |
				(s->sfc << CIP_FDF_SFC_SHIFT) | syt);
>>>>>>> v3.18
	buffer += 2;

	pcm = ACCESS_ONCE(s->pcm);
	if (pcm)
		s->transfer_samples(s, pcm, buffer, data_blocks);
	else
		amdtp_fill_pcm_silence(s, buffer, data_blocks);
	if (s->midi_ports)
		amdtp_fill_midi(s, buffer, data_blocks);

	s->data_block_counter = (s->data_block_counter + data_blocks) & 0xff;

<<<<<<< HEAD
	packet.payload_length = 8 + data_blocks * 4 * s->data_block_quadlets;
	packet.interrupt = IS_ALIGNED(index + 1, INTERRUPT_INTERVAL);
	packet.skip = 0;
	packet.tag = TAG_CIP;
	packet.sy = 0;
	packet.header_length = 0;

	err = fw_iso_context_queue(s->context, &packet, &s->buffer.iso_buffer,
				   s->buffer.packets[index].offset);
	if (err < 0) {
		dev_err(&s->unit->device, "queueing error: %d\n", err);
		s->packet_index = -1;
		amdtp_out_stream_pcm_abort(s);
		return;
	}

	if (++index >= QUEUE_LENGTH)
		index = 0;
	s->packet_index = index;

	if (pcm) {
		ptr = s->pcm_buffer_pointer + data_blocks;
		if (ptr >= pcm->runtime->buffer_size)
			ptr -= pcm->runtime->buffer_size;
		ACCESS_ONCE(s->pcm_buffer_pointer) = ptr;

		s->pcm_period_pointer += data_blocks;
		if (s->pcm_period_pointer >= pcm->runtime->period_size) {
			s->pcm_period_pointer -= pcm->runtime->period_size;
			s->pointer_flush = false;
			tasklet_hi_schedule(&s->period_tasklet);
		}
	}
}

static void pcm_period_tasklet(unsigned long data)
{
	struct amdtp_out_stream *s = (void *)data;
	struct snd_pcm_substream *pcm = ACCESS_ONCE(s->pcm);

	if (pcm)
		snd_pcm_period_elapsed(pcm);
}

static void out_packet_callback(struct fw_iso_context *context, u32 cycle,
				size_t header_length, void *header, void *data)
{
	struct amdtp_out_stream *s = data;
	unsigned int i, packets = header_length / 4;
=======
	payload_length = 8 + data_blocks * 4 * s->data_block_quadlets;
	if (queue_out_packet(s, payload_length, false) < 0) {
		s->packet_index = -1;
		amdtp_stream_pcm_abort(s);
		return;
	}

	if (pcm)
		update_pcm_pointers(s, pcm, data_blocks);
}

static void handle_in_packet(struct amdtp_stream *s,
			     unsigned int payload_quadlets,
			     __be32 *buffer)
{
	u32 cip_header[2];
	unsigned int data_blocks, data_block_quadlets, data_block_counter,
		     dbc_interval;
	struct snd_pcm_substream *pcm = NULL;
	bool lost;

	cip_header[0] = be32_to_cpu(buffer[0]);
	cip_header[1] = be32_to_cpu(buffer[1]);

	/*
	 * This module supports 'Two-quadlet CIP header with SYT field'.
	 * For convenience, also check FMT field is AM824 or not.
	 */
	if (((cip_header[0] & CIP_EOH_MASK) == CIP_EOH) ||
	    ((cip_header[1] & CIP_EOH_MASK) != CIP_EOH) ||
	    ((cip_header[1] & CIP_FMT_MASK) != CIP_FMT_AM)) {
		dev_info_ratelimited(&s->unit->device,
				"Invalid CIP header for AMDTP: %08X:%08X\n",
				cip_header[0], cip_header[1]);
		goto end;
	}

	/* Calculate data blocks */
	if (payload_quadlets < 3 ||
	    ((cip_header[1] & CIP_FDF_MASK) ==
				(AMDTP_FDF_NO_DATA << CIP_FDF_SFC_SHIFT))) {
		data_blocks = 0;
	} else {
		data_block_quadlets =
			(cip_header[0] & AMDTP_DBS_MASK) >> AMDTP_DBS_SHIFT;
		/* avoid division by zero */
		if (data_block_quadlets == 0) {
			dev_info_ratelimited(&s->unit->device,
				"Detect invalid value in dbs field: %08X\n",
				cip_header[0]);
			goto err;
		}
		if (s->flags & CIP_WRONG_DBS)
			data_block_quadlets = s->data_block_quadlets;

		data_blocks = (payload_quadlets - 2) / data_block_quadlets;
	}

	/* Check data block counter continuity */
	data_block_counter = cip_header[0] & AMDTP_DBC_MASK;
	if (data_blocks == 0 && (s->flags & CIP_EMPTY_HAS_WRONG_DBC) &&
	    s->data_block_counter != UINT_MAX)
		data_block_counter = s->data_block_counter;

	if (((s->flags & CIP_SKIP_DBC_ZERO_CHECK) && data_block_counter == 0) ||
	    (s->data_block_counter == UINT_MAX)) {
		lost = false;
	} else if (!(s->flags & CIP_DBC_IS_END_EVENT)) {
		lost = data_block_counter != s->data_block_counter;
	} else {
		if ((data_blocks > 0) && (s->tx_dbc_interval > 0))
			dbc_interval = s->tx_dbc_interval;
		else
			dbc_interval = data_blocks;

		lost = data_block_counter !=
		       ((s->data_block_counter + dbc_interval) & 0xff);
	}

	if (lost) {
		dev_info(&s->unit->device,
			 "Detect discontinuity of CIP: %02X %02X\n",
			 s->data_block_counter, data_block_counter);
		goto err;
	}

	if (data_blocks > 0) {
		buffer += 2;

		pcm = ACCESS_ONCE(s->pcm);
		if (pcm)
			s->transfer_samples(s, pcm, buffer, data_blocks);

		if (s->midi_ports)
			amdtp_pull_midi(s, buffer, data_blocks);
	}

	if (s->flags & CIP_DBC_IS_END_EVENT)
		s->data_block_counter = data_block_counter;
	else
		s->data_block_counter =
				(data_block_counter + data_blocks) & 0xff;
end:
	if (queue_in_packet(s) < 0)
		goto err;

	if (pcm)
		update_pcm_pointers(s, pcm, data_blocks);

	return;
err:
	s->packet_index = -1;
	amdtp_stream_pcm_abort(s);
}

static void out_stream_callback(struct fw_iso_context *context, u32 cycle,
				size_t header_length, void *header,
				void *private_data)
{
	struct amdtp_stream *s = private_data;
	unsigned int i, syt, packets = header_length / 4;
>>>>>>> v3.18

	/*
	 * Compute the cycle of the last queued packet.
	 * (We need only the four lowest bits for the SYT, so we can ignore
	 * that bits 0-11 must wrap around at 3072.)
	 */
	cycle += QUEUE_LENGTH - packets;

<<<<<<< HEAD
	for (i = 0; i < packets; ++i)
		queue_out_packet(s, ++cycle);
	fw_iso_context_queue_flush(s->context);
}

static int queue_initial_skip_packets(struct amdtp_out_stream *s)
{
	struct fw_iso_packet skip_packet = {
		.skip = 1,
	};
	unsigned int i;
	int err;

	for (i = 0; i < QUEUE_LENGTH; ++i) {
		skip_packet.interrupt = IS_ALIGNED(s->packet_index + 1,
						   INTERRUPT_INTERVAL);
		err = fw_iso_context_queue(s->context, &skip_packet, NULL, 0);
		if (err < 0)
			return err;
		if (++s->packet_index >= QUEUE_LENGTH)
			s->packet_index = 0;
	}

	return 0;
}

/**
 * amdtp_out_stream_start - start sending packets
 * @s: the AMDTP output stream to start
=======
	for (i = 0; i < packets; ++i) {
		syt = calculate_syt(s, ++cycle);
		handle_out_packet(s, syt);
	}
	fw_iso_context_queue_flush(s->context);
}

static void in_stream_callback(struct fw_iso_context *context, u32 cycle,
			       size_t header_length, void *header,
			       void *private_data)
{
	struct amdtp_stream *s = private_data;
	unsigned int p, syt, packets, payload_quadlets;
	__be32 *buffer, *headers = header;

	/* The number of packets in buffer */
	packets = header_length / IN_PACKET_HEADER_SIZE;

	for (p = 0; p < packets; p++) {
		if (s->packet_index < 0)
			break;

		buffer = s->buffer.packets[s->packet_index].buffer;

		/* Process sync slave stream */
		if (s->sync_slave && s->sync_slave->callbacked) {
			syt = be32_to_cpu(buffer[1]) & CIP_SYT_MASK;
			handle_out_packet(s->sync_slave, syt);
		}

		/* The number of quadlets in this packet */
		payload_quadlets =
			(be32_to_cpu(headers[p]) >> ISO_DATA_LENGTH_SHIFT) / 4;
		handle_in_packet(s, payload_quadlets, buffer);
	}

	/* Queueing error or detecting discontinuity */
	if (s->packet_index < 0) {
		/* Abort sync slave. */
		if (s->sync_slave) {
			s->sync_slave->packet_index = -1;
			amdtp_stream_pcm_abort(s->sync_slave);
		}
		return;
	}

	/* when sync to device, flush the packets for slave stream */
	if (s->sync_slave && s->sync_slave->callbacked)
		fw_iso_context_queue_flush(s->sync_slave->context);

	fw_iso_context_queue_flush(s->context);
}

/* processing is done by master callback */
static void slave_stream_callback(struct fw_iso_context *context, u32 cycle,
				  size_t header_length, void *header,
				  void *private_data)
{
	return;
}

/* this is executed one time */
static void amdtp_stream_first_callback(struct fw_iso_context *context,
					u32 cycle, size_t header_length,
					void *header, void *private_data)
{
	struct amdtp_stream *s = private_data;

	/*
	 * For in-stream, first packet has come.
	 * For out-stream, prepared to transmit first packet
	 */
	s->callbacked = true;
	wake_up(&s->callback_wait);

	if (s->direction == AMDTP_IN_STREAM)
		context->callback.sc = in_stream_callback;
	else if ((s->flags & CIP_BLOCKING) && (s->flags & CIP_SYNC_TO_DEVICE))
		context->callback.sc = slave_stream_callback;
	else
		context->callback.sc = out_stream_callback;

	context->callback.sc(context, cycle, header_length, header, s);
}

/**
 * amdtp_stream_start - start transferring packets
 * @s: the AMDTP stream to start
>>>>>>> v3.18
 * @channel: the isochronous channel on the bus
 * @speed: firewire speed code
 *
 * The stream cannot be started until it has been configured with
<<<<<<< HEAD
 * amdtp_out_stream_set_hw_params(), amdtp_out_stream_set_pcm(), and
 * amdtp_out_stream_set_midi(); and it must be started before any
 * PCM or MIDI device can be started.
 */
int amdtp_out_stream_start(struct amdtp_out_stream *s, int channel, int speed)
=======
 * amdtp_stream_set_parameters() and it must be started before any PCM or MIDI
 * device can be started.
 */
int amdtp_stream_start(struct amdtp_stream *s, int channel, int speed)
>>>>>>> v3.18
{
	static const struct {
		unsigned int data_block;
		unsigned int syt_offset;
	} initial_state[] = {
		[CIP_SFC_32000]  = {  4, 3072 },
		[CIP_SFC_48000]  = {  6, 1024 },
		[CIP_SFC_96000]  = { 12, 1024 },
		[CIP_SFC_192000] = { 24, 1024 },
		[CIP_SFC_44100]  = {  0,   67 },
		[CIP_SFC_88200]  = {  0,   67 },
		[CIP_SFC_176400] = {  0,   67 },
	};
<<<<<<< HEAD
	int err;

	mutex_lock(&s->mutex);

	if (WARN_ON(!IS_ERR(s->context) ||
		    (!s->pcm_channels && !s->midi_ports))) {
=======
	unsigned int header_size;
	enum dma_data_direction dir;
	int type, tag, err;

	mutex_lock(&s->mutex);

	if (WARN_ON(amdtp_stream_running(s) ||
		    (s->data_block_quadlets < 1))) {
>>>>>>> v3.18
		err = -EBADFD;
		goto err_unlock;
	}

<<<<<<< HEAD
=======
	if (s->direction == AMDTP_IN_STREAM &&
	    s->flags & CIP_SKIP_INIT_DBC_CHECK)
		s->data_block_counter = UINT_MAX;
	else
		s->data_block_counter = 0;
>>>>>>> v3.18
	s->data_block_state = initial_state[s->sfc].data_block;
	s->syt_offset_state = initial_state[s->sfc].syt_offset;
	s->last_syt_offset = TICKS_PER_CYCLE;

<<<<<<< HEAD
	err = iso_packets_buffer_init(&s->buffer, s->unit, QUEUE_LENGTH,
				      amdtp_out_stream_get_max_payload(s),
				      DMA_TO_DEVICE);
=======
	/* initialize packet buffer */
	if (s->direction == AMDTP_IN_STREAM) {
		dir = DMA_FROM_DEVICE;
		type = FW_ISO_CONTEXT_RECEIVE;
		header_size = IN_PACKET_HEADER_SIZE;
	} else {
		dir = DMA_TO_DEVICE;
		type = FW_ISO_CONTEXT_TRANSMIT;
		header_size = OUT_PACKET_HEADER_SIZE;
	}
	err = iso_packets_buffer_init(&s->buffer, s->unit, QUEUE_LENGTH,
				      amdtp_stream_get_max_payload(s), dir);
>>>>>>> v3.18
	if (err < 0)
		goto err_unlock;

	s->context = fw_iso_context_create(fw_parent_device(s->unit)->card,
<<<<<<< HEAD
					   FW_ISO_CONTEXT_TRANSMIT,
					   channel, speed, 0,
					   out_packet_callback, s);
=======
					   type, channel, speed, header_size,
					   amdtp_stream_first_callback, s);
>>>>>>> v3.18
	if (IS_ERR(s->context)) {
		err = PTR_ERR(s->context);
		if (err == -EBUSY)
			dev_err(&s->unit->device,
<<<<<<< HEAD
				"no free output stream on this controller\n");
		goto err_buffer;
	}

	amdtp_out_stream_update(s);

	s->packet_index = 0;
	s->data_block_counter = 0;
	err = queue_initial_skip_packets(s);
	if (err < 0)
		goto err_context;

	err = fw_iso_context_start(s->context, -1, 0, 0);
=======
				"no free stream on this controller\n");
		goto err_buffer;
	}

	amdtp_stream_update(s);

	s->packet_index = 0;
	do {
		if (s->direction == AMDTP_IN_STREAM)
			err = queue_in_packet(s);
		else
			err = queue_out_packet(s, 0, true);
		if (err < 0)
			goto err_context;
	} while (s->packet_index > 0);

	/* NOTE: TAG1 matches CIP. This just affects in stream. */
	tag = FW_ISO_CONTEXT_MATCH_TAG1;
	if (s->flags & CIP_EMPTY_WITH_TAG0)
		tag |= FW_ISO_CONTEXT_MATCH_TAG0;

	s->callbacked = false;
	err = fw_iso_context_start(s->context, -1, 0, tag);
>>>>>>> v3.18
	if (err < 0)
		goto err_context;

	mutex_unlock(&s->mutex);

	return 0;

err_context:
	fw_iso_context_destroy(s->context);
	s->context = ERR_PTR(-1);
err_buffer:
	iso_packets_buffer_destroy(&s->buffer, s->unit);
err_unlock:
	mutex_unlock(&s->mutex);

	return err;
}
<<<<<<< HEAD
EXPORT_SYMBOL(amdtp_out_stream_start);

/**
 * amdtp_out_stream_pcm_pointer - get the PCM buffer position
 * @s: the AMDTP output stream that transports the PCM data
 *
 * Returns the current buffer position, in frames.
 */
unsigned long amdtp_out_stream_pcm_pointer(struct amdtp_out_stream *s)
{
	/* this optimization is allowed to be racy */
	if (s->pointer_flush)
=======
EXPORT_SYMBOL(amdtp_stream_start);

/**
 * amdtp_stream_pcm_pointer - get the PCM buffer position
 * @s: the AMDTP stream that transports the PCM data
 *
 * Returns the current buffer position, in frames.
 */
unsigned long amdtp_stream_pcm_pointer(struct amdtp_stream *s)
{
	/* this optimization is allowed to be racy */
	if (s->pointer_flush && amdtp_stream_running(s))
>>>>>>> v3.18
		fw_iso_context_flush_completions(s->context);
	else
		s->pointer_flush = true;

	return ACCESS_ONCE(s->pcm_buffer_pointer);
}
<<<<<<< HEAD
EXPORT_SYMBOL(amdtp_out_stream_pcm_pointer);

/**
 * amdtp_out_stream_update - update the stream after a bus reset
 * @s: the AMDTP output stream
 */
void amdtp_out_stream_update(struct amdtp_out_stream *s)
=======
EXPORT_SYMBOL(amdtp_stream_pcm_pointer);

/**
 * amdtp_stream_update - update the stream after a bus reset
 * @s: the AMDTP stream
 */
void amdtp_stream_update(struct amdtp_stream *s)
>>>>>>> v3.18
{
	ACCESS_ONCE(s->source_node_id_field) =
		(fw_parent_device(s->unit)->card->node_id & 0x3f) << 24;
}
<<<<<<< HEAD
EXPORT_SYMBOL(amdtp_out_stream_update);

/**
 * amdtp_out_stream_stop - stop sending packets
 * @s: the AMDTP output stream to stop
=======
EXPORT_SYMBOL(amdtp_stream_update);

/**
 * amdtp_stream_stop - stop sending packets
 * @s: the AMDTP stream to stop
>>>>>>> v3.18
 *
 * All PCM and MIDI devices of the stream must be stopped before the stream
 * itself can be stopped.
 */
<<<<<<< HEAD
void amdtp_out_stream_stop(struct amdtp_out_stream *s)
{
	mutex_lock(&s->mutex);

	if (IS_ERR(s->context)) {
=======
void amdtp_stream_stop(struct amdtp_stream *s)
{
	mutex_lock(&s->mutex);

	if (!amdtp_stream_running(s)) {
>>>>>>> v3.18
		mutex_unlock(&s->mutex);
		return;
	}

	tasklet_kill(&s->period_tasklet);
	fw_iso_context_stop(s->context);
	fw_iso_context_destroy(s->context);
	s->context = ERR_PTR(-1);
	iso_packets_buffer_destroy(&s->buffer, s->unit);

<<<<<<< HEAD
	mutex_unlock(&s->mutex);
}
EXPORT_SYMBOL(amdtp_out_stream_stop);

/**
 * amdtp_out_stream_pcm_abort - abort the running PCM device
=======
	s->callbacked = false;

	mutex_unlock(&s->mutex);
}
EXPORT_SYMBOL(amdtp_stream_stop);

/**
 * amdtp_stream_pcm_abort - abort the running PCM device
>>>>>>> v3.18
 * @s: the AMDTP stream about to be stopped
 *
 * If the isochronous stream needs to be stopped asynchronously, call this
 * function first to stop the PCM device.
 */
<<<<<<< HEAD
void amdtp_out_stream_pcm_abort(struct amdtp_out_stream *s)
=======
void amdtp_stream_pcm_abort(struct amdtp_stream *s)
>>>>>>> v3.18
{
	struct snd_pcm_substream *pcm;

	pcm = ACCESS_ONCE(s->pcm);
	if (pcm) {
		snd_pcm_stream_lock_irq(pcm);
		if (snd_pcm_running(pcm))
			snd_pcm_stop(pcm, SNDRV_PCM_STATE_XRUN);
		snd_pcm_stream_unlock_irq(pcm);
	}
}
<<<<<<< HEAD
EXPORT_SYMBOL(amdtp_out_stream_pcm_abort);
=======
EXPORT_SYMBOL(amdtp_stream_pcm_abort);
>>>>>>> v3.18
