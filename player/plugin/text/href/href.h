/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is MPEG4IP.
 * 
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2005.  All Rights Reserved.
 * 
 * Contributor(s): 
 *              Bill May        wmay@cisco.com
 */
/*
 * href.h - class definition for raw audio codec
 */

#ifndef __HREF_H__
#define __HREF_H__ 1
#include "codec_plugin.h"
#include "text_plugin.h"

#define m_vft c.v.text_vft
#define m_ifptr c.ifptr

typedef struct href_codec_t {
  codec_data_t c;
  char *m_buffer;
  uint32_t m_buffer_len;
  char *m_base_url;
  char *m_url;
} href_codec_t;

#endif
