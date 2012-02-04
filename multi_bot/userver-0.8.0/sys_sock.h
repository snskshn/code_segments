/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.
          
    This file is Copyright (C) 2007  Mark Groves

    Authors: Mark Groves <mjgroves@uwaterloo.ca>
    See AUTHORS file for list of contributors to the project.
  
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA
*/


#ifdef QUEUE_BUFS 
struct sock_write_stats {
  unsigned int sws_samples;
  unsigned int sws_print_intr;
  int sws_max_buf_len;
  int sws_avg_buf_len;
  unsigned int sws_set_owner;
  unsigned int sws_wfree;
  unsigned int sws_other;
  unsigned int sws_skbs;
  unsigned int sws_wmem_len;

};
#define UPDATE_SOCK_WRITE_STATS(sk, caller, alloc) \
  if (sk->sk_write_stats.sws_samples) { \
    struct sock_write_stats* sws = &sk->sk_write_stats; \
    int cur_buf_len = atomic_read(&sk->sk_wmem_alloc); \
    int diff = cur_buf_len - sws->sws_avg_buf_len; \
    if (cur_buf_len > sws->sws_max_buf_len) \
      sws->sws_max_buf_len = cur_buf_len; \
    sws->sws_avg_buf_len += (diff / sws->sws_samples); \
    if ((sws->sws_samples & sws->sws_print_intr) == 0) { \
      printk("UPDATE_SOCK_WRITE_STATS(%s:%d,%p,%d,%d,%d,%d)\n", \
             __FUNCTION__, __LINE__, \
             sk, cur_buf_len, sws->sws_samples, \
             sws->sws_max_buf_len, sws->sws_avg_buf_len); \
    } \
    sws->sws_samples++; \
    sws->sws_##caller++; \
    sws->sws_skbs += alloc; \
  }
#endif

