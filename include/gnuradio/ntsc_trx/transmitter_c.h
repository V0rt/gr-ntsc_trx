/* -*- c++ -*- */
/*
 * Copyright 2023 gr-ntsc_trx author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NTSC_TRX_TRANSMITTER_C_H
#define INCLUDED_NTSC_TRX_TRANSMITTER_C_H

#include <gnuradio/ntsc_trx/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ntsc_trx {

    /*!
     * \brief <+description of block+>
     * \ingroup ntsc_trx
     *
     */
    class NTSC_TRX_API transmitter_c : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<transmitter_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ntsc_trx::transmitter_c.
       *
       * To avoid accidental use of raw pointers, ntsc_trx::transmitter_c's
       * constructor is in a private implementation
       * class. ntsc_trx::transmitter_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(float samp_rate);
    };

  } // namespace ntsc_trx
} // namespace gr

#endif /* INCLUDED_NTSC_TRX_TRANSMITTER_C_H */
