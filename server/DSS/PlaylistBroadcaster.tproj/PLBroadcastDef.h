
#ifndef __PLBroadcastDef__
#define __PLBroadcastDef__


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

#example (1) A FULL 1.0 DECLARED FILE
#Lines beginning with "#" characters are comments
#The order of the following entries is unimportant
#Quotes are optional for values

destination_ip_address  225.225.225.225	
destination_base_port  5004
play_mode  [sequential, sequential_looped, weighted]
limit_play  enabled
limit_seq_length  10
play_list_file /path/file
sdp_file /path/file
log_file /path/file

*/

#include "OSHeaders.h"

class PLBroadcastDef {

	public:
		PLBroadcastDef( const char* setupFileName );
		virtual ~PLBroadcastDef();
		
		Bool16	ParamsAreValid() { return mParamsAreValid; }
		void	ShowSettings();
		
		static Bool16 ConfigSetter( const char* paramName, const char* paramValue[], void * userData );

										// * == default value, <r> required input
		char*	mDestAddress;	 		// [0.0.0.0 | domain name?] *127.0.0.1 ( self )
		char*	mBasePort;				// [ 0...32k?] *5004
		
		
		char*	mPlayMode; 				// [sequential | *sequential_looped | weighted]
		// removed at build 12 char*	mLimitPlay;				// [*enabled | disabled]
		char*	mLimitPlayQueueLength; // [ 0...32k?] *20
		char*	mPlayListFile; 			// [os file path] *<PLBroadcastDef-name>.ply
		char*	mSDPFile;				// [os file path] <r>
		char*	mLogging; 				// [*enabled | disabled]
		char*	mLogFile;				// [os file path] *<PLBroadcastDef-name>.log
		char*	mSDPReferenceMovie;		// [os file path]
	
	protected:
		Bool16	mParamsAreValid;
		Bool16    SetValue( char** dest, const char* value);
		Bool16	SetDefaults( const char* setupFileName );
};

#endif