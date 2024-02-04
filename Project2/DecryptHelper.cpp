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


#include "Extractor.h"

//ID -> Origin is v14. v14 -> Initial seed based on Filename(Upper() is called for the file name)
// ->Sent to a function where there it is 52 * FileNameSeedPtr[4] + PtrA (PtrA points to KOM Header)
// -> (uint)(52 * FileNameSeedPtr[4] + PtrA)[4] * 5 is the seed or start of where the first 5 xor keys are generated.
// 
//Header XOR Encryption -> XML -> DecryptUniversal(generate 5 xor keys(4 bytes/uint) based on some ID and
//generate a SHA1 hash based on an appended version. Then use that as the key for decrypt(blowfish)/
//Then afterwards, check the Algorithm version on the XML for the current file and then decrypt it.
//Finally do zlib decompression

//Algo0 -> Zlib compressed
//Algo2 -> XOR
//Algo3 -> Algo2 + minor modification?
void DecryptionHelper::SetHeaderKeys(vector<uint>& HeaderKeys, uint FileSize)
{
	//Filesize is the seed
	auto startIndex = (static_cast<ulong>(12u) * (FileSize % 0xC8u) + 12u) / sizeof(uint);

	for (int i = 0; i < 3; i++)
		HeaderKeys.push_back(MappedHeaderKeys[startIndex++]);
}

void DecryptionV4::HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys)
{
	int count = 0;		   //Count until encrypted XML is empty
	int KeyAreaCnt = 0;    //Index of encryption header key used
	unsigned int* pointer; //pointer for encrypted xml
	unsigned int buffer = 0; //temporary 4 byte buffer when XOR decrypting 4 bytes at a time

	DecryptedXML.resize(EncryptedXML.size()); //Resize DecryptedXML string size

	while (count < EncryptedXML.size())
	{
		if (KeyAreaCnt >= HeaderKeys.size())
			KeyAreaCnt = 0;

		pointer = (unsigned int*)&EncryptedXML[count]; //point to encrypted XML
		buffer = *pointer ^ HeaderKeys[KeyAreaCnt];	   //4 byte decrypted buffer(int)

		//Map int to the DecryptedXML string
		DecryptedXML[count++] = (buffer & 0x000000ff);
		DecryptedXML[count++] = (buffer & 0x0000ff00) >> 8;
		DecryptedXML[count++] = (buffer & 0x00ff0000) >> 16;
		DecryptedXML[count++] = (buffer & 0xff000000) >> 24;

		KeyAreaCnt++;
	}
}

void DecryptionV5::HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys)
{
	
}



void DecryptionV6::HeaderDecrypt(string& EncryptedXML, string& DecryptedXML, vector<uint>& HeaderKeys)
{
	
}