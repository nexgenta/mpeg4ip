/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999 Apple Computer, Inc.  All Rights Reserved.
 * The contents of this file constitute Original Code as defined in and are 
 * subject to the Apple Public Source License Version 1.1 (the "License").  
 * You may not use this file except in compliance with the License.  Please 
 * obtain a copy of the License at http://www.apple.com/publicsource and 
 * read it before using this file.
 * 
 * This Original Code and all software distributed under the License are 
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the License for 
 * the specific language governing rights and limitations under the 
 * License.
 * 
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/* 
	File:		QTSSDemoModule.h

	Contains:	This is a modified version of the QTSSAccessModule also released with
				QTSS 2.0. It has been modified to shrink the linespacing so that
				the code can fit on slides. Also, this module issues redirects to
				an error movie.
					
	

*/

#ifndef _QTSSDEMOMODULE_H__
#define _QTSSDEMOMODULE_H__

#include "QTSS.h"

extern "C"
{
	QTSS_Error QTSSDemoAuthorizationModule_Main(void* inPrivateArgs);
}

#endif
