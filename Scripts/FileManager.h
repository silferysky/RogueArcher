#pragma once
#include <string>
#include "EngineIO.h"

class FileManager
{
	std::shared_ptr<std::string> m_vertexShader;
	std::shared_ptr<std::string> m_fragmentShader;
public:
	FileManager();
	~FileManager() = default;
    
	std::shared_ptr<std::string> getVertexShader();
	std::shared_ptr<std::string> getFragmentShader();
};

