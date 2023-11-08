/* -*- c++ -*- */
/*
 * Copyright 2023 gr-ntsc_trx author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NTSC_TRX_TRANSMITTER_C_IMPL_H
#define INCLUDED_NTSC_TRX_TRANSMITTER_C_IMPL_H

#include <gnuradio/ntsc_trx/transmitter_c.h>
#include "NTSC_configuration.h"
#include "image_matrix.h"

namespace gr {
  namespace ntsc_trx {

    class transmitter_c_impl : public transmitter_c
    {
     private:
         float d_samp_rate;
         int d_samples_cnt;
         int d_meta_state;
         int d_sub_state;
         float d_lines_cnt;
         int d_frame_parity;



     public:
      transmitter_c_impl(float samp_rate);
      ~transmitter_c_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace ntsc_trx
} // namespace gr

#endif /* INCLUDED_NTSC_TRX_TRANSMITTER_C_IMPL_H */
