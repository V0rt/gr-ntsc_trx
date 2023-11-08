/* -*- c++ -*- */
/*
 * Copyright 2023 gr-ntsc_trx author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "transmitter_c_impl.h"

namespace gr {
  namespace ntsc_trx {

    transmitter_c::sptr
    transmitter_c::make(float samp_rate)
    {
      return gnuradio::make_block_sptr<transmitter_c_impl>(
        samp_rate);
    }


    /*
     * The private constructor
     */
    transmitter_c_impl::transmitter_c_impl(float samp_rate)
      : gr::sync_block("transmitter_c",
              gr::io_signature::make(0,0, 0),
              gr::io_signature::make(1, 1 , sizeof(float)))
    {
      /*** INIT ***/
      d_samp_rate = samp_rate;
      d_meta_state = VERTICAL_SYNC;
      d_sub_state = EQUALISING;
      d_frame_parity = EVEN;
      d_samples_cnt = 0;
      d_lines_cnt = 0;
      printf("Welcome in NTSC Transmitter C++\n");

//      std::fill(&img_mat[0][0], &img_mat[0][0] + 360*120, 0);
//      std::fill(&img_mat[0][0] + 360*120, &img_mat[0][0] + 360*240, 1);

//      FILE* fd = fopen("/mnt/ramdisk/file", "rb");
//      fseek(fd,0, SEEK_END);
//      int num_items = ftell(fd) / sizeof(float);
//      float arr[num_items];
//      fseek(fd, 0,SEEK_SET);
//      fread(&arr[0], sizeof(float), num_items, fd);
//      fclose(fd);
    }

    /*
     * Our virtual destructor.
     */
    transmitter_c_impl::~transmitter_c_impl()
    {
    }

    int
    transmitter_c_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {



      /*------------------ SIGNAL PROCESSING--------------------*/
//      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      for(int i=0; i < noutput_items; i++){
          d_samples_cnt++;



          /*------------------- META STATE MACHINE -----------------*/
          if(d_meta_state == LINES_TRANSMISSION){


              /*---------------------- SUB STATE MACHINE -----------------*/

              /*** HORIZONTAL SYNCHRONISATION ***/
              if(d_sub_state == HORIZONTAL_SYNC){
                  out[i] = HORIZONTAL_SYNC_LEVEL ;

                  // Time for Back Porch
                  if(d_samples_cnt > HORIZONTAL_SYNC_DURATION * d_samp_rate){
                      d_sub_state = BACK_PORCH;
                      d_samples_cnt = 0;
                  }
              }


              /*** BACK PORCH ***/
              if(d_sub_state == BACK_PORCH){
                  out[i] = BACK_PORCH_LEVEL;

                  // Time for Active Video
                  if(d_samples_cnt > BACK_PORCH_DURATION * d_samp_rate){
                      d_sub_state = VIDEO;
                      d_samples_cnt = 0;
                  }
              }


              /*** ACTIVE VIDEO ***/
              if(d_sub_state == VIDEO){
                  // Transmit pixel's black and white level from img_mat in image_matrix.h
                  out[i] = BLACK_LEVEL + (WHITE_LEVEL - BLACK_LEVEL) * img_mat[int(d_lines_cnt )][int(360 * d_samples_cnt / (d_samp_rate * VIDEO_DURATION))] ;

                  // Time for Front Porch
                  if(d_samples_cnt > VIDEO_DURATION * d_samp_rate){
                      d_sub_state = FRONT_PORCH;
                      d_samples_cnt = 0;
                  }
              }


              /*** FRONT PORCH ***/
              if(d_sub_state == FRONT_PORCH){
                  out[i] = FRONT_PORCH_LEVEL;

                  // Time for Horizontal Sync
                  if(d_samples_cnt > FRONT_PORCH_DURATION * d_samp_rate){
                      d_sub_state = HORIZONTAL_SYNC;
                      d_samples_cnt = 0;
                      d_lines_cnt++;
                  }
              }

              // Exit to  Vertical Sync
              if(d_lines_cnt == NBR_VIDEO_LINES){
                  d_meta_state = VERTICAL_SYNC;
                  d_sub_state = EQUALISING;
                  d_samples_cnt = 0;
                  d_lines_cnt = 0;
              }
          }



          /*------------------- META STATE MACHINE -----------------*/
          if(d_meta_state == VERTICAL_SYNC){


              /*---------------------- SUB STATE MACHINE -----------------*/

              /*** EQUALISING_PULSES ***/
              if(d_sub_state == EQUALISING){

                  // Set the Right Output
                  if(d_samples_cnt < HORIZONTAL_SYNC_DURATION * d_samp_rate)
                      out[i] = HORIZONTAL_SYNC_LEVEL;
                  else
                      out[i] = EQUALISING_LEVEL;

                  // New Equalising Line
                  if(d_samples_cnt > 0.5 * LINE_DURATION * d_samp_rate){
                      d_samples_cnt = 0;
                      d_lines_cnt += 0.5;
                  }

                  // Exit to Serration Lines for pre-equalising
                  if(d_lines_cnt == NBR_EQUALISING_LINES){
                      d_sub_state = SERRATION;
                      d_samples_cnt = 0;
                  }

                  // Exit to SERRATION LINES for post-equalising
                  if((d_lines_cnt == 3 * NBR_EQUALISING_LINES && d_frame_parity == ODD) ||
                      (d_lines_cnt == 3 * NBR_EQUALISING_LINES - 0.5 && d_frame_parity == EVEN)){
                      d_sub_state = BLANKING;
                      d_samples_cnt = 0;
                  }
              }


              /*** SERRATION PULSES ***/
              if(d_sub_state == SERRATION){

                  // Set Right Output
                  if(d_samples_cnt > (0.5 * LINE_DURATION - HORIZONTAL_SYNC_DURATION) * d_samp_rate)
                      out[i] = EQUALISING_LEVEL;
                  else
                      out[i] = HORIZONTAL_SYNC_LEVEL;

                  // New Serration Line
                  if(d_samples_cnt > 0.5 * LINE_DURATION * d_samp_rate){
                      d_samples_cnt = 0;
                      d_lines_cnt += 0.5;
                  }

                  // Exit to Equalising Lines
                  if(d_lines_cnt == 2 * NBR_EQUALISING_LINES){
                      d_sub_state = EQUALISING;
                      d_samples_cnt = 0;
                  }
              }


              /*** BLANKING PULSES ***/
              if(d_sub_state == BLANKING){

                  // Set Right Output
                  if(d_samples_cnt < HORIZONTAL_SYNC_DURATION * d_samp_rate)
                      out[i] = HORIZONTAL_SYNC_LEVEL;
                  else
                      out[i] = EQUALISING_LEVEL;

                  // New Blanking Line
                  if(d_samples_cnt > LINE_DURATION * d_samp_rate){
                      d_samples_cnt = 0;
                      d_lines_cnt++;
                  }

                  // Exit VERTICAL_SYNC YYY
                  if((d_lines_cnt == NBR_VERTICAL_SYNC_LINES && d_frame_parity == ODD) ||
                      (d_lines_cnt == NBR_VERTICAL_SYNC_LINES + 1.5 && d_frame_parity == EVEN)){
                      d_meta_state = LINES_TRANSMISSION;
                      d_sub_state = HORIZONTAL_SYNC;
                      (d_frame_parity == EVEN) ? d_frame_parity = ODD : d_frame_parity = EVEN;
                      d_samples_cnt = 0;
                      d_lines_cnt = 0;
                  }
              }
          }



      } // End of meta state machine
      return noutput_items; // End of signal processing
    }

  } /* namespace ntsc_trx */
} /* namespace gr */
