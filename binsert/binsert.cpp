#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ostream>

using namespace std;

int main(int argc, char * argp[])
{
	if (argc >= 3) {
		std::string binPath = argp[1];
		std::string insPath = argp[2];
		int offset = atoi(argp[3]);

		ifstream bin(binPath, std::ios::binary);
		if (bin.good()) {
			ifstream ins(insPath, std::ios::binary);
			if (ins.good()) {
				{
					// find total lengths & ensure it's correct.
					ins.seekg(0, std::ios::end);
					auto patchLen = ins.tellg();
					ins.seekg(0, std::ios::beg);

					bin.seekg(0, std::ios::end);
					if (offset > bin.tellg()) {
						printf("Can't write into 0x%X when size is 0x%X!\n", offset, (int)bin.tellg());
						return -1;
					}
					
					// get original binary & read in
					auto binLen = bin.tellg();
					bin.seekg(0, std::ios::beg);
					char* binary = new char[binLen];
					bin.read(binary, binLen);
					bin.close();

					// read in the patch bytes
					char* patch = new char[patchLen];
					ins.read(patch, patchLen);
					ins.close();

					// patch in the bytes.
					for (int i = 0; i < patchLen; ++i)
						binary[offset + i] = patch[i];

					delete[] patch;

					ofstream binOut(binPath, std::ios::binary);
					if (binOut.good()) {
						binOut.write(binary, binLen);
						binOut.close();

						delete[] binary;

						printf("Patched!\n");
					}
					else {
						printf("Couldn't write to '%s'\n", binPath.c_str());
					}
				}
			}
			else {
				printf("Coudn't open patch file '%s'\n", insPath.c_str());
			}
		}
		else {
			printf("Coudn't open '%s'\n", binPath.c_str());
		}
	}
	else {
		printf("\n\tbinsert v1.00\n.\\binsert <file> <patch> <offset>\n\n");
	}
	return -1;
}