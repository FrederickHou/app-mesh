#include "Label.h"
#include "../common/Utility.h"
#include "ResourceCollection.h"

Label::Label()
{
}

Label::~Label()
{
}

web::json::value Label::AsJson()
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	auto tags = web::json::value::object();
	for (auto tag : m_labels)
	{
		tags[tag.first] = web::json::value::string(tag.second);
	}
	// add default label here
	tags[DEFAULT_LABLE_HOST_NAME] = web::json::value::string(ResourceCollection::instance()->getHostName());
	return tags;
}

const std::shared_ptr<Label> Label::FromJson(const web::json::value& obj)
{
	const static char fname[] = "Label::FromJson() ";

	std::shared_ptr<Label> label = std::make_shared<Label>();
	auto jobj = obj.as_object();
	for (auto lblJson : jobj)
	{
		std::string lableKey = GET_STD_STRING(lblJson.first);
		label->m_labels[lableKey] = GET_STD_STRING(lblJson.second.as_string());
		LOG_INF << fname << "label: " << lableKey << "=" << label->m_labels[lableKey];
	}
	return label;
}

void Label::addLabel(const std::string& name, const std::string& value)
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	m_labels[name] = value;
}

void Label::delLabel(const std::string& name)
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	if (m_labels.count(name)) m_labels.erase(name);
}
