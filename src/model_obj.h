#pragma once
#include "file_system.h"

typedef struct mobjface_s
{
	unsigned short vert;
	unsigned short uv;
	unsigned short norm;
}mobjface_t;

class ModelOBJ
{
public:
	ModelOBJ(FileData* pFileData);
	~ModelOBJ();

	void DrawDebug();
private:
	size_t m_VertSize = 0;
	size_t m_UVSize = 0;

	std::vector<float>	m_vecVertsData;
	std::vector<float>	m_vecUVData;
	std::vector<float>	m_vecNormalsData;

	std::vector<mobjface_t>	m_vecFaces;

	void ParseData(FileData* pFileData);
	void ParseCommand(std::string & buffer);
	void ParseNormal(std::string& buffer);
	void ParseUV(std::string& buffer);
	void ParseVertex(std::string& buffer);
	void ParseFace(std::string& buffer);
};

