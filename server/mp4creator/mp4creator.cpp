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
 * Copyright (C) Cisco Systems Inc. 2001.  All Rights Reserved.
 * 
 * Contributor(s): 
 *		Dave Mackie		dmackie@cisco.com
 */

#define MP4CREATOR_GLOBALS
#include "mp4creator.h"
#include "mpeg4ip_getopt.h"

// forward declarations
void PrintTrackList(const char* mp4FileName, u_int32_t verbosity);
char* MakeTempMp4FileName();

MP4TrackId* CreateMediaTracks(
	MP4FileHandle mp4File, const char* inputFileName);

MP4TrackId CreateHintTrack(
	MP4FileHandle mp4File, MP4TrackId mediaTrackId,
	const char* payloadName, bool interleave);


// external declarations

// track creators
MP4TrackId* AviCreator(
	MP4FileHandle mp4File, const char* aviFileName);

MP4TrackId AacCreator(
	MP4FileHandle mp4File, FILE* inFile);

MP4TrackId Mp3Creator(
	MP4FileHandle mp4File, FILE* inFile);

MP4TrackId Mp4vCreator(
	MP4FileHandle mp4File, FILE* inFile);

// hinters
void RfcIsmaHinter(
	MP4FileHandle mp4File, MP4TrackId mediaTrackId, MP4TrackId hintTrackId,
	bool interleave);

void Rfc2250Hinter(
	MP4FileHandle mp4File, MP4TrackId mediaTrackId, MP4TrackId hintTrackId);

void Rfc3119Hinter(
	MP4FileHandle mp4File, MP4TrackId mediaTrackId, MP4TrackId hintTrackId,
	bool interleave);

void Rfc3016Hinter(
	MP4FileHandle mp4File, MP4TrackId mediaTrackId, MP4TrackId hintTrackId);


