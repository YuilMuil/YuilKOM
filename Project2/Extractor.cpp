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

std::string ExtractorHelper::ZlibDecompress(const std::string& str)
{
	z_stream zs;                        // z_stream is zlib's control structure
	memset(&zs, 0, sizeof(zs));

	if (inflateInit(&zs) != Z_OK)
		throw(std::runtime_error("inflateInit failed while decompressing."));

	zs.next_in = (Bytef*)str.data();
	zs.avail_in = (unsigned int)str.size();

	int ret;
	char outbuffer[32768];
	std::string outstring;

	// get the decompressed bytes blockwise using repeated calls to inflate
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = inflate(&zs, 0);

		if (outstring.size() < zs.total_out) {
			outstring.append(outbuffer,
				zs.total_out - outstring.size());
		}

	} while (ret == Z_OK);

	inflateEnd(&zs);

	if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
		std::ostringstream oss;
		oss << "Exception during zlib decompression: (" << ret << ") "
			<< zs.msg;
		throw(std::runtime_error(oss.str()));
	}

	return outstring;
}

void ExtractorHelper::MapData(vector<MappedData>& DataVec, string& XMLBuffer, ifstream& FileBuffer)
{
	pugi::xml_document doc;

	if (!doc.load_buffer(XMLBuffer.data(), XMLBuffer.length())) // If XML refuses to load, then return false.
		return;

	pugi::xml_node FileList = doc.child("Files"); // Parent node in which we will traverse from. (Files->File1,File2,File3...)

	if (FileList.empty()) // If filelist is empty then there's no need to go further.
		return;

	for (pugi::xml_node child = FileList.child("File"); child; child = child.next_sibling())
	{
		MappedData TempFileInformation;

		TempFileInformation.Name = child.attribute("Name").as_string();
		TempFileInformation.Algorithm = child.attribute("Algorithm").as_int();
		TempFileInformation.Checksum = child.attribute("Checksum").as_string();
		TempFileInformation.CompressedSize = child.attribute("CompressedSize").as_int();   //Offset to be used as the iterator end area when referencing the binary buffer
		TempFileInformation.OriginalSize = child.attribute("Size").as_int();
		TempFileInformation.FileTime = child.attribute("FileTime").as_string();
		TempFileInformation.MappedID = child.attribute("MappedID").as_string();

		TempFileInformation.FileBuffer.resize(TempFileInformation.CompressedSize);
		FileBuffer.read(TempFileInformation.FileBuffer.data(), TempFileInformation.CompressedSize);

		cout << TempFileInformation.Name << " : Algorithm: " << TempFileInformation.Algorithm << std::endl;
		switch (TempFileInformation.Algorithm)
		{
		case AlgorithmVer::NO_XOR:
			TempFileInformation.FileBuffer = ZlibDecompress(TempFileInformation.FileBuffer);
			break;
		//case AlgorithmVer::SIMPLE_XOR:
		//	DecryptionHelper::DecryptAlgo2(TempFileInformation.FileBuffer);
		//	TempFileInformation.FileBuffer = ZlibDecompress(TempFileInformation.FileBuffer);
		//	break;
		//case AlgorithmVer::ADVANCED_XOR:
		//	DecryptionHelper::DecryptAlgo3(TempFileInformation.FileBuffer);
		//	TempFileInformation.FileBuffer = ZlibDecompress(TempFileInformation.FileBuffer);
		//	break;
		//default:
		//	cerr << "Unable to find Algorithm version! Please report to YuilMuil.\n";
		//	break;
		}

		DataVec.push_back(TempFileInformation);
	}
}

void ExtractorHelper::ExportData(vector<MappedData>& DataVec, string Directory)
{
	Directory = Directory.substr(0, Directory.size() - sizeof("kom"));
	ofstream f;
	if (!fs::exists(Directory))
		fs::create_directory(Directory);
	
	for (auto& obj : DataVec)
	{
		std::ofstream output{ Directory + "\\" + obj.Name, std::ios::binary };
		output.write(&obj.FileBuffer[0], obj.FileBuffer.size());
	}
}