/**************************************************************************
  exif.h  -- A simple ISO C++ library to parse basic EXIF 
             information from a JPEG file.
  Based on the description of the EXIF file format at:
  -- http://park2.wakwak.com/~tsuruzoh/Computer/Digicams/exif-e.html
  -- http://www.media.mit.edu/pia/Research/deepview/exif.html
  -- http://www.exif.org/Exif2-2.PDF
  Copyright (c) 2010-2015 Mayank Lahiri
  mlahiri@gmail.com
  All rights reserved.
  VERSION HISTORY:
  ================
  2.2: Release December 2014
       -- 
  2.1: Released July 2013
       -- fixed a bug where JPEGs without an EXIF SubIFD would not be parsed
       -- fixed a bug in parsing GPS coordinate seconds
       -- fixed makefile bug
       -- added two pathological test images from Matt Galloway
          http://www.galloway.me.uk/2012/01/uiimageorientation-exif-orientation-sample-images/
       -- split main parsing routine for easier integration into Firefox
  2.0: Released February 2013
       -- complete rewrite
       -- no new/delete
       -- added GPS support
  1.0: Released 2010
  
  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions are met:
  -- Redistributions of source code must retain the above copyright notice, 
     this list of conditions and the following disclaimer.
  -- Redistributions in binary form must reproduce the above copyright notice, 
     this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS 
   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN 
   NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
   OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __EXIF_H
#define __EXIF_H

#include <string>
#include <memory>
#include "JpegEXIF.h"

typedef std::shared_ptr<JpegEXIF> JpegExif_Ptr;
// 
// Class responsible for storing and parsing EXIF information from a JPEG blob
//
class EXIFInfo {
 public:
  // Parsing function for an entire JPEG image buffer.
  //
  // PARAM 'data': A pointer to a JPEG image.
  // PARAM 'length': The length of the JPEG image.
  // RETURN:  PARSE_EXIF_SUCCESS (0) on succes with 'result' filled out
  //          error code otherwise, as defined by the PARSE_EXIF_ERROR_* macros
	 JpegExif_Ptr parseFrom(const unsigned char *data, size_t length);
	 JpegExif_Ptr parseFrom(const std::string &data);

  // Parsing function for an EXIF segment. This is used internally by parseFrom()
  // but can be called for special cases where only the EXIF section is 
  // available (i.e., a blob starting with the bytes "Exif\0\0").
	 JpegExif_Ptr parseFromEXIFSegment(const unsigned char *buf, size_t len);
  
  /*
  // Set all data members to default values.
  void clear();

  // Data fields filled out by parseFrom()
  char ByteAlign;                   // 0 = Motorola byte alignment, 1 = Intel 
  std::string ImageDescription;     // Image description
  std::string Make;                 // Camera manufacturer's name
  std::string Model;                // Camera model
  unsigned short Orientation;       // Image orientation, start of data corresponds to
                                    // 0: unspecified in EXIF data
                                    // 1: upper left of image
                                    // 3: lower right of image
                                    // 6: upper right of image
                                    // 8: lower left of image
                                    // 9: undefined
  unsigned short BitsPerSample;     // Number of bits per component
  std::string Software;             // Software used
  std::string DateTime;             // File change date and time
  std::string DateTimeOriginal;     // Original file date and time (may not exist)
  std::string DateTimeDigitized;    // Digitization date and time (may not exist)
  std::string SubSecTimeOriginal;   // Sub-second time that original picture was taken
  std::string copyright;            // File copyright information
  std::string title;				// XP Title
  std::string comment;				// XP Comment Byte Comment tag used by Windows, encoded in UCS2
  std::string author;				// XP Author Byte Author tag used by Windows, encoded in UCS2
  std::string keywords;				// XP Keywords Byte Keywords tag used by Windows, encoded in UCS2
  std::string subject;				// XP Subject
  unsigned short rating;			// Short Rating tag used by Windows
  unsigned short ratingPercent;		// Rating tag used by Windows, value in percent
  double ExposureTime;              // Exposure time in seconds
  double FNumber;                   // F/stop
  unsigned short ISOSpeedRatings;   // ISO speed
  double ShutterSpeedValue;         // Shutter speed (reciprocal of exposure time)
  double ExposureBiasValue;         // Exposure bias value in EV
  unsigned short exposureProgram;	// The class of the program used by the camera to set exposure when the picture is taken.
  double SubjectDistance;           // Distance to focus point in meters
  double FocalLength;               // Focal length of lens in millimeters
  unsigned short FocalLengthIn35mm; // Focal length in 35mm film
  char Flash;                       // 0 = no flash, 1 = flash used
  unsigned short MeteringMode;      // Metering mode
                                    // 1: average
                                    // 2: center weighted average
                                    // 3: spot
                                    // 4: multi-spot
                                    // 5: multi-segment
  unsigned ImageWidth;              // Image width reported in EXIF data
  unsigned ImageHeight;             // Image height reported in EXIF data
  unsigned short lightSource;       // The kind of light source.
									// 0 = Unknown
									// 	  1 = Daylight
								    // 	  2 = Fluorescent
								    // 	  3 = Tungsten(incandescent light)
								    // 	  4 = Flash
								    // 	  9 = Fine weather
								    // 	  10 = Cloudy weather
								    // 	  11 = Shade
								    // 	  12 = Daylight fluorescent(D 5700 - 7100K)
								    // 	  13 = Day white fluorescent(N 4600 - 5400K)
								    // 	  14 = Cool white fluorescent(W 3900 - 4500K)
								    // 	  15 = White fluorescent(WW 3200 - 3700K)
								    // 	  17 = Standard light A
								    // 	  18 = Standard light B
								    // 	  19 = Standard light C
								    // 	  20 = D55
								    // 	  21 = D65
								    // 	  22 = D75
								    // 	  23 = D50
								    // 	  24 = ISO studio tungsten
								    // 	  255 = Other light source
 
  unsigned short  whiteBalance;		// Short This tag indicates the white balance mode set when the image was shot.
  
  double DigitalZoomRatio;			// This tag indicates the digital zoom ratio when the image was shot.If the numerator of the recorded value is 0, this indicates that digital zoom was not used.

  unsigned short SceneCaptureType;	// This tag indicates the type of scene that was shot.It can also be used to record the mode in which the image was shot.Note that this differs from the <SceneType> tag.
  
  unsigned short Contrast;			// This tag indicates the direction of contrast processing applied by the camera when the image was shot.
  unsigned short Saturation;		// This tag indicates the direction of saturation processing applied by the camera when the image was shot.
  unsigned short Sharpness;		    // This tag indicates the direction of sharpness processing applied by the camera when the image was shot.
  
  unsigned short SubjectDistanceRange; // This tag indicates the distance to the subject.

  
 

  struct Geolocation_t {            // GPS information embedded in file
    double Latitude;                  // Image latitude expressed as decimal
    double Longitude;                 // Image longitude expressed as decimal
    double Altitude;                  // Altitude in meters, relative to sea level
    char AltitudeRef;                 // 0 = above sea level, -1 = below sea level
    struct Coord_t {
      double degrees;               
      double minutes;
      double seconds;
      char direction;
    } LatComponents, LonComponents;   // Latitude, Longitude expressed in deg/min/sec 
  } GeoLocation;
  EXIFInfo() {
    clear();
  }
  */
};

// Parse was successful
#define PARSE_EXIF_SUCCESS                    0
// No JPEG markers found in buffer, possibly invalid JPEG file
#define PARSE_EXIF_ERROR_NO_JPEG              1982
// No EXIF header found in JPEG file.
#define PARSE_EXIF_ERROR_NO_EXIF              1983
// Byte alignment specified in EXIF file was unknown (not Motorola or Intel).
#define PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN    1984
// EXIF header was found, but data was corrupted.
#define PARSE_EXIF_ERROR_CORRUPT              1985

#endif

