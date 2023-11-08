/* -*- c++ -*- */
/*
 * Copyright 2023 gr-ntsc_trx author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NTSC_TRX_VIDEO_STREAM_CONVERTER_C_H
#define INCLUDED_NTSC_TRX_VIDEO_STREAM_CONVERTER_C_H

#include <gnuradio/ntsc_trx/api.h>
#include <gnuradio/sync_decimator.h>

namespace gr {
  namespace ntsc_trx {

    /*!
     * \brief <+description of block+>
     * \ingroup ntsc_trx
     *
     */
    class NTSC_TRX_API video_stream_converter_c : virtual public gr::sync_decimator
    {
     public:
      typedef std::shared_ptr<video_stream_converter_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ntsc_trx::video_stream_converter_c.
       *
       * To avoid accidental use of raw pointers, ntsc_trx::video_stream_converter_c's
       * constructor is in a private implementation
       * class. ntsc_trx::video_stream_converter_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(float samp_rate, float decimation_factor);
    };

  } // namespace ntsc_trx
} // namespace gr

#endif /* INCLUDED_NTSC_TRX_VIDEO_STREAM_CONVERTER_C_H */
