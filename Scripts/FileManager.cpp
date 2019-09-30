#include "FileManager.h"


FileManager::FileManager()
	: m_vertexShader{ std::make_shared<std::string>() },
	  m_fragmentShader{ std::make_shared<std::string>() }
{
	*m_vertexShader = BasicIO::ReadFile("vertexShader.txt");
	*m_fragmentShader = BasicIO::ReadFile("fragmentShader.txt");
}

std::shared_ptr<std::string> FileManager::getVertexShader()
{
	return m_vertexShader;
}

std::shared_ptr<std::string> FileManager::getFragmentShader()
{
	return m_fragmentShader;
}
