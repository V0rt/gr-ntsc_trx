/* -*- c++ -*- */
/*
 * Copyright 2023 gr-ntsc_trx author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "decoder_c_impl.h"
#include "NTSC_configuration.h"

namespace gr {
  namespace ntsc_trx {
    decoder_c::sptr
    decoder_c::make(float samp_rate)
    {
      return gnuradio::make_block_sptr<decoder_c_impl>(
        samp_rate);
    }


    /*
     * The private constructor
     */
    decoder_c_impl::decoder_c_impl(float samp_rate)
      : gr::sync_block("decoder_c",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(4, 4, sizeof(float)))
    {
      /*** INIT ***/
      d_samp_rate = samp_rate;
      d_state = IDLE;
      d_samples_cnt = 0;
      d_lines_cnt = 0;
      printf("Welcome in NTSC Decoder C++\n");
    }

    /*
     * Our virtual destructor.
     */
    decoder_c_impl::~decoder_c_impl()
    {
    }

    int
    decoder_c_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {



      /*------------------------- SIGNAL PROCESSING ---------------------*/
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      float *out1 = (float *) output_items[1];
      float *out2 = (float *) output_items[2];
      float *out3 = (float *) output_items[3];

      for(int i = 0; i < noutput_items; i++){
          d_samples_cnt++;
          out1[i] = -1; // -1 = defined if d_state = VIDEO
          out2[i] = -1;
          out3[i] = -1;



          /*------------------------- STATE MACHINE ---------------------*/

          /*** IDLE ***/
          if(d_state == IDLE){
              out[i] =  IDLE;

              // Level of Horizontal Sync
              if(in[i-1] > HORIZONTAL_SYNC_THRESHOLD and in[i] < HORIZONTAL_SYNC_THRESHOLD){
                  d_state = HORIZONTAL_SYNC;
                  d_samples_cnt = 0;
              }
          }


          /*** HORIZONTAL SYNCHRONISATION ***/
          if(d_state == HORIZONTAL_SYNC){
              out[i] =  HORIZONTAL_SYNC ;

              // Time for Back Porch
              if(d_samples_cnt > HORIZONTAL_SYNC_DURATION * d_samp_rate){
                  d_state = BACK_PORCH;
                  d_samples_cnt = 0;
              }
          }


          /*** BACK PORCH ***/
          if(d_state == BACK_PORCH){
              out[i] =  BACK_PORCH;

              // Time for Video
              if(d_samples_cnt > BACK_PORCH_DURATION * d_samp_rate){
                  d_state = VIDEO;
                  d_samples_cnt = 0;
              }
          }


          /*** VIDEO ***/
          if(d_state == VIDEO){
              out[i] = VIDEO;

              // Set Pixel Output = (X, Y, BW)
              out1[i] = int(X_WIDTH * d_samples_cnt / (VIDEO_DURATION * d_samp_rate));
              out2[i] = int(Y_HEIGHT * d_lines_cnt / (1.0 * NBR_VIDEO_LINES));
              out3[i] = int((in[i] - BLACK_LEVEL) / (WHITE_LEVEL - BLACK_LEVEL) * 254);

              // Time for Front Porch
              if(d_samples_cnt > VIDEO_DURATION * d_samp_rate){
                  d_state = FRONT_PORCH;
                  d_samples_cnt = 0;
                  d_lines_cnt++;
              }

              // Level and Time for Vertical Sync
              if(in[i-1] > HORIZONTAL_SYNC_THRESHOLD  && in[i] < HORIZONTAL_SYNC_THRESHOLD && d_samples_cnt < 0.75 * VIDEO_DURATION * d_samp_rate){
                  d_state = VERTICAL_SYNC;
                  d_samples_cnt = 0;
              }
          }


          /*** FRONT PORCH ***/
          if(d_state == FRONT_PORCH){
              out[i] =  FRONT_PORCH;

              // Level of Sync Tip
              if(in[i-1] > HORIZONTAL_SYNC_THRESHOLD and in[i] < HORIZONTAL_SYNC_THRESHOLD){
                  d_state = HORIZONTAL_SYNC;
                  d_samples_cnt = 0;
              }

              // Time for horizontal sync
              if(d_samples_cnt > FRONT_PORCH_DURATION * d_samp_rate){
                  d_state = HORIZONTAL_SYNC;
                  d_samples_cnt = 0;
              }
          }


          /*** VERTICAL SYNCHRONISATION LINES ***/
          if(d_state == VERTICAL_SYNC){
              out[i] =  VERTICAL_SYNC ;

              // Ignore the Vertical Sync lines
              if(d_samples_cnt > (NBR_VERTICAL_SYNC_LINES + 1.5) * LINE_DURATION * d_samp_rate){
                  d_lines_cnt = 0;
                  d_state = IDLE;
              }
          }
      }
      return noutput_items;
    }

  } /* namespace ntsc_trx */
} /* namespace gr */
