#ifndef GALAGA_FORTMATION_READER
#define GALAGA_FORTMATION_READER

#include <Component.h>
#include <string>
#include <glm/vec3.hpp>
#include "rapidjson/document.h"
#include <map>

// Class that contains two json documents with information about the Formation of enemies from the current stage
class FormationReaderCP final : public engine::Component
{
public:
	FormationReaderCP(engine::GameObject* pOwner);
	~FormationReaderCP();

	void Update(const float) override;
	void ReceiveMessage(const std::string&, const std::string&) override;

	void OpenJSONFile(const std::string& jsonFilePath);
	void ClearJSONFile();

	std::multimap<std::string, glm::vec3> ReadFormation(const std::string& jsonFilePath, const std::string& enemyType);

private:
	rapidjson::Document m_FormationJsonDoc;
	bool m_IsLoaded;

};

#endif