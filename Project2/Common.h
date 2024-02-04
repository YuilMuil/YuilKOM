/******************************************************************************
 * YuilKOM
 * Copyright (c) 2024 YuilMuil
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *****************************************************************************/


#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <zlib.h>

#include <pugixml.hpp>	   // XML parser
#include "CryptoKeys.h" //Cryptography keys(header and xor)

//Importing common STL functions we use into global namespace.
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::array;
using std::cout;
using std::cerr;
using std::ios;

namespace fs = std::filesystem;

enum AlgorithmVer
{
	NO_XOR = 0,
	SIMPLE_XOR = 2,
	ADVANCED_XOR = 3,
	END = 100,
};

class MappedData
{
public:
	string Name; //Name of file
	int OriginalSize; //size before zlib compressed
	int CompressedSize; //zlib compression after size
	string Checksum; //Adler32 of entire file
	string FileTime; //Same as checksum
	int Algorithm; //Algorithm version
	string MappedID; //Seems to used to verify whether the file is legit or not(hard-coded I think)
	string FileBuffer; //File Buffer
};

class KOM
{
public:
	string Header; //first 27 bytes, check whether it is komv3 or komv4
	uint XMLSize = 0; //68-72 byte offset.
	string XMLEncrypted; //Encrypted XML Data
	string XMLDecrypted; //Decrypted XML Data

	vector<uint> HeaderKeys; //Size is 12 bytes or 3 unsigned ints.
	vector<uint> MappedIDKeys; //Size is 20 bytes or 5 unsigned ints.
	vector<MappedData> MappedDataVec;
};