// main routine
int main(int argc, char** argv)
{
	char* usageString = 
		"usage: %s <options> <mp4-file>\n"
		"  Options:\n"
		"  -create=<input-file>    Create track from <input-file>\n"
		"    input files can be of type: .aac .mp3 .divx .mp4v\n"
		"  -delete=<track-id>      Delete a track\n"
		"  -hint[=<track-id>]      Create hint track, also -H\n"
		"  -interleave             Use interleaved audio payload format, also -I\n"
		"  -list                   List tracks in mp4 file\n"
		"  -mtu=<size>             MTU for hint track\n"
		"  -optimize               Optimize mp4 file layout\n"
		"  -rate=<fps>             Video frame rate, e.g. 30 or 29.97\n"
		"  -verbose[=[1-5]]        Enable debug messages\n"
		;

	bool doCreate = false;
	bool doDelete = false;
	bool doHint = false;
	bool doList = false;
	bool doOptimize = false;
	bool doInterleave = false;
	char* mp4FileName = NULL;
	u_int32_t verbosity = MP4_DETAILS_ERROR;
	char* inputFileName = NULL;
	char* payloadName = NULL;
	MP4TrackId hintTrackId = MP4_INVALID_TRACK_ID;
	MP4TrackId deleteTrackId = MP4_INVALID_TRACK_ID;

	// begin processing command line
	ProgName = argv[0];
	VideoFrameRate = 0;		// determine from input file
	MaxPayloadSize = 1460;

	while (true) {
		int c = -1;
		int option_index = 0;
		static struct option long_options[] = {
			{ "create", 1, 0, 'c' },
			{ "delete", 1, 0, 'd' },
			{ "help", 0, 0, '?' },
			{ "hint", 2, 0, 'H' },
			{ "interleave", 0, 0, 'I' },
			{ "list", 0, 0, 'l' },
			{ "mtu", 1, 0, 'm' },
			{ "optimize", 0, 0, 'O' },
			{ "payload", 1, 0, 'p' },
			{ "rate", 1, 0, 'r' },
			{ "verbose", 2, 0, 'v' },
			{ NULL, 0, 0, 0 }
		};

		c = getopt_long_only(argc, argv, "c:d:H::Ilm:o:Op:v::",
			long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 'c':
			doCreate = true;
			inputFileName = optarg;
			break;
		case 'd':
			if (sscanf(optarg, "%u", &deleteTrackId) != 1) {
				fprintf(stderr, 
					"%s: bad track-id specified: %s\n",
					 ProgName, optarg);
				exit(EXIT_COMMAND_LINE);
			}
			doDelete = true;
			break;
		case 'H':
			doHint = true;
			if (optarg) {
				if (sscanf(optarg, "%u", &hintTrackId) != 1) {
					fprintf(stderr, 
						"%s: bad track-id specified: %s\n",
						 ProgName, optarg);
					exit(EXIT_COMMAND_LINE);
				}
			}
			break;
		case 'I':
			doInterleave = true;
			break;
		case 'l':
			doList = true;
			break;
		case 'm':
			u_int32_t mtu;
			if (sscanf(optarg, "%u", &mtu) != 1 || mtu < 64) {
				fprintf(stderr, 
					"%s: bad mtu specified: %s\n",
					 ProgName, optarg);
				exit(EXIT_COMMAND_LINE);
			}
			MaxPayloadSize = mtu - 40;	// subtract IP, UDP, and RTP hdrs
			break;
		case 'O':
			doOptimize = true;
			break;
		case 'p':
			payloadName = optarg;
			break;
		case 'r':
			if (sscanf(optarg, "%f", &VideoFrameRate) != 1) {
				fprintf(stderr, 
					"%s: bad rate specified: %s\n",
					 ProgName, optarg);
				exit(EXIT_COMMAND_LINE);
			}
			break;
		case 'v':
			verbosity |= (MP4_DETAILS_READ | MP4_DETAILS_WRITE);
			if (optarg) {
				u_int32_t level;
				if (sscanf(optarg, "%u", &level) == 1) {
					if (level >= 2) {
						verbosity |= MP4_DETAILS_TABLE;
					} 
					if (level >= 3) {
						verbosity |= MP4_DETAILS_SAMPLE;
					} 
					if (level >= 4) {
						verbosity |= MP4_DETAILS_HINT;
					} 
					if (level >= 5) {
						verbosity = MP4_DETAILS_ALL;
					} 
				}
			}
			break;
		case '?':
			fprintf(stderr, usageString, ProgName);
			exit(EXIT_SUCCESS);
		default:
			fprintf(stderr, "%s: unknown option specified, ignoring: %c\n", 
				ProgName, c);
		}
	}

	// check that we have at least one non-option argument
	if ((argc - optind) < 1) {
		fprintf(stderr, usageString, ProgName);
		exit(EXIT_COMMAND_LINE);
	}

	// if it appears we have two file names, then assume -c for the first
	if ((argc - optind) == 2 && inputFileName == NULL) {
		doCreate = true;
		inputFileName = argv[optind++];
	}

	// point to the specified mp4 file name
	mp4FileName = argv[optind++];

	// warn about extraneous non-option arguments
	if (optind < argc) {
		fprintf(stderr, "%s: unknown options specified, ignoring: ", ProgName);
		while (optind < argc) {
			fprintf(stderr, "%s ", argv[optind++]);
		}
		fprintf(stderr, "\n");
	}

	// consistency checks
	if (doDelete && (doCreate || doHint)) {
		fprintf(stderr, 
			"%s: delete operation must be done separately\n",
			 ProgName);
		exit(EXIT_COMMAND_LINE);
	}

	// end processing of command line

	if (doList) {
		PrintTrackList(mp4FileName, verbosity);
		exit(EXIT_SUCCESS);
	}

	// test if mp4 file exists
	bool mp4FileExists = (access(mp4FileName, F_OK) == 0);

	MP4FileHandle mp4File;

	if (doCreate || doHint) {
		if (!mp4FileExists) {
			if (doCreate) {
				mp4File = MP4Create(mp4FileName, verbosity);
				if (mp4File) {
					MP4SetTimeScale(mp4File, 90000);
				}
			} else {
				fprintf(stderr,
					"%s: can't hint track in file that doesn't exist\n", 
					ProgName);
				exit(EXIT_CREATE_FILE);
			}
		} else {
			mp4File = MP4Modify(mp4FileName, verbosity);
		}

		if (!mp4File) {
			// mp4 library should have printed a message
			exit(EXIT_CREATE_FILE);
		}

		if (doCreate) {
			MP4TrackId* pTrackIds = CreateMediaTracks(mp4File, inputFileName);

			if (pTrackIds && doHint) {
				while (*pTrackIds != MP4_INVALID_TRACK_ID) {
					CreateHintTrack(mp4File, *pTrackIds, 
						payloadName, doInterleave);
					pTrackIds++;
				}
			}
		} else {
			CreateHintTrack(mp4File, hintTrackId, payloadName, doInterleave);
		} 

		if (doCreate) {
			MP4MakeIsmaCompliant(mp4File);
		}

		MP4Close(mp4File);

	} else if (doDelete) {
		if (!mp4FileExists) {
			fprintf(stderr,
				"%s: can't delete track in file that doesn't exist\n", 
				ProgName);
			exit(EXIT_CREATE_FILE);
		}

		mp4File = MP4Modify(mp4FileName, verbosity);
		if (!mp4File) {
			// mp4 library should have printed a message
			exit(EXIT_CREATE_FILE);
		}

		MP4DeleteTrack(mp4File, deleteTrackId);

		MP4Close(mp4File);

		doOptimize = true;	// to purge unreferenced track data
	}

	if (doOptimize) {
		char* outputFileName = MakeTempMp4FileName();
		ASSERT(outputFileName);

		MP4Optimize(mp4FileName, outputFileName, verbosity);

		int rc;

#ifdef _WIN32
		rc = remove(mp4FileName);
		if (rc == 0) {
			rc = rename(outputFileName, mp4FileName);
		}
#else
		rc = rename(outputFileName, mp4FileName);
#endif
		if (rc != 0) {
			fprintf(stderr, "%s: can't overwrite %s, output is in %s\n",
				ProgName, mp4FileName, outputFileName);
		}
	}

	return(EXIT_SUCCESS);
}

