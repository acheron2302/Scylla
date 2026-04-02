#pragma once

#include <map>
#include "PeParser.h"
#include "PeRebuild.h"
#include "Thunks.h"
#include "IATReferenceScan.h"


class ImportRebuilder : public PeParser {
public:
	ImportRebuilder(const WCHAR * file) : PeParser(file, true), peRebuild()
	{
		pImportDescriptor = 0;
		pThunkData = 0;
		pImportByName = 0;

		numberOfImportDescriptors = 0;
		sizeOfImportSection = 0;
		sizeOfApiAndModuleNames = 0;
		importSectionIndex = 0;
		useOFT = false;
		sizeOfOFTArray = 0;
		newIatInSection = false;
		BuildDirectImportsJumpTable = false;
		sizeOfJumpTable = 0;
		useCustomImageBase = false;
		customImageBase = 0;
	}

	bool rebuildImportTable(const WCHAR * newFilePath, std::map<DWORD_PTR, ImportModuleThunk> & moduleList);
	void enableOFTSupport();
	void enableNewIatInSection(DWORD_PTR iatAddress, DWORD iatSize);
	void setCustomImageBase(DWORD_PTR newImageBase);

	IATReferenceScan * iatReferenceScan;
	bool BuildDirectImportsJumpTable;
	bool useCustomImageBase;
	DWORD_PTR customImageBase;
	PeRebuild peRebuild;
private:
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor;
	PIMAGE_THUNK_DATA pThunkData;
	PIMAGE_IMPORT_BY_NAME pImportByName;

	size_t numberOfImportDescriptors;
	size_t sizeOfImportSection;
	size_t sizeOfApiAndModuleNames;
	size_t importSectionIndex;

	//OriginalFirstThunk Array in Import Section
	size_t sizeOfOFTArray;
	bool useOFT;
	bool newIatInSection;
	DWORD_PTR IatAddress;
	
	DWORD IatSize;

	DWORD sizeOfJumpTable;

	DWORD directImportsJumpTableRVA;
	BYTE * JMPTableMemory;
	DWORD newIatBaseAddressRVA;
	

	DWORD fillImportSection(std::map<DWORD_PTR, ImportModuleThunk> & moduleList);
	BYTE * getMemoryPointerFromRVA(DWORD_PTR dwRVA);

	bool createNewImportSection(std::map<DWORD_PTR, ImportModuleThunk> & moduleList);
	bool buildNewImportTable(std::map<DWORD_PTR, ImportModuleThunk> & moduleList);
	void setFlagToIATSection(DWORD_PTR iatAddress);
	size_t addImportToImportTable( ImportThunk * pImport, PIMAGE_THUNK_DATA pThunk, PIMAGE_IMPORT_BY_NAME pImportByName, DWORD sectionOffset);
	size_t addImportDescriptor(ImportModuleThunk * pImportModule, DWORD sectionOffset, DWORD sectionOffsetOFTArray);

	void calculateImportSizes(std::map<DWORD_PTR, ImportModuleThunk> & moduleList);

	void addSpecialImportDescriptor(DWORD_PTR rvaFirstThunk, DWORD sectionOffsetOFTArray);
	void patchFileForNewIatLocation();
	void changeIatBaseAddress( std::map<DWORD_PTR, ImportModuleThunk> & moduleList );
	void patchFileForDirectImportJumpTable();
};