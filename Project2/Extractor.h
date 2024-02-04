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
#include "Common.h"

class ExtractorHelper
{
private:
	ulong ulOffset;

public:	
	ExtractorHelper(vector<string> DirList);
	std::string ZlibDecompress(const std::string& str);
	void MapData(vector<MappedData>& DataVec, string& XMLBuffer, ifstream& FileBuffer);
	void ExportData(vector<MappedData>& DataVec, string Directory);

};

class DecryptionHelper
{
public:
	//Header
	static void SetHeaderKeys(vector<uint>& HeaderKeys, uint FileSize);
	virtual void HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);

	//XOR Decrypt(Algo2/Algo3)
	static void DecryptAlgo2(string& Buffer);
	static void DecryptAlgo3(string& Buffer);

	//Decryption functions for algorithms and komv5/v6
	static void SHA1_SeedGenerate(vector<uint>& HeaderKeys, uint FileSize);
	static void BlowfishDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);
};

class DecryptionV4 : DecryptionHelper
{
	//Header
	void HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);
};

class DecryptionV5 : DecryptionHelper
{
	//Header
	void HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);
};

class DecryptionV6 : DecryptionHelper
{
	//Header
	void HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);
};