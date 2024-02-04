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
#include "Archiver.h"

int main(int argc, char* argv[])
{
	string Directory = "C:\\Users\\Administrator\\Downloads\\data036.kom";
	ifstream f(Directory, ios::binary);
	KOMv4 obj;

	//Check header
	obj.Header.resize(27);
	f.read(&obj.Header[0], 27);
	cout << obj.Header << "\n";
	f.ignore(41);
	f.read(reinterpret_cast<char*>(&obj.XMLSize), sizeof(uint));
	cout << "XML SIZE: " << obj.XMLSize << "\n";

	obj.XMLEncrypted.resize(obj.XMLSize);

	f.read(&obj.XMLEncrypted[0], obj.XMLSize);

	DecryptionHelper::SetHeaderKeys(obj.HeaderKeys, obj.XMLSize);
	DecryptionHelper::HeaderDecrypt(obj.XMLEncrypted, obj.XMLDecrypted, obj.HeaderKeys);

	//ofstream of(Directory + ".xml", ios::binary);
	//of << obj.XMLDecrypted;

	ExtractorHelper ex;
	ex.MapData(obj.MappedDataVec, obj.XMLDecrypted, f);
	ex.ExportData(obj.MappedDataVec, Directory);
}