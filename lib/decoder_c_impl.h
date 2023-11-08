/* -*- c++ -*- */
/*
 * Copyright 2023 gr-ntsc_trx author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NTSC_TRX_DECODER_C_IMPL_H
#define INCLUDED_NTSC_TRX_DECODER_C_IMPL_H

#include <gnuradio/ntsc_trx/decoder_c.h>

namespace gr {
  namespace ntsc_trx {

    class decoder_c_impl : public decoder_c
    {
     private:
         float d_samp_rate;
         float d_samples_cnt;
         float d_lines_cnt;
         int d_state;


     public:
      decoder_c_impl(float samp_rate);
      ~decoder_c_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace ntsc_trx
} // namespace gr

#endif /* INCLUDED_NTSC_TRX_DECODER_C_IMPL_H */