MP4TrackId* CreateMediaTracks(MP4FileHandle mp4File, const char* inputFileName)
{
	FILE* inFile = fopen(inputFileName, "rb");

	if (inFile == NULL) {
		fprintf(stderr, 
			"%s: can't open file %s: %s\n",
			ProgName, inputFileName, strerror(errno));
		exit(EXIT_CREATE_MEDIA);
	}

	const char* extension = strrchr(inputFileName, '.');
	if (extension == NULL) {
		fprintf(stderr, 
			"%s: no file type extension\n", ProgName);
		exit(EXIT_CREATE_MEDIA);
	}

	static MP4TrackId trackIds[2] = {
		MP4_INVALID_TRACK_ID, MP4_INVALID_TRACK_ID
	};
	MP4TrackId* pTrackIds = trackIds;

	if (!strcasecmp(extension, ".avi")) {
		fclose(inFile);
		inFile = NULL;

		pTrackIds = AviCreator(mp4File, inputFileName);

	} else if (!strcasecmp(extension, ".aac")) {
		trackIds[0] = AacCreator(mp4File, inFile);

	} else if (!strcasecmp(extension, ".mp3")
	  || !strcasecmp(extension, ".mp1")
	  || !strcasecmp(extension, ".mp2")) {
		trackIds[0] = Mp3Creator(mp4File, inFile);

	} else if (!strcasecmp(extension, ".divx")
	  || !strcasecmp(extension, ".mp4v")) {
		trackIds[0] = Mp4vCreator(mp4File, inFile);

	} else {
		fprintf(stderr, 
			"%s: unknown file type\n", ProgName);
		exit(EXIT_CREATE_MEDIA);
	}

	if (inFile) {
		fclose(inFile);
	}

	return pTrackIds;
}

