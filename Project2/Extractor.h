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
	std::string ZlibDecompress(const std::string& str);
	void MapData(vector<MappedData>& DataVec, string& XMLBuffer, ifstream& FileBuffer);
	void ExportData(vector<MappedData>& DataVec, string Directory);

};

class DecryptionHelper
{
public:
	//Header
	virtual void SetHeaderKeys(vector<uint>& HeaderKeys, uint FileSize);
	virtual void HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);

	//XOR Decrypt(Algo2/Algo3) -> Algo3 is 034-040 and Algo2 is for the rest
	static void BlowfishDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);
	static void DecryptAlgo2(string& Buffer);
	static void DecryptAlgo3(string& Buffer);
};

class DecryptionV4 : DecryptionHelper
{
	//Header
	void SetHeaderKeys(vector<uint>& HeaderKeys, uint FileSize);
	void HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);
};

class DecryptionV5 : DecryptionHelper
{
	//Header
	void SetHeaderKeys(vector<uint>& HeaderKeys, uint FileSize);
	void HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);
};

class DecryptionV6 : DecryptionHelper
{
	//Header
	void SetHeaderKeys(vector<uint>& HeaderKeys, uint FileSize);
	void HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys);
};