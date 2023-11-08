/* -*- c++ -*- */
/*
 * Copyright 2023 gr-ntsc_trx author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "video_stream_converter_c_impl.h"

namespace gr {
  namespace ntsc_trx {

    video_stream_converter_c::sptr
    video_stream_converter_c::make(float samp_rate, float decimation_factor)
    {
      return gnuradio::make_block_sptr<video_stream_converter_c_impl>(samp_rate, decimation_factor);
    }


    /*
     * The private constructor
     */
    video_stream_converter_c_impl::video_stream_converter_c_impl(float samp_rate, float decimation_factor)
      : gr::sync_decimator("video_stream_converter_c",
              gr::io_signature::make(4, 4, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(short)), decimation_factor)
    {
      /*** INIT ***/
      d_decimation_factor = decimation_factor;
      d_x_in = 0;
      d_y_in = 0;
      d_bw_in = 0;
      d_x_out = 0;
      d_y_out = 0;
      for(int i = 0; i < X_WIDTH; i++){
          for(int j = 0; j < Y_HEIGHT; j++)
              d_img_mat[j][i] = 127;
      }
      printf("Welcome in NTSC Video Stream Converter C++\n");
    }

    /*
     * Our virtual destructor.
     */
    video_stream_converter_c_impl::~video_stream_converter_c_impl()
    {
    }

    int
    video_stream_converter_c_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {



      /*-----------------------SIGNAL PROCESSING -------------------*/
      const float *in = (const float *) input_items[0];
      const float *in1 = (const float *) input_items[1];
      const float *in2 = (const float *) input_items[2];
      const float *in3 = (const float *) input_items[3];
      short *out = (short *) output_items[0];



      /*------------------------ INPUTS-------------------------*/
      for(int i = 0; i < d_decimation_factor * noutput_items; i++){

          // Process only on Video Transmission
          if(in[i] == VIDEO){

              // Fetch and Control X, Y, BW
              d_x_in = int(in1[i]);
              d_y_in = int(in2[i]);
              d_bw_in = int(in3[i]);
              if(d_bw_in > 250) d_bw_in = 250;
              if(d_bw_in < 5) d_bw_in = 5;

              // Inputs update image matrix
              if(d_x_in < X_WIDTH - 2 && d_x_in > 2  && d_y_in < Y_HEIGHT - 2 && d_y_in > 2 ){
                  d_img_mat[d_y_in][d_x_in] = d_bw_in;
              }
          }
      }



      /*------------------------ OUTPUT -----------------------*/
      for(int i = 0; i < noutput_items; i++){

          // Update position
          d_x_out++;
          if(d_x_out == X_WIDTH){
              d_x_out = 0;
              d_y_out++;
          }
          if(d_y_out == Y_HEIGHT){
              d_y_out = 0;
          }

          // Output correct pixel b&w level
          if(d_x_out < X_WIDTH && d_x_out > -1 && d_y_out < Y_HEIGHT && d_y_out > -1 )
              out[i] = d_img_mat[d_y_out][d_x_out];
      }



      return noutput_items;
    }

  } /* namespace ntsc_trx */
} /* namespace gr */