MP4TrackId CreateHintTrack(MP4FileHandle mp4File, MP4TrackId mediaTrackId,
	const char* payloadName, bool interleave)
{
	if (MP4GetTrackNumberOfSamples(mp4File, mediaTrackId) == 0) {
		fprintf(stderr, 
			"%s: couldn't create hint track, no media samples\n", ProgName);
		return MP4_INVALID_TRACK_ID;
	}

	// create the hint track
	MP4TrackId hintTrackId = MP4AddHintTrack(mp4File, mediaTrackId);

	if (hintTrackId == MP4_INVALID_TRACK_ID) {
		fprintf(stderr, 
			"%s: couldn't create hint track\n", ProgName);
		exit(EXIT_CREATE_HINT);
	}

	// vector out to specific hinters
	const char* trackType = MP4GetTrackType(mp4File, mediaTrackId);

	if (!strcmp(trackType, MP4_AUDIO_TRACK_TYPE)) {
		u_int8_t audioType = MP4GetTrackAudioType(mp4File, mediaTrackId);

		switch (audioType) {
		case MP4_MPEG4_AUDIO_TYPE:
		case MP4_MPEG2_AAC_MAIN_AUDIO_TYPE:
		case MP4_MPEG2_AAC_LC_AUDIO_TYPE:
		case MP4_MPEG2_AAC_SSR_AUDIO_TYPE:
			RfcIsmaHinter(mp4File, mediaTrackId, hintTrackId, interleave);
			break;
		case MP4_MPEG1_AUDIO_TYPE:
		case MP4_MPEG2_AUDIO_TYPE:
			if (!strcasecmp(payloadName, "3119")) {
				Rfc3119Hinter(mp4File, mediaTrackId, hintTrackId, interleave);
			} else {
				Rfc2250Hinter(mp4File, mediaTrackId, hintTrackId);
			}
			break;
		default:
			fprintf(stderr, 
				"%s: can't hint non-MPEG4/non-MP3 audio type\n", ProgName);
			exit(EXIT_CREATE_HINT);
		}
	} else if (!strcmp(trackType, MP4_VIDEO_TRACK_TYPE)) {
		u_int8_t videoType = MP4GetTrackVideoType(mp4File, mediaTrackId);

		if (videoType == MP4_MPEG4_VIDEO_TYPE) {
			Rfc3016Hinter(mp4File, mediaTrackId, hintTrackId);
		} else {
			fprintf(stderr, 
				"%s: can't hint non-MPEG4 video type\n", ProgName);
			exit(EXIT_CREATE_HINT);
		}
	} else {
		fprintf(stderr, 
			"%s: can't hint track type %s\n", ProgName, trackType);
		exit(EXIT_CREATE_HINT);
	}

	return hintTrackId;
}

void PrintTrackList(const char* mp4FileName, u_int32_t verbosity)
{
	MP4FileHandle mp4File = MP4Read(mp4FileName, verbosity);

	if (!mp4File) {
		fprintf(stderr, "%s: couldn't open %s: %s", 
			ProgName, mp4FileName, strerror(errno));
		exit(1);
	}

	u_int32_t numTracks = MP4GetNumberOfTracks(mp4File);

	printf("Id\tType\n");
	for (u_int32_t i = 0; i < numTracks; i++) {
		MP4TrackId trackId = MP4FindTrackId(mp4File, i);
		const char* trackType = MP4GetTrackType(mp4File, trackId);
		printf("%u\t%s\n", trackId, trackType);
	}

	MP4Close(mp4File);
}

// there are so many attempts to get this right
// that for portablity reasons, it's best just to roll our own
char* MakeTempMp4FileName()
{
#ifndef _WIN32
	static char tempFileName[64];
	u_int32_t i;
	for (i = getpid(); i < 0xFFFFFFFF; i++) {
		sprintf(tempFileName, "./tmp%u.mp4", i);
		if (access(tempFileName, F_OK) != 0) {
			break;
		}
	}
	if (i == 0xFFFFFFFF) {
		return NULL;
	}
#else
	static char tempFileName[MAX_PATH + 3];
	GetTempFileName(".", // dir. for temp. files 
					"mp4",                // temp. filename prefix 
					0,                    // create unique name 
					tempFileName);        // buffer for name 
#endif

	return tempFileName;
}
