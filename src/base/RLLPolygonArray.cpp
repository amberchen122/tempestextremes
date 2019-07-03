///////////////////////////////////////////////////////////////////////////////
///
///	\file    RLLPolygonArray.h
///	\author  Paul Ullrich
///	\version July 2, 2019
///
///	<remarks>
///		Copyright 2000-2019 Paul Ullrich
///
///		This file is distributed as part of the Tempest source code package.
///		Permission is granted to use, copy, modify and distribute this
///		source code and its documentation under the terms of the GNU General
///		Public License.  This software is provided "as is" without express
///		or implied warranty.
///	</remarks>

#include "RLLPolygonArray.h"
#include "Exception.h"
#include "STLStringHelper.h"

#include <iostream>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////

void RLLPolygonArray::FromFile(
	const std::string & strFilename
) {
	if ((m_vecNames.size() != 0) ||
	    (m_vecNodes.size() != 0)
	) {
		_EXCEPTIONT("RLLPolygonArray already initialized");
	}

	// String buffer
	std::string strBuffer;

	// Open the file as an input stream
	std::ifstream ifInput(strFilename);
	if (!ifInput.is_open()) {
		_EXCEPTION1("Unable to open input file \"%s\"", strFilename.c_str());
	}

	// Read mode
	enum {
		RLLPolygonArray_ReadModeCount,
		RLLPolygonArray_ReadModePolygons,
		RLLPolygonArray_ReadModeDone
	} eReadMode = RLLPolygonArray_ReadModeCount;

	// Count
	int nPolygonCount = (-1);

	// Number of polygons read
	int nPolygonRead = 0;

	// Loop through all lines
	int iLine = 0;
	for (;;) {
		iLine++;
		getline(ifInput, strBuffer);
		if (ifInput.eof()) {
			break;
		}

		if (strBuffer.length() == 0) {
			continue;
		}
		if (strBuffer[0] == '#') {
			continue;
		}

		// Check for end of file
		if (eReadMode == RLLPolygonArray_ReadModeDone) {
			_EXCEPTION1("Too many entries found in file \"%s\"",
				strFilename.c_str());
		}

		// Get the number of regions
		if (eReadMode == RLLPolygonArray_ReadModeCount) {
			nPolygonCount = atoi(strBuffer.c_str());
			if ((nPolygonCount <= 0) || (nPolygonCount > 100000)) {
				_EXCEPTION2("Sanity check failed: RLLPolygonArray file \"%s\" "
					"indicates %i polygons present (max 100000)",
					strFilename.c_str(), nPolygonCount);
			}
			m_vecNodes.resize(nPolygonCount);
			eReadMode = RLLPolygonArray_ReadModePolygons;
			continue;
		}

		// Read in a polygon
		if (eReadMode == RLLPolygonArray_ReadModePolygons) {

			enum {
				RLLPolygonArray_ReadModePolyInfo,
				RLLPolygonArray_ReadModePolyCount,
				RLLPolygonArray_ReadModePolyLon,
				RLLPolygonArray_ReadModePolyLat,
				RLLPolygonArray_ReadModePolyDone
			} eReadModePoly = RLLPolygonArray_ReadModePolyInfo;

			std::string strPolygonName;
			std::vector<double> vecPolygonLon;
			std::vector<double> vecPolygonLat;
			int nVertexCount = 0;
			int nLonRead = 0;
			int nLatRead = 0;
			int iLast = 0;
			for (int i = 0; i <= strBuffer.length(); i++) {

				if (eReadModePoly == RLLPolygonArray_ReadModePolyDone) {
					_EXCEPTION2("Spurious characters found on line %i of \"%s\"",
						iLine, strFilename.c_str());
				}

				// Check comma
				if ((i == strBuffer.length()) || (strBuffer[i] == ',')) {
					std::string strText = strBuffer.substr(iLast, i-iLast);
					iLast = i + 1;

					// Information about polygon
					if (eReadModePoly == RLLPolygonArray_ReadModePolyInfo) {
						strPolygonName = strText;
						eReadModePoly = RLLPolygonArray_ReadModePolyCount;
						continue;
					}

					// Number of vertices
					if (eReadModePoly == RLLPolygonArray_ReadModePolyCount) {
						nVertexCount = atoi(strText.c_str());
						if ((nVertexCount <= 0) || (nVertexCount > 1000)) {
							_EXCEPTION3("Sanity check failed: RLLPolygonArray file \"%s\" "
								"line %i indicates %i vertices present (max 1000)",
								strFilename.c_str(), iLine, nVertexCount);
						}
						eReadModePoly = RLLPolygonArray_ReadModePolyLon;
						continue;
					}

					// Longitude
					if (eReadModePoly == RLLPolygonArray_ReadModePolyLon) {
						if (!STLStringHelper::IsFloat(strText)) {
							_EXCEPTION3("Syntax error on line %i of \"%s\":"
								"Invalid float \"%s\"",
								iLine, strFilename.c_str(), strText.c_str());
						}
						vecPolygonLon.push_back(atof(strText.c_str()));

						if (vecPolygonLon.size() == nVertexCount) {
							eReadModePoly = RLLPolygonArray_ReadModePolyLat;
						}
						continue;
					}

					// Latitude
					if (eReadModePoly == RLLPolygonArray_ReadModePolyLat) {
						if (!STLStringHelper::IsFloat(strText)) {
							_EXCEPTION3("Syntax error on line %i of \"%s\":"
								"Invalid float \"%s\"",
								iLine, strFilename.c_str(), strText.c_str());
						}
						vecPolygonLat.push_back(atof(strText.c_str()));

						if (vecPolygonLat.size() == nVertexCount) {
							eReadModePoly = RLLPolygonArray_ReadModePolyDone;
						}
						continue;
					}
				}

				// Check quotation
				if (strBuffer[i] == '\"') {
					if (eReadModePoly != RLLPolygonArray_ReadModePolyInfo) {
						_EXCEPTION2("Quotation marks only allowed in polygon name on line %i of \"%s\"",
							iLine, strFilename.c_str());
					}
					for (i++; i < strBuffer.length(); i++) {
						if (strBuffer[i] == '\"') {
							break;
						}
					}
					if (i == strBuffer.length()) {
						_EXCEPTION2("Unterminated quotation mark on line %i of \"%s\"",
							iLine, strFilename.c_str());
					}
				}
			}

			// Check polygon vertices
			if (vecPolygonLon.size() != nVertexCount) {
				_EXCEPTION2("Insufficient data on line %i of \"%s\"",
					iLine, strFilename.c_str());
			}
			if (vecPolygonLat.size() != nVertexCount) {
				_EXCEPTION2("Insufficient data on line %i of \"%s\"",
					iLine, strFilename.c_str());
			}

			// Insert polygon vertices into array
			m_vecNames.push_back(strPolygonName);
			for (int v = 0; v < nVertexCount; v++) {
				RLLPoint pt;
				pt.lon = vecPolygonLon[v];
				pt.lat = vecPolygonLat[v];
				m_vecNodes.push_back(pt);
			}

			if (m_vecNames.size() == nPolygonCount) {
				eReadMode = RLLPolygonArray_ReadModeDone;
			}
		}
	}

	if (m_vecNames.size() != nPolygonCount) {
		_EXCEPTION3("Number of polygons (%i) does not match count (%i) in \"%s\"",
			strFilename.c_str(), m_vecNames.size(), nPolygonCount);
	}
}

///////////////////////////////////////////////////////////////////////////////

const std::string & RLLPolygonArray::NameOfRegionContainingPoint(
	const RLLPoint & pt
) {
	return m_vecNames[0];
}

///////////////////////////////////////////////////////////////////////////////

