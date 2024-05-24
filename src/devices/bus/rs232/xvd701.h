// license:BSD-3-Clause
// copyright-holders:smf
#ifndef MAME_BUS_RS232_XVD701_H
#define MAME_BUS_RS232_XVD701_H

#include "diserial.h"
#include "rs232.h"
#include "screen.h"

#include "video/pl_mpeg.h"

class jvc_xvd701_device : public device_t,
		public device_serial_interface,
		public device_rs232_port_interface
{
public:
	enum jvc_xvd701_media_type : uint32_t
	{
		JVC_MEDIA_VCD = 0,
		JVC_MEDIA_DVD = 1,
	};

	enum jvc_xvd701_playback_status : uint32_t
	{
		STATUS_STOP = 0,
		STATUS_PLAYING = 1,
		STATUS_PAUSE = 2,
	};

	jvc_xvd701_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual void input_txd(int state) override { device_serial_interface::rx_w(state); }

	void set_video_surface(bitmap_rgb32 *video_surface)
	{
		m_video_bitmap = video_surface;
	}

	void set_data_folder(const char *data_folder)
	{
		m_data_folder = data_folder;
	}

	void set_media_type(const jvc_xvd701_media_type media_type)
	{
		m_media_type = media_type;
	}

	void decode_next_frame(double elapsed_time);

	plm_t *m_plm;
	uint8_t *m_rgb_data;
	bitmap_rgb32 *m_video_bitmap;
	const char *m_data_folder;

protected:
	virtual ioport_constructor device_input_ports() const override;
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_add_mconfig(machine_config &config) override;

	virtual void tra_callback() override;
	virtual void tra_complete() override;
	virtual void rcv_complete() override;

private:
	TIMER_CALLBACK_MEMBER(send_response);

	unsigned char sum(unsigned char *buffer, int length);
	void create_packet(unsigned char status, const unsigned char response[6]);

	bool seek_chapter(int chapter);

	jvc_xvd701_media_type m_media_type;

	unsigned char m_command[11];
	unsigned char m_response[11];
	int m_response_index;
	emu_timer *m_timer_response;

	jvc_xvd701_playback_status m_playback_status;

	unsigned char m_jlip_id;
	bool m_is_powered;

	int m_chapter;
	double m_wait_timer;

	enum : unsigned char {
		STATUS_UNKNOWN_COMMAND = 1,
		STATUS_OK = 3,
		STATUS_ERROR = 5,
	};
	const unsigned char NO_RESPONSE[6] = { 0 };
};

DECLARE_DEVICE_TYPE(JVC_XVD701, jvc_xvd701_device)

#endif // MAME_BUS_RS232_XVD701_H
