#include "camera.h"
#include "model_obj.h"
#include "application.h"

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

ModelOBJ::ModelOBJ(FileData* pFileData)
{
	ParseData(pFileData);
}

ModelOBJ::~ModelOBJ()
{

}

void ModelOBJ::DrawDebug()
{
	glRotatef(90, 1, 0, 0);

	glBegin(GL_TRIANGLES);
	
	for (auto& face : m_vecFaces)
	{
		glColor3fv(&m_vecVertsData[(face.norm - 1) * 3]);
		
		glVertex3fv(&m_vecVertsData[(face.vert - 1) * m_VertSize]);
	}

	glEnd();

	glRotatef(-90, 1, 0, 0);
}

void ModelOBJ::ParseData(FileData* pFileData)
{
	size_t offset = 0;
	std::string buffer;

	char* p = (char*)pFileData->Data();

	while (*p)
	{
		if (*p == '\n')
		{
			if (buffer.size() > 0)
				ParseCommand(buffer);

			buffer = "";
			p++;
			continue;
		}


		buffer += *p++;
	}
}

void ModelOBJ::ParseCommand(std::string& buffer)
{
	char commandId = buffer[0];

	switch (commandId)
	{
	case '#':
		// Comment
		break;
	case 'v':
		// Vertex commands
	{
		char subcommand = buffer[1];

		switch (subcommand)
		{
		case 'n':
			ParseNormal(buffer);
			break;
		case 't':
			ParseUV(buffer);
			break;
		default:
			ParseVertex(buffer);
			break;
		}
	}
	break;
	case 'f':
		ParseFace(buffer);
		break;
	case 's':
		// smooth shading
		break;
	case 'o':
		// object
		break;
	case 'g':
		// group
		break;
	case 'u':
		// usemtl
		break;
	case 'm':
		// external mtl
		break;
	}
}

void ModelOBJ::ParseNormal(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int normSize = 0;

	s.erase(0, 2);

	do
	{
		token = s.substr(0, pos);

		if (!token.empty())
		{
			m_vecNormalsData.push_back(stof(token));
			normSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!token.empty())
	{
		m_vecNormalsData.push_back(stof(token));
		normSize++;
	}

	if (normSize != 3)
	{
		// TODO: error message
	}
}

void ModelOBJ::ParseUV(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int newUVSize = 0;

	s.erase(0, 2);

	do
	{
		token = s.substr(0, pos);

		if (!token.empty())
		{
			m_vecUVData.push_back(stof(token));
			newUVSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!token.empty())
	{
		m_vecUVData.push_back(stof(token));
		newUVSize++;
	}

	if (m_UVSize)
	{
		if (newUVSize != m_UVSize)
		{
			// Inconsistent size...
		}
	}
	else
		m_UVSize = newUVSize;
}

void ModelOBJ::ParseVertex(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int newVertSize = 0;

	s.erase(0, 1);

	do
	{
		token = s.substr(0, pos);

		if (!token.empty())
		{
			m_vecVertsData.push_back(stof(token));
			newVertSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!token.empty())
	{
		m_vecVertsData.push_back(stof(token));
		newVertSize++;
	}

	if (m_VertSize)
	{
		if (newVertSize != m_VertSize)
		{
			// Inconsistent size...
		}
	}
	else
		m_VertSize = newVertSize;
}

void ModelOBJ::ParseFace(std::string& s)
{
	size_t pos = 0; std::string token;

	bool hasUV = m_vecUVData.size() > 0;
	bool hasNorm = m_vecNormalsData.size() > 0;

	auto parseFaceDef = [&]()
	{
		if (token[0] == '\r')
			return;

		mobjface_t face;
		memset(&face, 0, sizeof(face));

		auto elements = split(token, "/");

		if (elements.size() == 0)
		{
			Application::EPICFAIL("Error while parsing obj");
		}

		face.vert = stoi(elements[0]);

		if (hasUV)
		{
			if (!elements[1].empty())
				face.uv = stoi(elements[1]);
		}

		if (hasNorm)
		{
			if (!elements[2].empty())
				face.norm = stoi(elements[2]);
		}

		m_vecFaces.push_back(face);
	};

	s.erase(0, 1);

	do
	{
		token = s.substr(0, pos);

		if (!token.empty())
		{
			parseFaceDef();
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!token.empty())
	{
		parseFaceDef();
	}


}
